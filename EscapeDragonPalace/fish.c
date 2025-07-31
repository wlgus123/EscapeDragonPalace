#include "monster.h"
#include"mtg.h"


float Fspeed = 0.4;
int Fhp = 4;
int Fpoint = 1;
int Fpx,Fpy;
int Fcolor = 15;

char fish[2][3][16] = {
	{"  ______", //0
	 " / o )) \\/)",
	 " \\______/\\)"
	},
	{"   ______", //1
	 "(\\/ (( o \\",
	 "(/\\______/"
	}
};

void DrawFish() { //물고기 그리기

	for (int i = 0; i < 3; i++) {
		_DrawTextColor(Fpx, Fpy + i, fish[Fpoint][i], Fcolor);
	}
}

void Draw() {
	char temp[80];
	DrawFish();

	
	//test용
	sprintf(temp,"Hp : %d",Fhp);
	_DrawText(10, 10, temp);
	sprintf(temp, "point : %d", Fpoint);
	_DrawText(10, 11, temp);
	

}

void RunIni() {
	_Invalidate();
}


int RunKey() {
	char k;

	k = _GetKey();

}

void hit() { //맞음 > 색깔 붉은색 > 다시 흰색 > 죽으면 없어짐
	time_t start = time(NULL); // 타임함수를 이용
	time_t end = start + 1;
	Fhp--;
	Fcolor = 12;
	
	if (Fhp == 0) {
		Fcolor = 0;
	}

	_Invalidate();
	while(time(NULL) < end) {
		//대기
	}
	
	if (Fcolor == 12) {
		Fcolor = 15;
	}

	return start;
	
}
void wall(){ //벽이 존재 > 반대로
	switch (Fpoint) {
	case 0:
		Fpoint++;


		break;
	case 1:
		Fpoint--;


		break;
	}
}
//차후에 move랑 wall이랑 묶을듯(키보드 안받는다는 전제)
void move() {
	//wall();
	switch (Fpoint) {
	case 0:
		Fpx--;
		
		

		break;
	case 1:
		Fpx++;
		

		break;
	}

}

void RunTimer() {
	static long oldT = 0;
	long newT;

	newT = _GetTickCount();
	if (abs(newT - oldT) < 1000 * Fspeed) {
		return;
	}
	else
		oldT = newT;
	
	//반복한 것을 여기에


	move();

	_Invalidate();
}

void Fish() {
	RunIni();
	while (1) {

		if (Fhp == -1) { //임의
			break;
		}
		if ('d' == RunKey()) {
			hit();
		}

		if ('w' == RunKey()) {
			wall();
		}
		RunTimer();
		_Invalidate();

	}
	
	return 0;
}




int main() {
	_BeginWindow();
	Fish();
	_EndWindow();
}