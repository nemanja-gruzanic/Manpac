#ifndef OBSTACLES_H
#define OBSTACLES_H
#include <time.h>
extern int z_obstacle[50];
extern double x_obstacle[50];
extern int r[50];


static void generate_positions() {
    srand(time(NULL));

    int i = 0;

    for (i = 0; i < 50; i++)
        r[i] = rand() % 3;
    
}

static void obstacles() {  
    int i = 0;   
    for(i = 0; i < 50; i++) {
    
        switch (r[i]) {
        case 0: x_obstacle[i] = -0.5; break;
        case 1: x_obstacle[i] = 0; break;
        case 2: x_obstacle[i] = 0.5;  break;
        }
    
        z_obstacle[i] = -2*i;
   
        glPushMatrix();
        glColor3f(1,0,0);
        glTranslatef(x_obstacle[i], 0.1, -2*i);
        glutSolidCube(0.2);
        glPopMatrix();
  }
}
#endif
