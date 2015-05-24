//gfxfont.h
//转自网络 出处不祥 中文字体解决方案 修改BUG以处理重定义问题ByInsZVA
#ifndef GDIFONT_H
#define GDIFONT_H

#include <hgesprite.h>
#include <atlbase.h>
#include <stdio.h>
#pragma warning(disable:4305)

__inline float _floor(float f)
{
    static int _n;
    _asm fld f
    _asm fistp _n
    return (float)_n;
}

class GfxFont
{
public:
    static const unsigned char g_byAlphaLevel[65];
    GfxFont(const char* lpsFontName, int nFaceSize, BOOL bBold = FALSE, BOOL bItalic = FALSE, BOOL bAntialias = TRUE);
    ~GfxFont(void);

public:
    // 渲染文本
    virtual void    Print( float x, float y, const char *format, ... )
    {
        char sBuffer[10240] = {0};
        char *lpsArg=(char*)&format+sizeof(format);
        vsprintf_s(sBuffer,10240, format,lpsArg);
        Render(x,y,CA2W(sBuffer));
    }
    virtual void    Render(float x, float y, const wchar_t* text )
    {
        float offsetX = x;
        float offsetY = y;

        while(*text)
        {
            if (*text == L'\n' || *text == L'\r')
            {
                offsetX = x;
                offsetY += (m_nFontSize + m_nKerningHeight);
            }
            else
            {
                unsigned int idx = GetGlyphByCharacter(*text);
                if (idx > 0)
                {
                    m_pSprite->SetTexture(m_Glyphs[idx].t);
                    m_pSprite->SetTextureRect(0, 0, m_Glyphs[idx].w, m_Glyphs[idx].h);
                    m_pSprite->Render(offsetX - m_Glyphs[idx].x, offsetY - m_Glyphs[idx].y);
                    offsetX += (GetWidthFromCharacter(*text) + m_nKerningWidth);
                }
                else
                {
                    offsetX += (GetWidthFromCharacter(*text) + m_nKerningWidth);
                }
            }

            ++text;
        }
    }

    // 设置与获取颜色
    virtual void    SetColor( DWORD dwColor, int i = -1 )
    {
        m_pSprite->SetColor(dwColor,i);
    }
    virtual DWORD    GetColor( int i = 0 )
    {
        return m_pSprite->GetColor(i);
    }

    // 获取文本宽高
    virtual SIZE    GetTextSize( const wchar_t* text )
    {
        SIZE dim = {0, static_cast<LONG>(m_nFontSize)};
        float nRowWidth = 0;

        while(*text)
        {
            if (*text == L'\n' || *text == L'\r')
            {
                dim.cy += static_cast<LONG>(m_nFontSize + m_nKerningHeight);
                if (dim.cx < static_cast<LONG>(nRowWidth))
                    dim.cx = static_cast<LONG>(nRowWidth);
                nRowWidth = 0;
            }
            else
                nRowWidth += (GetWidthFromCharacter(*text) + m_nKerningWidth);
            ++text;
        }

        if (dim.cx < static_cast<LONG>(nRowWidth))
            dim.cx = static_cast<LONG>(nRowWidth);

        return dim;
    }

    // 根据坐标获取字符
    virtual wchar_t    GetCharacterFromPos( const wchar_t* text, float pixel_x, float pixel_y )
    {
        float x = 0;
        float y = 0;

        while (*text)
        {
            if (*text == L'\n' || *text == L'\r')
            {
                x = 0;
                y += (m_nFontSize+m_nKerningHeight);
                text++;

                if (!(*text))
                    break;
            }

            float w = GetWidthFromCharacter(*text);
            if (pixel_x > x && pixel_x <= x + w &&
                pixel_y > y && pixel_y <= y + m_nFontSize)
                return *text;

            x += (w+m_nKerningWidth);

            text++;
        }

        return L'\0';
    }

    // 设置字间距
    virtual void    SetKerningWidth( float kerning )
    {
        m_nKerningWidth = kerning;
    }
    virtual void    SetKerningHeight( float kerning )
    {
        m_nKerningHeight = kerning;
    }

    // 获取字间距
    virtual float    GetKerningWidth()
    {
        return m_nKerningWidth;
    }
    virtual float    GetKerningHeight()
    {
        return m_nKerningHeight;
    }

