#pragma once

// 메크로
#define ClAM_WIDTH 1
#define CLAM_HEIGHT 1
#define CLAM_HP 1

// 전역변수
static const char* clamGraphic = "(\\  /)";

//조개 함수
void TriggerClam();
void UpdateClam();
void DrawClam();