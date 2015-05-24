#include "hge.h"
#include "hgesprite.h"
#include "hgeanim.h"
#pragma once
class SkillAnimation
{
public:

	SkillAnimation(HGE* _hge)
	{
		hge=_hge;
		x=y=128;
		state=0;
	}

	virtual ~SkillAnimation(void)
	{
		delete ani;
	}

	virtual void Render()
	{
		if(state==1)ani->Render(x,y);
	}

	virtual void Update(float delta)
	{
		if(state!=1)return;
		ani->Update(delta);
		if(!ani->IsPlaying())state=2;
	}

	virtual void Release()
	{
		hge->Texture_Free(tex);
	}

	virtual void Play(float _x,float _y)
	{
		x=_x;
		y=_y;
		state=1;
		ani->Play();
		hge->Effect_Play(snd);
		
	}
	
	unsigned int GetState()
	{
		return state;
	}
protected:
	HTEXTURE tex;
	HEFFECT snd;
	hgeAnimation * ani;
	float x,y;
	HGE *hge;
	unsigned int state;//0-就绪 1-已开始播放 2-已结束
};

