#pragma once

// 메크로
#define CLAM_WIDTH 1
#define CLAM_HEIGHT 7
#define CLAM_HP 1

// 전역변수
static const char* clamGraphic = "(\\  /)";

//조개 함수
void TriggerClam();
void UpdateClam(MyPoint pos, bool alive);
void DrawClam(MyPoint pos);
MyPoint GetClamPos();
int GetClamDir();