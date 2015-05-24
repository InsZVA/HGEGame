#include "hge.h"//包含hge头文件
#include "hgeanim.h"
#include "hgesprite.h"
#include <hgeparticle.h>
#include "Hero.h"
#include "Map.h"
#include "MessageTip.h"
#include "hgegui.h"
#include "menuitem.h"
#include "CollisionSystem.h"//暂留

HGE *hge=0;//创建一个指向hge类的指针。 
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
bool RenderFunc()//绘制函数，程序开始后HGE将不停调用它 
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
		ft->Print((1024-48)/2+2,180+2,"鸣谢");
		ft->Print(150+2,250+2,"感谢网友提供的GfxFont中文字库（有重定义BUG，自己已修复）；");
		ft->Print(150+2,279+2,"感谢HGE官方例程的GUI代码及素材（自己修改以适应本游戏）；");
		ft->Print(150+2,308+2,"感谢HGE引擎及相关工具文档带来的便利hge.relishgames.com；");
		ft->Print(150+2,337+2,"感谢RPG游戏大师中大量的图像及音效音乐素材；");
		ft->Print(150+2,366+2,"感谢VS2010及Photoshop开发工具带来的便利；");
		ft->Print(150+2,395+2,"感谢朋友们对我的大力支持！");
		ft->Print(600+2,500+2,"InsZVA 李自乐");
		ft->Print(560+2,530+2,"2015年2月 新堡子村");
		ft->SetColor(ARGB(255,255,255,255));
		ft->Print((1024-48)/2,180,"鸣谢");
		ft->Print(150,250,"感谢网友提供的GfxFont中文字库（有重定义BUG，自己已修复）；");
		ft->Print(150,279,"感谢HGE官方例程的GUI代码及素材（自己修改以适应本游戏）；");
		ft->Print(150,308,"感谢HGE引擎及相关工具文档带来的便利hge.relishgames.com；");
		ft->Print(150,337,"感谢RPG游戏大师中大量的图像及音效音乐素材；");
		ft->Print(150,366,"感谢VS2010及Photoshop开发工具带来的便利；");
		ft->Print(150,395,"感谢朋友们对我的大力支持！");
		ft->Print(600,500,"InsZVA 李自乐");
		ft->Print(560,530,"2015年2月 新堡子村");
		//gui->Render();
		break;
	case 3:
		
		PKRender();
		break;
	}
    hge->Gfx_EndScene();
	return false;//程序正常时总是返回false，返回true将从System_Start往下执行 
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
bool FrameFunc()//逻辑函数，程序开始后HGE将不停调用它，一些逻辑判断或者处理可以写在这里。 
{ 
	float delta=hge->Timer_GetDelta();
	static int lastid=0;
		float x,y;
	switch(GAME_SCENE)
	{
	case 0:
		int id;
		if(!hge->Channel_IsPlaying(bgm0channel))hge->Channel_Resume(bgm0channel);//维持背景音乐
		
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
		if(!hge->Channel_IsPlaying(bgmchannel))hge->Channel_Resume(bgmchannel);//维持背景音乐
		//按键监控
		if(hge->Input_GetKeyState(HGEK_ESCAPE))return true;
		if(hge->Input_GetKeyState(HGEK_J)){h->GetCmd(HGEK_J,delta);}
		if(hge->Input_GetKeyState(HGEK_W)){h->GetCmd(HGEK_W,delta);goto ordered;}
		if(hge->Input_GetKeyState(HGEK_S)){h->GetCmd(HGEK_S,delta);goto ordered;}
		if(hge->Input_GetKeyState(HGEK_A)){h->GetCmd(HGEK_A,delta);goto ordered;}
		if(hge->Input_GetKeyState(HGEK_D)){h->GetCmd(HGEK_D,delta);goto ordered;}
	ordered:
		//其他更新
		
		h->Update(delta);
		break;
	case 2:
		if(!hge->Channel_IsPlaying(bgm0channel))hge->Channel_Resume(bgm0channel);//维持背景音乐
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
	return false;//总是返回false 
} 


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)//WinMain函数，程序的入口。 
{ 
	//GetCurrentDirectoryA(256,cd);
	hge=hgeCreate(HGE_VERSION);//使用hgeCreate函数创建HGE接口，参数必须传递正确的HGE_VERSION,它是在hge.h中定义的 
	hge->System_SetState(HGE_SCREENWIDTH, 1024);
	hge->System_SetState(HGE_SCREENHEIGHT,768);
	hge->System_SetState(HGE_SHOWSPLASH,false);
	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);//设置逻辑函数为FrameFunc函数 
	hge->System_SetState(HGE_RENDERFUNC,RenderFunc);//设置绘制函数为RenderFunc函数
	hge->System_SetState(HGE_TITLE, "Fight For The Ancints"); 
	hge->System_SetState(HGE_WINDOWED,true);
	hge->System_SetState(HGE_USESOUND,true);
	hge->System_SetState(HGE_ZBUFFER,true);
	
	if(hge->System_Initiate())//用hge类的System_Initiate()方法，检测初始化是否有错误出现。 
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
		gui->AddCtrl(new hgeGUIMenuItem(1,sndmenu,576,250,0.0f,"新的游戏"));
		gui->AddCtrl(new hgeGUIMenuItem(2,sndmenu,576,350,0.5f,"特别鸣谢"));
		gui->AddCtrl(new hgeGUIMenuItem(3,sndmenu,576,450,1.0f,"退出游戏"));
		gui->SetNavMode(HGEGUI_UPDOWN | HGEGUI_CYCLED);
		gui->SetCursor(cursor);
		gui->SetFocus(1);
		gui->Enter();
		
		ft=new GfxFont("黑体",24);
		
		//mt=new MessageTip(hge);
		
		h=new Hero(hge);
		map=new Map(hge,"Graphics\\Tilesets\\043-Cave01.png",h->fighter);
		h->SetMap(map);
		map->LoadMap_s("map.txt");
		map->LoadEnemy("enemy.txt");
		bgm0channel=hge->Effect_Play(bgm0);
		par->Fire();
		par2->Fire();

		hge->System_Start();//如果没有问题，则使用System_Start方法，开始程序。 

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
	hge->System_Shutdown();//程序停止 
	hge->Release();//释放HGE所占用的内存。 
}