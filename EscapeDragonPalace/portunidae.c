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
		" (|)  (|) ",
		"  \\o_o/ ",
		" =(___)= "
	}
};

void DrawPorturn() {

    for (int i = 0; i < 3; i++) {
        _DrawTextColor(Ppx, Ppy+ i, portun[crush][i], Pcolor);
    }

}

void Draw() {
    DrawPorturn();
}

void RunIni() {
    _Invalidate();

}

int RunKey() {
    char k;
    k = _GetKey();
}

void hit() {
    time_t start = time(NULL); // 타임함수를 이용
    time_t end = start + 1;
    Php--;
    Pcolor = 12;

    if (Php == 0) {
        Pcolor = 0;
    }

    _Invalidate();
    while (time(NULL) < end) {
        //대기
    }

    if (Pcolor == 12) {
        Pcolor = 15;
    }

    return start;
}
void wall() { //벽이 존재 > 반대로
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


void P_RunTimer() {

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

int Portun() {
    RunIni();
    while (1) {
        srand(time(NULL));
        crush = rand() % 2; //꽃게가 랜덤으로 잡았다 피게

        //키는 시험용
        if ('h'==RunKey()) {
            hit();
        }
        if ('w' == RunKey()) {
            wall();
        }

        P_RunTimer();
        _Invalidate();
    }
}


int main() {
    _BeginWindow();
    Portun();
    _EndWindow();
}