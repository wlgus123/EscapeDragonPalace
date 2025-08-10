#pragma once

#define NUMWEAPON 3 // 무기 종류 갯수
#define WEAPON_SPRITE_LENGTH  21 // 무기 스프라이트 길이

// 무기 구조체
typedef struct Weapon {
	char name[30];                      // 무기 이름
	char sprite[WEAPON_SPRITE_LENGTH];  // 무기 스프라이트
    int attack;                         // 공격력
    int attackSpeed;                    // 공격속도
} Weapon;

Weapon weaponList[NUMWEAPON];   // 무기 리스트
char buffer[50];				// 무기 정보 출력용 버퍼

// 함수
void SelectWeapon();		// 무기 선택 함수
int GetSelectedIndex();		// 선택된 무기 인덱스 반환 함수
bool GetWeaponChosen();		// 무기 선택 여부 반환 함수
void SetWeaponChosen(bool src);		// 무기 선택 여부 설정 함수
void InitWeapon(Weapon* weapons);	// 무기 초기화 함수
void DrawWeapon(const Weapon* w, int i, int isSelected); // 무기 정보 출력 함수
