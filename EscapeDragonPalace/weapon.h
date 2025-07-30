#pragma once

#define NUMWEAPON 3
#define WEAPON_SPRITE_LENGTH  21

typedef struct Weapon {
    char name[30];
    char sprite[WEAPON_SPRITE_LENGTH];
    int attack;    // 공격력
    int attackSpeed;    // 공격속도
} Weapon;

typedef struct WeaponInstance {
    Weapon* weapon;// 무기 종류
    int x, y;      // 맵 위치
    int isHeld;    // 0: 바닥에 있음, 1: 플레이어가 소지
} WeaponInstance;

Weapon weaponList[NUMWEAPON];
char buffer[50];

extern void InitWeapon(Weapon* weapons);
extern void DrawWeapon(const Weapon* w, int i, int isSelected);
extern void SelectWeapon();