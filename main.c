#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "obstacles.h"
#include "path.h"
#include "ball.h"
#include "lighting.h"
#include "coins.h"

#define MOVEMENT_ALONG_Z 0
#define MOVEMENT_ALONG_X 1
#define TIMER_INTERVAL 120

/* global series of random numbers for create obstacles */
int r[50];
int z_obstacle[50];
double x_obstacle[50];
  
int z_coin[50];
double x_coin[50];
int collected[50];
/* variable that makes the coin disappear during collection */
int first_game=1;

static int counter;
static float speed_up;
static double x_curr, z_curr;
/*border of movement along x axis */
static double x_border = 0;

static double x_parametar = 0.0;

static int animation_ongoing;
/* variable that controls the animation along the x axis */
static int animation1_ongoing;

static void end_game();
static void collecting_coins();
static void victory();

static void coins();
static void path();
static void ball();
static void obstacles();
static void lighting();

void on_display ();
void on_reshape(int width,int height);
void on_keyboard(unsigned char key, int x, int y);
static void on_timer(int value);

int main(int argc, char **argv)
{
    /* GLUT initialize */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    
    /* create window */
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("manpac");

    /* Register callback functions */
    glutDisplayFunc(on_display);
    glutReshapeFunc(on_reshape);
    glutKeyboardFunc(on_keyboard);
    
    /* include z buffer */
    glEnable(GL_DEPTH_TEST);
    
    generate_positions();
    /* initial position of the main object and variable of speed*/
    int i;
    speed_up = 1;
    x_curr = 0.0;
    z_curr = 4;
    animation_ongoing = 0;
    /* counter is variable for record how many points we have accumulated */
    counter = 0;
    
    for(i = 0; i < 50; i++)
      collected[i] = 0;
    
    /* set lighting */
    lighting();

    
    /* specify clear values ​​for the color buffers, RGB */
    glClearColor(0.75, 0.75, 0.75, 0.0);
    
    
    /*  glutMainLoop enters the GLUT event processing loop.  */
    glutMainLoop();

    return 0;
}


void on_display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
   /* setting view  */
    gluLookAt(
    0, 2, 4 + z_curr,
    0, 0, z_curr,
    0, 1, 0);
   
  /* when main object hit an obstacle, then return to the start of the path */
    end_game(); 
   
  /* plotting path, ball, obstacles and coins */
    path();
    ball();
    obstacles();
    coins();
  
  /* when player reaches a certain limit without hitting an obstacle, the player wins */
    victory();
  
  /* register collected points*/
    collecting_coins();
   
    glutSwapBuffers();
}



void on_keyboard(unsigned char key, int x, int y){
    switch(key){
        /* moving left */
        case 'A':
        case 'a':
        if(x_border == -0.5)
            x_border = -0.5;
        else
            x_border -= 0.5;
      
        animation1_ongoing = 1;
        glutTimerFunc(TIMER_INTERVAL, on_timer, MOVEMENT_ALONG_X);
    
        if (!animation_ongoing && z_curr > -80) {
            glutTimerFunc(TIMER_INTERVAL, on_timer, MOVEMENT_ALONG_Z);
            animation_ongoing = 1;
        }
        glutPostRedisplay();
        break;
    /* moving right */
        case 'D':
        case 'd':
        if (x_border == 0.5)
            x_border = 0.5;
      
        else{
            x_border += 0.5;
        }
      
        animation1_ongoing = 1;
        glutTimerFunc(TIMER_INTERVAL, on_timer, MOVEMENT_ALONG_X);
      
        if (!animation_ongoing && z_curr > -80) {
            glutTimerFunc(TIMER_INTERVAL, on_timer, MOVEMENT_ALONG_Z);
            animation_ongoing = 1;
        }
        
        glutPostRedisplay();
        break;
   
    /* esc */
        case 27:
        exit(0);
    }
}

void on_reshape(int width, int height)
{
    /* set viewport */
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30, (float) width / height, 1, 10);
}

static void on_timer(int value)
{
    
    
    if (value == MOVEMENT_ALONG_Z) {
        /* update coordinates of object and speed*/
        speed_up += 0.05;
        z_curr -= 0.05*speed_up;
        
        /* sets the limit to which the main object can go */
        if(z_curr < -80 )
            animation_ongoing = 0;
        
        /* redrawing windows */
        glutPostRedisplay();

        /* if necessary, reset the timer */
        if (animation_ongoing) {
            glutTimerFunc(TIMER_INTERVAL, on_timer, MOVEMENT_ALONG_Z);
        }
    }
    
    if (value == MOVEMENT_ALONG_X) {
        if (x_border == -0.5 && x_curr == -0.5)
            animation1_ongoing = 0;
      
        else if (fabs(x_border - x_curr) < 0.0001)
            animation1_ongoing = 0;
      
        else {
            if (x_border == 0 && x_curr > 0)
                x_parametar = -0.1;
      
            else if (x_border == 0 && x_curr < 0)
                x_parametar = 0.1;
      
            else if (x_border > 0)
                x_parametar = 0.1;
      
            else if (x_border < 0)
                x_parametar = -0.1;
      
            x_curr += x_parametar;
    
        }
    
    glutPostRedisplay();
    
    if (animation1_ongoing)
        glutTimerFunc(TIMER_INTERVAL, on_timer, MOVEMENT_ALONG_X);
    }
}

static void end_game(){
    int i;
    double distance;
    for( i = 0; i < 50; i++){
    /* distance of the main object from obstacles*/
        distance = fabs(z_curr - z_obstacle[i]);
        double razlika = fabs(x_curr - x_obstacle[i]);
    
        if(distance < 0.2 && razlika < 0.001){
            glutTimerFunc(TIMER_INTERVAL, on_timer, MOVEMENT_ALONG_Z); 
            animation_ongoing = 0;
      /* when hitting an obstacle, the main object is set to the beginning of the course and reset speed*/
            x_curr = 0;
            z_curr = 4;
            speed_up = 1;
            first_game = 1;
            printf("your points: %d \n", counter);
            counter = 0;
            for(i = 0; i < 50; i++)
                collected[i] = 0;
        }
    }
}

static void victory(){
    if(z_curr < -80){
        animation_ongoing = 0;
        printf("victory \n");
        printf("your points: %d \n", counter);
    }
}

static void collecting_coins(){
    int i;
  /* distance of the main object from coins*/
    double distance;
    first_game = 0; 
    for(i = 0; i < 50; i++){
        distance = fabs(z_curr - z_coin[i]);
        double razlika = fabs(x_curr - x_coin[i]);
    
        if(distance < 0.2 && razlika < 0.0001 && collected[i]==0){
            collected[i] = 1;
            counter++;
            /* change x coordinate of coin and he disappear*/
            x_coin[i] = 10;
      
        }
    }
}
