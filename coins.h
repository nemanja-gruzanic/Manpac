#ifndef COINS_H
#define COINS_H
#include <time.h>
#include "obstacles.h"
extern int z_coin[50];
extern double x_coin[50];
extern int first_game;


static void coins() {  
    int i = 0;   
    for(i = 0; i < 50; i++) {
    
        z_coin[i] = -2 * i + 1;
      
        if (first_game) {
            switch (r[i]) {
            case 0: x_coin[i] = -0.5; break;
            case 1: x_coin[i] = 0; break;
            case 2: x_coin[i] = 0.5;  break;
            }
        }
   
        glPushMatrix();
        glColor3f(0,1,0);
        glTranslatef(x_coin[i], 0.1, -2*i+1);
        glutSolidSphere(0.05,40,40);
        glPopMatrix();
  }
       
  
}
#endif
