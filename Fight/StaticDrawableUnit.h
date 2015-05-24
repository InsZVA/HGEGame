#pragma once
#include "unit.h"
#include <hgesprite.h>
class StaticDrawableUnit :
	public Unit
{
public:
	StaticDrawableUnit(HGE* _hge,char* texpath,int _tx,int _ty,int x,int y,int z,int w,int h,bool _ispass):Unit(_hge,x,y,w,h)
	{
		tex01=hge->Effect_Load(texpath);
		spr=new hgeSprite(tex01,_tx,_ty,w,h);
		spr->SetZ(z);
		spr->SetHotSpot(w/2,h);
		ispass=_ispass;
	}
	StaticDrawableUnit(HGE* _hge,HTEXTURE tex,int _tx,int _ty,int x,int y,int z,int w,int h,bool _ispass):Unit(_hge,x,y,w,h)
	{
		tex01=tex;
		spr=new hgeSprite(tex,_tx,_ty,w,h);
		spr->SetZ(z);
		spr->SetHotSpot(w/2,h);
		ispass=_ispass;
	}
	virtual ~StaticDrawableUnit(void)
	{
		delete spr;
	}

	virtual void Release()
	{
		hge->Texture_Free(tex01);
	}

	virtual void Render()
	{
		spr->Render(x,y);
	}

	virtual bool IsPass()
	{
		return ispass;
	}
protected:
	HTEXTURE tex01;
	hgeSprite* spr;
	bool ispass;//ÊÇ·ñÍ¨ÐÐ
};