    // 字体大小
    virtual float    GetFontSize()
    {
        return m_nFontSize;
    }

private:
    // 根据字符获取轮廓
    unsigned int    GetGlyphByCharacter( wchar_t c )
    {
        unsigned int idx = (unsigned int)c;
        if (NULL == (m_Glyphs[idx].t)) CacheCharacter(idx,c);
        return idx;
    }
    inline float    GetWidthFromCharacter( wchar_t c, bool original = false )
    {
        unsigned int idx = GetGlyphByCharacter(c);
        if (original && idx > 0 && idx < font_count) return m_Glyphs[idx].c;
        return    (idx >= 0x2000) ? m_nFontSize : _floor(m_nFontSize / 2);
    }
    inline void        CacheCharacter(unsigned int idx, wchar_t c)
    {
        if (idx < font_count && NULL == m_Glyphs[idx].t)
        {
            UINT nChar = (UINT)c;

            MAT2 mat2 = {{0,1},{0,0},{0,0},{0,1}};
            GLYPHMETRICS gm;
            DWORD nLen = ::GetGlyphOutlineW(m_hMemDC,nChar,m_nAntialias,&gm,0,NULL,&mat2);

            HTEXTURE hTex = m_pHGE->Texture_Create(gm.gmBlackBoxX,gm.gmBlackBoxY);
            if (NULL == hTex) return;

            if((signed)nLen > 0)
            {
                LPBYTE lpBuf = new BYTE[nLen];

                if (nLen == ::GetGlyphOutlineW(m_hMemDC,nChar,m_nAntialias,&gm,nLen,lpBuf,&mat2))
                {
                    BYTE*    lpSrc = lpBuf;
                    DWORD*    lpDst = m_pHGE->Texture_Lock(hTex,FALSE);

                    if (GGO_BITMAP == m_nAntialias)
                    {
                        LONG nSrcPitch = (gm.gmBlackBoxX / 32 + (gm.gmBlackBoxX % 32 == 0 ? 0 : 1)) * 4;
                        LONG nDstPitch = m_pHGE->Texture_GetWidth(hTex);

                        for (UINT y = 0; y < gm.gmBlackBoxY; ++y)
                        {
                            for (UINT x = 0; x < gm.gmBlackBoxX; ++x)
                            {
                                for(UINT k = 0; k < 8; ++k)   
                                {
                                    UINT i = 8 * x + k;
                                    if (i >= gm.gmBlackBoxX)
                                    {
                                        x+=7;
                                        break;
                                    }
                                    lpDst[i] = ((lpSrc[x] >> (7 - k)) & 1) ? 0xFFFFFFFF : 0x0;
                                }
                            }

                            lpSrc += nSrcPitch;
                            lpDst += nDstPitch;
                        }
                    }
                    else
                    {
                        LONG nSrcPitch = (gm.gmBlackBoxX / 4 + (gm.gmBlackBoxX % 4 == 0 ? 0 : 1)) * 4;
                        LONG nDstPitch = m_pHGE->Texture_GetWidth(hTex);

                        for (UINT y = 0; y < gm.gmBlackBoxY; ++y)
                        {
                            for (UINT x = 0; x < gm.gmBlackBoxX; ++x)
                            {
                                lpDst[x] = ARGB(g_byAlphaLevel[lpSrc[x]],0xFF,0xFF,0xFF);
                            }

                            lpSrc += nSrcPitch;
                            lpDst += nDstPitch;
                        }
                    }

                    m_pHGE->Texture_Unlock(hTex);
                }

                delete lpBuf;
            }
            else
            {
                // 非正常显示字符
            }

            m_Glyphs[idx].t = hTex;
            m_Glyphs[idx].w = static_cast<float>(gm.gmBlackBoxX);
            m_Glyphs[idx].h = static_cast<float>(gm.gmBlackBoxY);
            m_Glyphs[idx].x = static_cast<float>(-gm.gmptGlyphOrigin.x);
            m_Glyphs[idx].y = static_cast<float>(-m_nAscent + gm.gmptGlyphOrigin.y);
            m_Glyphs[idx].c = static_cast<float>(gm.gmCellIncX);
        }
    }

    typedef struct tagEngineFontGlyph
    {
        HTEXTURE    t;
        float        w;
        float        h;
        float        x;
        float        y;
        float        c;
    }TENGINEFONTGLYPH;

    static const unsigned int font_count = 0xFFFF;// = sizeof(wchar_t);
    TENGINEFONTGLYPH    m_Glyphs[font_count];
    UINT                m_nAntialias;//反锯齿
    LONG                m_nAscent;//基线
    DWORD                m_dwFontColor;
    float                m_nFontSize;
    float                m_nKerningWidth;
    float                m_nKerningHeight;

    HGE*                m_pHGE;
    hgeSprite*            m_pSprite;

    // GDI设备
    HDC                    m_hMemDC;
    HFONT                m_hFont;
};




#endif//GDIFONT_H