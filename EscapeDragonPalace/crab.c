#include "monster.h"

float Pspeed = 0.2;
int Php = 5;
int Ppoint = 0;
int Ppx,Ppy;
int Pcolor = 15;
int crush = 0;


char portun[2][3][16] = {
	{
		"(\\/) (\\/) ",
		"  \\o_o/ ",
		" =(___)= "
	},
	{
		"(\\/) (\\/) ",
		" \\o_o/ ",
		" =(___)= "
	}
};

void DrawPorturn() { //꽃게 그리는 함수
    for (int i = 0; i < 3; i++) {
        _DrawTextColor(Ppx, Ppy+ i, portun[crush][i], Pcolor);
    }

}

void Draw() {
    DrawPorturn();
}

void hit() {
    time_t start = time(NULL); // 맞으면 피 내려가는 코드 및 색깔이 바뀌었다가 다시 돌아오는 코드
    time_t end = start + 1;

    Php--;

    Pcolor = 12;//붉은색으로 변경

    if (Php == 0) { //죽을경우
        Pcolor = 0;
    }

    _Invalidate();
    while (time(NULL) < end) {
        //1초간 대기
    }

    if (Pcolor == 12) { // 흰색으로 다시 돌아오게
        Pcolor = 15;
    }
}

void wall() { //벽이 존재 > 반대로 움직이게
    switch (Ppoint) {
    case 0:
        Ppoint++;


        break;
    case 1:
        Ppoint--;


        break;
    }
}

void move() {
    

    switch (Ppoint) {
    case 0:
        
        Ppx++;

        break;
    case 1:
        
        Ppx--;

        break;
    }
}


void P_RunTimer() { //반복적으로 키를 안받고 움직이도록

    static long oldT = 0;
    long newT;
    

    newT = _GetTickCount();
    if (abs(newT - oldT) < 1000*Pspeed)
    {
        return;
    }
    else
    {
        oldT = newT;
    }
    //=======

    move();
    _Invalidate();
}

int RunKey() {
    char k;
    k = _GetKey();


    //키는 시험용
    if ('h' == k) {
        hit();
    }
    if ('w' == k) {
        wall();
    }
}

int Portun() {
    srand(time(NULL));
    crush = rand() % 2; //꽃게가 랜덤으로 잡았다 피게

    RunKey();

    P_RunTimer();
}


int main() {
    _BeginWindow();

    while (1) 
    {
        Portun();
		_Invalidate();
    }

    _EndWindow();
}