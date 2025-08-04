#include "init.h"
#include "monster.h"

// 몬스터 업데이트
void UpdateMonster()
{
	unsigned int now = _GetTickCount();
	UpdateCrab(now);
	UpdateClam();
	int damage = 1; // 무기 공격력 받아와야 함

	if (GetFishAlive()) {
		UpdateFish(now);
	}

	if (GetFishIsDamaged())
	{
		HitFish(now, damage);
	}
}