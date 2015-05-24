#include "GfxFont.h"
const unsigned char GfxFont::g_byAlphaLevel[65] = 
{
    0,  4,  8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48,
    52, 56, 60, 64, 68, 72, 76, 80, 84, 88, 92, 96,100,
    104,108,112,116,120,124,128,132,136,140,144,148,152,
    156,160,164,168,172,176,180,184,188,192,196,200,204,
    208,212,216,220,224,228,232,236,240,244,248,252,255
};
GfxFont::GfxFont(const char* lpsFontName, int nFaceSize, BOOL bBold, BOOL bItalic, BOOL bAntialias)
{
	m_pHGE = hgeCreate(HGE_VERSION);

        // 创建GDI相关设备
        HDC hDC = GetDC(m_pHGE->System_GetState(HGE_HWND));
        m_hMemDC = CreateCompatibleDC(hDC);
        if (NULL == m_hMemDC) return;
        ReleaseDC(m_pHGE->System_GetState(HGE_HWND),hDC);

        ::SetMapMode(m_hMemDC, MM_TEXT);
        ::SetTextColor(m_hMemDC,RGB(255,255,255));
        ::SetBkColor(m_hMemDC,RGB(0,0,0));

        m_hFont = CreateFontA(
            -nFaceSize,
            0,
            0,
            0,
            (bBold) ? FW_BOLD : FW_NORMAL,
            bItalic,
            FALSE,
            FALSE,
            DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY,
            FF_DONTCARE | DEFAULT_PITCH,
            lpsFontName);
        if (NULL == (m_hFont)) return;
        SelectObject(m_hMemDC, m_hFont);

        memset(m_Glyphs,0,sizeof(TENGINEFONTGLYPH)*font_count);

        m_nAntialias    = bAntialias ? GGO_GRAY8_BITMAP : GGO_BITMAP;

        TEXTMETRIC tm;
        ::GetTextMetrics(m_hMemDC,&tm);
        m_nAscent        = tm.tmAscent;

        m_nFontSize        = static_cast<float>(nFaceSize);
        m_nKerningWidth    = 0;
        m_nKerningHeight= 0;

        m_pSprite = new hgeSprite(0, 0, 0, 0, 0);
        m_pSprite->SetColor(ARGB(255, 255, 255, 255));
		m_pSprite->SetZ(0);
}
GfxFont::~GfxFont(void)
    {
        for (int nIdx = 0; nIdx < font_count; ++nIdx)
        { if (m_Glyphs[nIdx].t) m_pHGE->Texture_Free(m_Glyphs[nIdx].t); }

        if ((m_hFont)) DeleteObject(m_hFont);
        if ((m_hMemDC)) DeleteDC(m_hMemDC);

        if(m_pSprite) delete m_pSprite;
        if(m_pHGE) m_pHGE->Release();
    }