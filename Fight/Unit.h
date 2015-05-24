#pragma once
#include <hge.h>
#pragma warning(disable:4244)
class Unit
{
public:

	Unit(HGE* _hge,int _x,int _y,int _w,int _h)
	{
		hge=_hge;
		x=_x,y=_y,w=_w,h=_h;
	}

	virtual ~Unit(void)
	{
	}
	float x,y,w,h;
protected:
	
	HGE* hge;
};

