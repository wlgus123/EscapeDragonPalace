#pragma once

#define CRAB_WIDTH 13
#define CRAB_HEIGHT 3
#define CRAB_HP 5

// Àü¿ª º¯¼ö
const static char crabGraphic[2][CRAB_HEIGHT][CRAB_WIDTH] = {
    {   
        "(\\/) (\\/)",
         "  \\o_o/ ",
         " =(___)= "
    }, //ºñÆø·Â

    {
        "(l)  (l)",
         " \\o_o/ ",
         "=(___)= " //BeÆø·Â1
    }
};


// ²É°Ô ÇÔ¼ö
void UpdateCrab(unsigned int now);
void DrawCrab(int posX, int posY);
void HitCrab(unsigned int now, int damage);
int GetCrabDir();
MyPoint GetCrabPos();
void CrabHitP(int posX, int posY);