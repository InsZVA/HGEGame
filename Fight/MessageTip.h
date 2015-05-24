#include "hge.h"
#include "hgesprite.h"
#include "GfxFont.h"
#include "Fighter.h"
#include <stdio.h>
#include <string>
#pragma once
#pragma warning(disable:4996)
class MessageTip
{
public:

	MessageTip(HGE* _hge,Fighter* _h)
	{
		hge=_hge;
		phototex=hge->Texture_Load("Graphics\\Battlers\\001-Fighter01.png");
		atktex=hge->Texture_Load("Graphics\\Icons\\001-Weapon01.png");
		dfdtex=hge->Texture_Load("Graphics\\Icons\\009-Shield01.png");
		baktex=hge->Texture_Load("Graphics\\msg.png");
		skilltex=hge->Texture_Load("Graphics\\Icons\\044-Skill01.png");
		Photo=new hgeSprite(phototex,0,0,111,170);
		Atk=new hgeSprite(atktex,0,0,24,24);
		Dfd=new hgeSprite(dfdtex,0,0,24,24);
		Back=new hgeSprite(baktex,0,0,1024,256);
		Skill=new hgeSprite(skilltex,0,0,24,24);
		Photo->SetZ(0.1);
		Photo->SetHotSpot(55,85);
		Atk->SetZ(0.1);
		Dfd->SetZ(0.1);
		Back->SetZ(0.15);
		Skill->SetZ(0.1);
		f=new GfxFont("����",24);
		t=new GfxFont("����",12);
		f->SetColor(ARGB(255,255,255,255));
		t->SetColor(ARGB(255,255,255,255));
		currentmsg=-1;
		strcpy(debugmsg,"");
		loaded=false;
		h=_h;
	}

	~MessageTip(void)
	{
		delete Photo,delete Atk,delete Dfd,delete Back;
		delete f;
	}

	void Release()
	{
		hge->Effect_Free(phototex);
		hge->Effect_Free(atktex);
		hge->Effect_Free(dfdtex);
		hge->Effect_Free(baktex);
	}

	void Render()
	{
		Photo->Render(96,128+512);
		Atk->Render(274,560);
		Dfd->Render(274,600);
		Skill->Render(274,640);
		Back->Render(0,512);
		
		f->Print(298,560,"%d",h->atk);
		f->Print(298,600,"%d",h->dfd);
		f->Print(36,720,"HP %d/%d +%d",h->hp,h->hpmax,h->hprec);
		f->Print(36,756,"MP %d/%d +%d",h->mp,h->mpmax,h->mprec);
		f->Print(298,640,"�׻�-Lv1 [J]");
		
		t->Print(260,696,"�ٻ��׵繥�����ˣ����������*1�˺���");
		//t->Print(260,708,"Ŀ��㣺4 �˺���Χ��1 ��ȴʱ�䣺1");

		t->Print(260,732,"��%d  ˮ��%d  �ף�%d  �磺%d",h->fire,h->water,h->storm,h->wind);

		t->Print(0,0,debugmsg);
		if(currentmsg!=-1){
			f->Print(750,560,msg[currentmsg]);}
	}

	void LoadMsg(char* path)
	{
		FILE * file;
		file=fopen(path,"r");
		for(int i=0;i<12;i++)
		{
			fscanf(file,"%d %d %s",&msgx[i],&msgy[i],&msg[i]);
			if(feof(file)){msgnum=i+1;break;}
		}
		fclose(file);
		loaded=true;
	}

	void CheckMsg(int x,int y)
	{
		if(!loaded)return;
		currentmsg=-1;
		sprintf(debugmsg,"CheckMsg x:%d y:%d",x,y);
		for(int i=0;i<msgnum;i++)
		{
			if(x==msgx[i]&&y==msgy[i])currentmsg=i;
		}
	}
private:
	hgeSprite * Photo, * Atk, * Dfd, * Back, *Skill;//�ܹ�ռ1024*256��ͷ��Ϊ���е�196*220 
	HTEXTURE phototex,atktex,dfdtex,baktex,skilltex;
	GfxFont * f,* t;
	HGE * hge;
	char msg[12][128];
	int msgx[12],msgy[12];
	int msgnum;
	int currentmsg;
	bool loaded;
	char debugmsg[128];
	Fighter *h;
};

