#pragma once
#include <list>
#include "Unit.h"
#define ABS(x) ((x)>0?(x):(-(x)))
using namespace std;
class CollisionSystem
{
public:

	CollisionSystem(void)
	{
		
	}
	void AddUnit(Unit * u)
	{
		UnitList.push_back(u);
	}
	void Unique()
	{
		UnitList.sort();
		UnitList.unique();
	}
	bool IsCollision(Unit* u1,Unit* u2)
	{
		return ABS(u1->x-u2->x)<=(u1->w+u2->w)/2&&ABS(u1->y-u2->y)<=(u1->h+u2->h)/2;
	}
	~CollisionSystem(void)
	{
		UnitList.empty();
	}
private:
	list<Unit*> UnitList;
};

