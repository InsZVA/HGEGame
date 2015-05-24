#include "hge.h"//����hgeͷ�ļ�
#include "hgeanim.h"
#include "hgesprite.h"
#include <hgeparticle.h>
#include "Hero.h"
#include "Map.h"
#include "MessageTip.h"
#include "hgegui.h"
#include "menuitem.h"
#include "CollisionSystem.h"//����

HGE *hge=0;//����һ��ָ��hge���ָ�롣 
Hero * h;
Map * map;
unsigned int GAME_SCENE;//0-menu 1-mainmap 2-thanks 3-Fight
static char cd[256];
HEFFECT bgm, bgm0, bgm1;
HCHANNEL bgmchannel,bgm0channel,bgm1channel;
MessageTip * mt;
static HEFFECT sndmenu;
static hgeSprite * cursor, *menuback, *spr, *spr2;
static HTEXTURE cursortex, menubacktex, partex;
static hgeGUI * gui;
static hgeParticleSystem* par, *par2;
static float parx = 512, pary = 484, par2x = 128, par2y = 128;
static float thankstime = 20;
static GfxFont * ft;
static CollisionSystem cs;
static HTEXTURE fightbacktex;
static hgeSprite * fightback;
Enemy* e = 0;
void PKRender();
bool RenderFunc()//���ƺ���������ʼ��HGE����ͣ������ 
{ 
	hge->Gfx_BeginScene();  
    hge->Gfx_Clear(0); 
	switch(GAME_SCENE)
	{
	case 0:
		menuback->Render(0,0);
		par->Render();
		par2->Render();
		gui->Render();break;
	case 1:
		map->Render();  
		h->Render();
		//mt->Render();
		break;
	case 2:
		menuback->Render(0,0);
		par->Render();
		par2->Render();
		ft->SetColor(ARGB(255,0,0,0));
		ft->Print((1024-48)/2+2,180+2,"��л");
		ft->Print(150+2,250+2,"��л�����ṩ��GfxFont�����ֿ⣨���ض���BUG���Լ����޸�����");
		ft->Print(150+2,279+2,"��лHGE�ٷ����̵�GUI���뼰�زģ��Լ��޸�����Ӧ����Ϸ����");
		ft->Print(150+2,308+2,"��лHGE���漰��ع����ĵ������ı���hge.relishgames.com��");
		ft->Print(150+2,337+2,"��лRPG��Ϸ��ʦ�д�����ͼ����Ч�����زģ�");
		ft->Print(150+2,366+2,"��лVS2010��Photoshop�������ߴ����ı�����");
		ft->Print(150+2,395+2,"��л�����Ƕ��ҵĴ���֧�֣�");
		ft->Print(600+2,500+2,"InsZVA ������");
		ft->Print(560+2,530+2,"2015��2�� �±��Ӵ�");
		ft->SetColor(ARGB(255,255,255,255));
		ft->Print((1024-48)/2,180,"��л");
		ft->Print(150,250,"��л�����ṩ��GfxFont�����ֿ⣨���ض���BUG���Լ����޸�����");
		ft->Print(150,279,"��лHGE�ٷ����̵�GUI���뼰�زģ��Լ��޸�����Ӧ����Ϸ����");
		ft->Print(150,308,"��лHGE���漰��ع����ĵ������ı���hge.relishgames.com��");
		ft->Print(150,337,"��лRPG��Ϸ��ʦ�д�����ͼ����Ч�����زģ�");
		ft->Print(150,366,"��лVS2010��Photoshop�������ߴ����ı�����");
		ft->Print(150,395,"��л�����Ƕ��ҵĴ���֧�֣�");
		ft->Print(600,500,"InsZVA ������");
		ft->Print(560,530,"2015��2�� �±��Ӵ�");
		//gui->Render();
		break;
	case 3:
		
		PKRender();
		break;
	}
    hge->Gfx_EndScene();
	return false;//��������ʱ���Ƿ���false������true����System_Start����ִ�� 
} 
void AI(Enemy * e)
{

	e->acted = true;
}
void PKUpdate(float delta)
{
	static bool animdone = true;
	static unsigned int roundnum = 0;
	if (roundnum % 2 == 0)
	{
		if (h->fighter->skill->isdone() && h->fighter->acted){
			roundnum++;
			e->acted = false;
			AI(e);
		}
	}
	else
	{
		if (e->skill->isdone() && e->acted)
		{
			roundnum++;
			h->fighter->acted = false;
		}
	}

}
void PKRender()
{
	map->mt->Render();
	fightback->Render(0, 0);
	h->fighter->Render();
	e->Render();
}
bool FrameFunc()//�߼�����������ʼ��HGE����ͣ��������һЩ�߼��жϻ��ߴ������д����� 
{ 
	float delta=hge->Timer_GetDelta();
	static int lastid=0;
		float x,y;
	switch(GAME_SCENE)
	{
	case 0:
		int id;
		if(!hge->Channel_IsPlaying(bgm0channel))hge->Channel_Resume(bgm0channel);//ά�ֱ�������
		
		// If ESCAPE was pressed, tell the GUI to finish
		if(hge->Input_GetKeyState(HGEK_ESCAPE)) { lastid=3;gui->Leave(); return true;}
	
		// We update the GUI and take an action if
		// one of the menu items was selected
		id=gui->Update(delta);
		//if(id==-1)
		//{

			switch(id)
			{
			case 1:
				gui->Leave();
				par->Stop();
				par2->Stop();
				bgmchannel=hge->Effect_Play(bgm);
				hge->Channel_Stop(bgm0channel);
				GAME_SCENE=1;
				break;
			case 2:
				gui->Leave();
				GAME_SCENE=2;
				break;
			case 3:
				gui->Leave();
				return true;
			}
		//}
		//else 
		//	if(id){lastid=id;}
		
		hge->Input_GetMousePos(&x,&y);
		if(x>parx)parx+=300*delta;else parx-=300*delta;
		if(y>pary)pary+=300*delta;else pary-=300*delta;
		par->MoveTo(parx,pary);
		par->Update(delta);
		if(par2x<=1024-128&&par2y<=128)par2x+=60*delta;
		else if(par2x>1024-128&&par2y<=768-128)par2y+=70*delta;
		else if(par2x>128&&par2y>768-128)par2x-=80*delta;
		else par2y-=90*delta;
		par2->MoveTo(par2x,par2y);
		par2->Update(delta);
	break;
	case 1:
		if(!hge->Channel_IsPlaying(bgmchannel))hge->Channel_Resume(bgmchannel);//ά�ֱ�������
		//�������
		if(hge->Input_GetKeyState(HGEK_ESCAPE))return true;
		if(hge->Input_GetKeyState(HGEK_J)){h->GetCmd(HGEK_J,delta);}
		if(hge->Input_GetKeyState(HGEK_W)){h->GetCmd(HGEK_W,delta);goto ordered;}
		if(hge->Input_GetKeyState(HGEK_S)){h->GetCmd(HGEK_S,delta);goto ordered;}
		if(hge->Input_GetKeyState(HGEK_A)){h->GetCmd(HGEK_A,delta);goto ordered;}
		if(hge->Input_GetKeyState(HGEK_D)){h->GetCmd(HGEK_D,delta);goto ordered;}
	ordered:
		//��������
		
		h->Update(delta);
		break;
	case 2:
		if(!hge->Channel_IsPlaying(bgm0channel))hge->Channel_Resume(bgm0channel);//ά�ֱ�������
		thankstime-=delta;
		if(thankstime<=0){thankstime=20;gui->SetFocus(1);gui->Enter();GAME_SCENE=0;}
		hge->Input_GetMousePos(&x,&y);
		if(x>parx)parx+=300*delta;else parx-=300*delta;
		if(y>pary)pary+=300*delta;else pary-=300*delta;
		par->MoveTo(parx,pary);
		par->Update(delta);
		if(par2x<=1024-128&&par2y<=128)par2x+=60*delta;
		else if(par2x>1024-128&&par2y<=768-128)par2y+=70*delta;
		else if(par2x>128&&par2y>768-128)par2x-=80*delta;
		else par2y-=90*delta;
		par2->MoveTo(par2x,par2y);
		par2->Update(delta);
		break;
	case 3:
		if(!hge->Channel_IsPlaying(bgm1channel))hge->Channel_Resume(bgm1channel);
	}
	return false;//���Ƿ���false 
} 


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)//WinMain�������������ڡ� 
{ 
	//GetCurrentDirectoryA(256,cd);
	hge=hgeCreate(HGE_VERSION);//ʹ��hgeCreate��������HGE�ӿڣ��������봫����ȷ��HGE_VERSION,������hge.h�ж���� 
	hge->System_SetState(HGE_SCREENWIDTH, 1024);
	hge->System_SetState(HGE_SCREENHEIGHT,768);
	hge->System_SetState(HGE_SHOWSPLASH,false);
	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);//�����߼�����ΪFrameFunc���� 
	hge->System_SetState(HGE_RENDERFUNC,RenderFunc);//���û��ƺ���ΪRenderFunc����
	hge->System_SetState(HGE_TITLE, "Fight For The Ancints"); 
	hge->System_SetState(HGE_WINDOWED,true);
	hge->System_SetState(HGE_USESOUND,true);
	hge->System_SetState(HGE_ZBUFFER,true);
	
	if(hge->System_Initiate())//��hge���System_Initiate()����������ʼ���Ƿ��д�����֡� 
	{ 
		sndmenu=hge->Effect_Load("Audio\\menu.wav");
		bgm=hge->Effect_Load("Audio\\BGM\\051-Positive09.ogg");
		bgm0=hge->Effect_Load("Audio\\BGM\\058-Slow01.ogg");
		bgm1=hge->Effect_Load("Audio\\BGM\\001-Battle01.ogg");
		cursortex=hge->Texture_Load("Graphics\\cursor.png");
		menubacktex=hge->Texture_Load("Graphics\\menuback.jpg");
		fightbacktex=hge->Texture_Load("Graphics\\Battlebacks\\converted\\JPEG\\043-Cave01.jpg");
		partex=hge->Texture_Load("particles.png");
		cursor=new hgeSprite(cursortex,0,0,32,32);
		menuback=new hgeSprite(menubacktex,0,0,1024,768);
		(fightback=new hgeSprite(fightbacktex,0,0,1024,512))->SetZ(1.0f);
		spr=new hgeSprite(partex,32,32,32,32);
		spr->SetHotSpot(16,16);
		spr->SetBlendMode(BLEND_COLORMUL | BLEND_ALPHAADD);
		spr->SetZ(0.00001);
		spr2=new hgeSprite(partex,0,32,32,32);
		spr2->SetHotSpot(16,16);
		spr2->SetBlendMode(BLEND_COLORMUL | BLEND_ALPHAADD);
		spr2->SetZ(0.00001);
		par=new hgeParticleSystem("particle4.psi",spr);
		par2=new hgeParticleSystem("particle9.psi",spr2);
		menuback->SetZ(1);
		cursor->SetZ(0);

		gui=new hgeGUI();
		gui->AddCtrl(new hgeGUIMenuItem(1,sndmenu,576,250,0.0f,"�µ���Ϸ"));
		gui->AddCtrl(new hgeGUIMenuItem(2,sndmenu,576,350,0.5f,"�ر���л"));
		gui->AddCtrl(new hgeGUIMenuItem(3,sndmenu,576,450,1.0f,"�˳���Ϸ"));
		gui->SetNavMode(HGEGUI_UPDOWN | HGEGUI_CYCLED);
		gui->SetCursor(cursor);
		gui->SetFocus(1);
		gui->Enter();
		
		ft=new GfxFont("����",24);
		
		//mt=new MessageTip(hge);
		
		h=new Hero(hge);
		map=new Map(hge,"Graphics\\Tilesets\\043-Cave01.png",h->fighter);
		h->SetMap(map);
		map->LoadMap_s("map.txt");
		map->LoadEnemy("enemy.txt");
		bgm0channel=hge->Effect_Play(bgm0);
		par->Fire();
		par2->Fire();

		hge->System_Start();//���û�����⣬��ʹ��System_Start��������ʼ���� 

		hge->Effect_Free(bgm);
		hge->Effect_Free(bgm0);
		hge->Effect_Free(bgm1);
		hge->Effect_Free(sndmenu);
		hge->Texture_Free(cursortex);
		hge->Texture_Free(menubacktex);
		hge->Texture_Free(partex);
		hge->Texture_Free(fightbacktex);
		h->Release();
		map->Release();
		//mt->Release();
	} 
	h->~Hero();
	map->~Map();
	//mt->~MessageTip();
	delete gui,delete cursor,delete menuback;
	delete par,delete spr,delete spr2,delete par2;
	delete ft,delete fightback;
	hge->System_Shutdown();//����ֹͣ 
	hge->Release();//�ͷ�HGE��ռ�õ��ڴ档 
}