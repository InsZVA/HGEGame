#include "Map.h"
extern unsigned int GAME_SCENE;
extern HEFFECT bgm1;
extern Enemy* e;
unsigned int Map::Pass(int dir,float x,float y,float buf)
{
	float px=x,py=y;
	for(int i=0;i<enemynum;i++)
	{
		if(enemyx[i]==(int)(px/32)&&enemyy[i]==(int)(py/32))
		{
			hge->Channel_Pause(bgmchannel);
			bgm1channel=hge->Effect_Play(bgm1);
			GAME_SCENE=3;
			e = enemy[i];
			break;
		}
	}
	//return ispass[0][0];
	switch(dir)
	{
	case 3://W
		mt->CheckMsg((int)(px/32),(int)((py-buf)/32));
		return (py-buf)>=0&&ispass[(int)(px/32)][(int)((py-buf)/32)];
	case 0://S
		mt->CheckMsg((int)(px/32),(int)((py+buf)/32));
		return (py+buf)<512&&ispass[(int)(px/32)][(int)((py+buf)/32)];
	case 1://A
		mt->CheckMsg((int)((px-buf)/32),(int)(py/32));
		return (px-buf)>=0&&ispass[(int)((px-buf)/32)][(int)(py/32)];
	case 2://D
		mt->CheckMsg((int)((px+buf)/32),(int)(py/32));
		return (px+buf)<1024&&ispass[(int)((px+buf)/32)][(int)(py/32)];
	}
	return 0;
}