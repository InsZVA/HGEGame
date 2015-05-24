#pragma once
#pragma warning(disable:4305)
#include "skillanimation.h"
#include "Fighter.h"
class SkillThunderAnim :
	public SkillAnimation
{
public:

	SkillThunderAnim(HGE* _hge):SkillAnimation(_hge)
	{
		tex=hge->Texture_Load("Graphics\\Animations\\017-Thunder01.png");
		snd=hge->Effect_Load("Audio\\SE\\124-Thunder02.ogg");
		ani=new hgeAnimation(tex,5,5,0,0,192,192);
		ani->SetSpeed(5);
		ani->SetZ(0.2);
		ani->SetHotSpot(96,192-26);
		ani->SetMode(HGEANIM_NOLOOP);
		ani->SetColor(ARGB(255,255,255,255));
	}

	~SkillThunderAnim(void)
	{
	}

};

class SkillThunder:
	public Skill
{
	SkillThunder(HGE* _hge):Skill(_hge)
	{
		ani=new SkillThunderAnim(_hge);
	}

	virtual ~SkillThunder()
	{
	}
};



