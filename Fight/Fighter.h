#pragma once
#include <string>
#include <hge.h>
#include <hgesprite.h>
#include "SkillAnimation.h"
class Fighter;
class Skill
{
public:

	Skill(HGE* _hge)
	{
		hge=_hge;
		started = false;
	}

	virtual ~Skill(void)
	{
		if(ani)ani->~SkillAnimation();
	}

	virtual void Release()
	{
		if(ani)ani->Release();
	}

	virtual void Attak(Fighter* f)
	{
		ani->Play(768, 480);
		started = true;
	}

	bool isdone()
	{
		if (!started)return true;
		return ani->GetState() == 2;
	}
protected:
	SkillAnimation * ani;
	HGE* hge;
	int basedamage;
	int windx,waterx,firex,thunderx;
	bool started;
};
class Fighter
{
public:
	Skill* skill;
	int hp,hpmax,mp,mpmax,hprec,mprec,atk,dfd,wind,storm,water,fire;
	char name[128];
	bool acted;
	Fighter(char * _name,char* texpath,int _hp,int _hpmax,int _mp,int _mpmax,int _hprec,int _mprec,int _atk,int _dfd,int _wind,int _storm,int _water,int _fire)
	{
		hp=_hp,hpmax=_hpmax,mp=_mp,mpmax=_mpmax,hprec=_hprec,mprec=_mprec,atk=_atk,dfd=_dfd,wind=_wind,storm=_storm,water=_water,fire=_fire;
		strcpy(name,_name);
		tex=(hge=hgeCreate(HGE_VERSION))->Texture_Load(texpath);
		sprite=new hgeSprite(tex,0,0,hge->Texture_GetWidth(tex),hge->Texture_GetHeight(tex));
		sprite->SetZ(0.5);
		sprite->SetHotSpot(hge->Texture_GetWidth(tex)/2,hge->Texture_GetHeight(tex));
		x=256;
		y=480;
		skill=0;
		acted = false;
	}

	virtual ~Fighter(void)
	{
		delete sprite;
		if(hge)hge->Release();
	}

	virtual void Release()
	{
		hge->Texture_Free(tex);
		if(skill)skill->Release();
	}

	virtual void AddSkill(Skill * skill0)
	{
		skill=skill0;//Reversed
	}

	virtual void Render()
	{
		sprite->Render(x,y);
	}
protected:
	HTEXTURE tex;
	HGE* hge;
	hgeSprite * sprite;
	float x,y;

};

