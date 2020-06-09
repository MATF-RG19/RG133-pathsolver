#ifndef FUNKCIJE_H
#define FUNKCIJE_H

#include <GL/glut.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

//globalne promenljive


extern int fail_condition;
extern float fail_animation_parameter;

extern GLfloat position[3];
extern int NIVO;

extern int lvl1_kvadrati[2][3];
extern int lvl1_aktivni[2][3];

extern int lvl2_kvadrati[3][3];
extern int lvl2_aktivni[3][3];

extern int lvl3_kvadrati[2][6];
extern int lvl3_aktivni[2][6];

extern int lvl4_kvadrati[3][5];
extern int lvl4_aktivni[3][5];

extern int lvl5_kvadrati[5][5];
extern int lvl5_aktivni[5][5];

extern int lvl6_kvadrati[5][5];
extern int lvl6_aktivni[5][5];

extern int lvl7_kvadrati[6][6];
extern int lvl7_aktivni[6][6];

extern int lvl8_kvadrati[4][6];
extern int lvl8_aktivni[4][6];

extern int lvl9_kvadrati[5][5];
extern int lvl9_aktivni[5][5];

extern int lvl10_kvadrati[6][5];
extern int lvl10_aktivni[6][5];


extern int nivo_dimenzije [10][2];

extern float player_position[9][1][2];


//funkcije


void draw_square2(float x_coord, float z_coord);

void draw_grid();

void postavi_nivo(int lvl);

void postavi_boje(float red, float green, float blue, float transparency);

int provera(float x, float z);

void fail_condition_check(int lvl);

void set_player_position(int lvl);

int get_number_of_sq(int lvl);

void print_level_info();






#endif
