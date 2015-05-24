#pragma once
#include "hge.h"
#include "hgesprite.h"
#include "stdio.h"
#include "MessageTip.h"
#include "Fighter.h"
#include "Enemy.h"
#include <string>
//#include "StaticDrawableUnit.h"
extern HCHANNEL bgm1channel,bgmchannel;
#pragma warning(disable:4996)
#pragma warning(disable:4244)
class Map
{
private:
	HGE* hge;
	hgeSprite * tile[32][16];
	hgeSprite * back;
	HTEXTURE tex;
	unsigned int ispass[32][16];
	unsigned int data[32][16];
	bool loaded;
	
	
	hgeSprite * enemyspr[12];
	HTEXTURE enemytex[12];
	int enemynum;
	int enemyx[12],enemyy[12];
	bool enemyloaded;
public:
	MessageTip * mt;
	Enemy* enemy[12];
public:
	Map(HGE* _hge,char * path,Fighter* _h)
	{
		hge=_hge;
		tex=hge->Texture_Load(path);
		loaded=false;
		back=new hgeSprite(tex,0,0,32,32);
		back->SetZ(1);
		memset(data,0,32*16*4);
		memset(ispass,0,32*16*4);
		mt=new MessageTip(hge,_h);
		enemyloaded=false;
	}

	~Map(void)
	{
		for(int i=0;i<32;i++)
			for(int j=0;j<16;j++)
				delete tile[i][j];
		delete back;
		for(int i=0;i<enemynum;i++)
		{
			enemy[i]->~Enemy();
			delete enemyspr[i];
		}
		mt->~MessageTip();
	}
	void LoadTexture(char * path)
	{
		tex=hge->Texture_Load(path);
	}
	void LoadMap_s(char * path)
	{
		if(loaded)return;
		FILE * f=fopen(path,"r");
		for(int j=0;j<16;j++)
			for(int i=0;i<32;i++)
			{
				fscanf(f,"%d",&data[i][j]);
				fscanf(f,"%d",&ispass[i][j]);
				//tile[i][j]=new StaticDrawableUnit(hge,tex,data[i][j]%32,data[i][j]/32,32*i,32*j,0.95,32,32,true);
				tile[i][j]=new hgeSprite(tex,data[i][j]%8*32,data[i][j]/8*32,32,32);
				if(data[i][j]&&ispass[i][j])tile[i][j]->SetZ((float)0.1);else tile[i][j]->SetZ(1);
			}
		fclose(f);
		loaded=true;
		mt->LoadMsg("msg.txt");
	}
	void LoadEnemy(char* path)
	{
		FILE * file;
		int hp,hpmax,mp,mpmax,hprec,mprec,atk,dfd,wind,storm,water,fire;
		char texpath[128],name[128],tex[128];
		file=fopen(path,"r");
		for(int i=0;i<12;i++)
		{
			fscanf(file,"%d %d %s %s %d %d %d %d %d %d %d %d %d %d %d %d %s",&enemyx[i],&enemyy[i],name,
				texpath,&hp,&hpmax,&mp,&mpmax,&hprec,&mprec,&atk,&dfd,&wind,&storm,&water,&fire,tex);
			enemy[i]=new Enemy(name,texpath,hp,hpmax,mp,mpmax,hprec,mprec,atk,dfd,wind,storm,water,fire);
			enemytex[i]=hge->Texture_Load(tex);
			enemyspr[i]=new hgeSprite(enemytex[i],0,0,32,32);
			enemyspr[i]->SetZ(0.6);
			if(feof(file)){enemynum=i+1;break;}
		}
		fclose(file);
		enemyloaded=true;
	}
	void Render()
	{
		if(!loaded)return;
		for(int i=0;i<32;i++)
			for(int j=0;j<16;j++)
				{
					if(data[i][j])back->Render(i*32,j*32);
					tile[i][j]->Render(i*32,j*32);
				}
		for(int i=0;i<enemynum;i++)
		{
			enemyspr[i]->Render(enemyx[i]*32,enemyy[i]*32);
		}
		mt->Render();
	}
	void Release()
	{
		mt->Release();
		for(int i=0;i<enemynum;i++)
		{
			enemy[i]->Release();
			hge->Texture_Free(enemytex[i]);
		}
		hge->Texture_Free(tex);
	}
	unsigned int Pass(int dir,float x,float y,float buf);


	//StaticDrawableUnit *tile[32][16];
};
