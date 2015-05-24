#pragma once
#include "fighter.h"
//
class Enemy :
	public Fighter
{
public:

	Enemy(char* _name,char* texpath,int _hp,int _hpmax,int _mp,int _mpmax,int _hprec,int _mprec,int _atk,int _dfd,int _wind,int _storm,int _water,int _fire):Fighter(_name,texpath,_hp,_hpmax,_mp,_mpmax,_hprec,_mprec,_atk,_dfd,_wind,_storm,_water,_fire)
	{
		x=768;
	}

	virtual ~Enemy(void)
	{
	}
};

