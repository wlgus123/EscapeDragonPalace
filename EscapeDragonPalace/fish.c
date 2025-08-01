#include "monster.h"

char fishGraphic[2][FISH_HEIGHT][FISH_WIDTH] = {
    {
        "  _______   ",
        " /o　))　\\/)",
        " \\_______/\\)"
    }, // 왼쪽 그림 0
    {
        "    _______  ",
        " (\\/　((　o\\",
        " (/\\_______/ "
    },// 오른쪽 그림 1
};

void InitFish(Monster fish, int x, int y) {
    fish.x = x;
    fish.y = y;
    fish.dir = 1;
    fish.hp = FISH_HP;
    fish.alive = 1;
    fish.type = MONSTER_FISH;
    fish.isDamaged = 0;
    fish.lastHitTime = 0;  // 마지막 피격 시간 초기화
}

void UpdateFish(Monster fish, unsigned int now) {
    if (!fish.alive) return;

    // 무적시간 지나면 피격 상태 해제
    if (fish.isDamaged && now - fish.lastHitTime >= INVINCIBLE_TIME) {
        fish.isDamaged = 0;
    }

    // 이동
    fish.x += fish.dir;

    // 벽 충돌 시 방향 전환
    if (fish.x <= 0) {
        fish.x = 0;
        fish.dir = 1;
    }
    if (fish.x + FISH_WIDTH >= 82) {
        fish.x = 80 - FISH_WIDTH + 2;
        fish.dir = -1;
    }
}

void DrawFish(Monster fish) {
    for (int y = 0; y < FISH_HEIGHT; y++)
    {
        _SetColor(fish.isDamaged ? 12 : 9);

        int idx;

        if (fish.dir == 1){
            idx = Right;

            char* line = fishGraphic[idx][y];
            int len = strlen(line);

            for (int x = 0; x < len; x++)
            {
                if (line[x] != ' ')
                {
                    _DrawText(fish.x + x, fish.y + y, (char[]) { line[x], '\0' });
                }
            }
        }
        else{
            idx = Left;

            char* line = fishGraphic[idx][y];
            int len = strlen(line);

            for (int x = 0; x < len; x++)
            {
                if (line[x] != ' ')
                {
                    _DrawText(fish.x + x, fish.y + y, (char[]) { line[x], '\0' });
                }
            }          
        }

    }

    _SetColor(15);
}


// 몬스터 피격 함수 (체력 감소, 피격 상태 시작)
void HitFish(Monster fish, unsigned int now, int damage) {
    if (fish.isDamaged) return; // 무적중이면 데미지 무시

    fish.hp--;
    fish.isDamaged = 1;
    fish.lastHitTime = now;

    if (fish.hp <= 0) {
        fish.alive = 0; // 죽음 처리
    }
}

void UpdateMonster(Monster fish)
{
	unsigned int now = _GetTickCount();

	int damage = 1; // 무기 공격력 받아와야 함

	if (fish.alive) {
		UpdateFish(fish, now);
	}

    if (fish.isDamaged)
    {
        HitFish(fish, now, damage);
    }
}
