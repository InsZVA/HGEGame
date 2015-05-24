#pragma once
#include "unit.h"
#include "hgeanim.h"
#include "hgesprite.h"
#include "SkillThunder.h"
#include "Map.h"
#include "Fighter.h"

#pragma warning(disable:4244)
class Hero:Unit
{
public:
	
	Hero(HGE* _hge):Unit(_hge,32,32,32,32)
	{
		st=new SkillThunderAnim(_hge);
		fighter=new Fighter("Fighter01","Graphics\\Battlers\\001-Fighter01.png",100,100,100,100,1,1,12,5,5,12,5,3);
		//map=_map;
		dir=0;
		x=600,y=300;
		tex01=hge->Texture_Load("Graphics\\Characters\\001-Fighter01.png");
		sndmove=hge->Effect_Load("Audio\\SE\\013-Move01.ogg");
		for(int i=0;i<4;i++)
		{
			Move[i]=new hgeAnimation(tex01,4,4,0,48*i,32,48);
			Move[i]->SetSpeed(4);
			Move[i]->SetZ(0.5);
			Move[i]->SetHotSpot(16,45);
		}
		Move[dir]->Play();
		tcd=0;
		channelmove=hge->Effect_Play(sndmove);
		hge->Channel_Pause(channelmove);
	}
	void SetMap(Map* _map)
	{map=_map;}
	~Hero(void)
	{
		delete Move[0];
		delete Move[1];
		delete Move[2];
		delete Move[3];
		st->~SkillThunderAnim();
		fighter->~Fighter();
	}
	void Release()
	{
		fighter->Release();
		hge->Texture_Free(tex01);
		hge->Effect_Free(sndmove);
		st->Release();
	}
	void GetCmd(unsigned int cmd,float delta)
	{
		hge->Channel_Pause(channelmove);
		
		ndir=dir;
		
		switch(cmd)
		{
			
			case HGEK_W:ndir=3;if(y>32&&map->Pass(ndir,x,y,32*delta))y-=32*delta;hge->Channel_Resume(channelmove);break;
			case HGEK_S:ndir=0;if(y<512&&map->Pass(ndir,x,y,32*delta))y+=32*delta;hge->Channel_Resume(channelmove);break;
			case HGEK_A:ndir=1;if(x>16&&map->Pass(ndir,x,y,32*delta))x-=32*delta;hge->Channel_Resume(channelmove);break;
			case HGEK_D:ndir=2;if(x<1024-16&&map->Pass(ndir,x,y,32*delta))x+=32*delta;hge->Channel_Resume(channelmove);break;
			case HGEK_J:
				if(tcd==0)
				{
					tcd=1;
					int tx=x,ty=y;
					switch(ndir)
					{
					case 0:ty+=128;break;
					case 1:tx-=128;break;
					case 2:tx+=128;break;
					case 3:ty-=128;break;
					}
					st->Play(tx,ty);
					
				}break;
		}
		if(ndir!=dir){
			Move[dir]->Stop();
			Move[ndir]->Play();
			dir=ndir;
		}
	}
	void Update(float delta)
	{
		tcd-=delta;
		if(tcd<0)tcd=0;
		Move[dir]->Update(delta);
		st->Update(delta);
	}
	void Render()
	{
		Move[dir]->Render(x, y);
		st->Render();
	}
private:
	hgeAnimation * Move[4];
	HTEXTURE tex01;
	unsigned int dir,ndir;
	SkillThunderAnim * st;
	float tcd;
	HEFFECT sndmove;
	HCHANNEL channelmove;
	Map * map;
public:
	Fighter* fighter;
};

