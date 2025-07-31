#include "monster.h"


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

void DrawFish() { //물고기 그리는 함수

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


void hit() { //맞음 > 색깔 붉은색(12) > 다시 흰색(15) > 죽으면 없어짐
	time_t start = time(NULL); // 타임함수를 이용
	time_t end = start + 1;
	Fhp--;
	Fcolor = 12;
	
	if (Fhp == 0) {
		Fcolor = 0; //죽은 판정으로 색을 완전히 없애는 코드
	}

	_Invalidate();
	while(time(NULL) < end) {
		//1초간 대기 후
	}
	
	if (Fcolor == 12) {
		Fcolor = 15;
	}
}

void wall() { //벽이 존재 > 반대로
	switch (Fpoint) {
	case 0:
		Fpoint++;


		break;
	case 1:
		Fpoint--;


		break;
	}
}

int RunKey() {
	char k;

	k = _GetKey();


	if ('h' == k) {
		hit();
	}

	if ('w' == k) {
		wall();
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

void RunTimer() { //움직임 반복되는 코드
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

int Fish() {

	RunKey();
	RunTimer();

	return 0;
}




int main() {
	_BeginWindow();
	while (1)
	{
		Fish();

		_Invalidate();
	}
	_EndWindow();
}