#include "funkcije.h"


//globalne promenljive za animaciju propadanja igraca
int fail_condition = 0;
float fail_animation_parameter = 0;


//pozicija igraca, inicijalno na sredini prvog kvadrata
GLfloat position[] = {1.5,0.1,0.5};


//pocetni nivo igrice
int NIVO = 1;


//matrice za pozicije kvadrata kao i aktivnih kvadrata, po nivoima
//aktivni kvadrati su kvadrati gde igrac stoji na pocetku nivoa
//lvl1

int lvl1_kvadrati[2][3] = {{1,1,0},{1,1,1}};

int lvl1_aktivni[2][3] = {{0,0,0},{1,0,0}};
//lvl2
int lvl2_kvadrati[3][3] = {{1,1,1},{1,1,1},{1,1,1}};

int lvl2_aktivni[3][3] = {{1,0,0},{0,0,0},{0,0,0}};
//lvl3
int lvl3_kvadrati[2][6] = {{0,1,1,1,1,1},{1,1,1,1,1,1}};

int lvl3_aktivni[2][6] = {{0,0,0,0,0,0},{1,0,0,0,0,0}};
//lvl 4
int lvl4_kvadrati[3][5] = {{1,1,1,1,1},{0,1,1,1,0},{0,1,1,1,0}};

int lvl4_aktivni[3][5] = {{1,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}};

//lvl 5
int lvl5_kvadrati[5][5] = {{1,1,1,0,0},{1,0,1,0,0},{1,1,1,1,1},{0,0,1,0,1},{0,0,1,1,1}};

int lvl5_aktivni[5][5] = {{0,0,0,0,0},{0,0,0,0,0},{0,1,0,0,0},{0,0,0,0,0},{0,0,0,0,0}};

// lvl 6
int lvl6_kvadrati[5][5] = {{1,1,0,1,1},{1,1,1,1,1},{0,0,0,1,1},{0,0,0,1,0},{0,0,0,1,0}};

int lvl6_aktivni[5][5] = {{0,0,0,0,0},{1,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}};

// lvl 7

int lvl7_kvadrati[6][6] = {{0,0,1,1,0,0},{0,0,1,1,0,0},{1,1,1,1,1,1},{1,1,1,1,1,1},{0,0,1,1,0,0},{0,0,1,1,0,0}};

int lvl7_aktivni[6][6] = {{0,0,0,0,0,0},{0,0,0,0,0,0},{1,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0}};

//lvl 8

int lvl8_kvadrati[4][6] = {{1,1,1,0,0,0},{0,0,1,0,0,0},{0,0,1,1,1,0},{0,0,1,1,1,1}};

int lvl8_aktivni[4][6] = {{1,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0}};

//lvl 9
int lvl9_kvadrati[5][5] = {{1,1,1,1,1},{1,1,1,0,1},{1,1,1,1,1},{1,1,1,0,0},{1,1,1,0,0}};

int lvl9_aktivni[5][5] = {{1,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}};

//lvl 10

int lvl10_kvadrati[6][5] = {{0,0,1,1,1},{1,1,1,0,1},{1,0,1,1,1},{1,0,1,1,1},{1,1,1,0,1},{0,0,1,1,1}};

int lvl10_aktivni[6][5] = {{0,0,0,0,0},{1,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}};

//lvl 11

int lvl11_kvadrati[4][5] = {{0,1,1,1,0},{1,1,1,1,1},{1,0,1,0,1},{1,1,1,1,1}};

int lvl11_aktivni[4][5] = {{0,1,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}};

//dimenzije matrica nivoa
int nivo_dimenzije [11][2] = {{2,3},{3,3},{2,6},{3,5},{5,5},{5,5},{6,6},{4,6},{5,5},{6,5},{4,5}};

//C ce podesiti preostala polja matrice na nulu
//podesavamo za prvi nivo globalnu promenljivu

int curr_lvl_kv[7][7]={{1,1,1,0,0,0,0},{1,1,1,0,0,0,0}};

int curr_lvl_akt[7][7] ={{1,0,0,0,0,0,0}};




//pomocna funkcija za postavljanje boja
void postavi_boje(float red, float green, float blue, float transparency){

    	GLfloat ambient[] = {0.3,0.3,0.3,0};
	    GLfloat diffuse[] = {red,green,blue,transparency};
    	GLfloat specular[] = {0.6,0.6,0.6,0};
    	GLfloat shininess = 80;

    	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

}



//pomocna funkcija za iscrtavanje mreze kvadrata
void draw_grid(){
	
	glBegin(GL_LINES);
	for(int i=0;i<=10;i++) {
		if (i==0) { glColor3f(.6,.3,.3); } else { glColor3f(.25,.25,.25); };
		glVertex3f(i,0,0);
		glVertex3f(i,0,10);
		if (i==0) { glColor3f(.3,.3,.6); } else { glColor3f(.25,.25,.25); };
		glVertex3f(0,0,i);
		glVertex3f(10,0,i);
	};
	glEnd();	
	
	}






//podesavamo inicijalnu poziciju igraca,tj da li je na prvom, drugom , trecem kvadratu...

float player_position[10][1][2] = {
	
	{{0.5,0.5}}, //lvl2 position, jer smo vec postavili pozicije za prvi nivo
	{{1.5,0.5}},
	{{0.5,0.5}},
	{{2.5,1.5}},
	{{1.5,0.5}},
	{{2.5,0.5}},
	{{0.5,0.5}},
	{{0.5,0.5}},
	{{1.5,0.5}},
	{{0.5,1.5}}
	};


//provera da li je se kvadrat iscrtava
int provera(float x, float z){
	
	if(((position[0] <=x+0.5) && (position[0] >= x-0.5)) && ((position[2] <= z+0.5) && (position[2] >= z-0.5)))
		return 1;
		

	return 0;

	}


	
//pomocna funkcija za proveru da li je igrac na validnom delu mreze kvadrata
void fail_condition_check(int lvl) {
	
	int i=0,j=0;
	
	
	int mati=nivo_dimenzije[lvl-1][0];
	int matj=nivo_dimenzije[lvl-1][1];
	
	if(!(trunc(position[2])<matj && (trunc(position[0])>=0) && (trunc(position[0])<=mati-1)) )
				fail_condition=1;
	if(!(curr_lvl_kv[(int)trunc(position[0])][(int)trunc(position[2])]))
				fail_condition=1; 
	
	
	
	}
	
	
//broj kvadrata koji su preostali u nivou

int get_number_of_sq(int lvl) {
	
	
	int mati=nivo_dimenzije[lvl-1][0];
	int matj=nivo_dimenzije[lvl-1][1];
	
	
	int num=0;
	
	for(int i=0;i<mati;i++)
		for(int j=0;j<matj;j++)
			num += curr_lvl_kv[i][j]? 1: 0;
	
	return num;
	}

//postavljanje pozicije igraca

void set_player_position(int lvl) {
	
	position[0]= player_position[lvl-2][0][0];
	position[2]= player_position[lvl-2][0][1];
	
	}



//postavjavljamo sva polja pomocnih globalnih promenljivih na nule pre nego sto ih izmenimo
void set_zeroes(){
		for (int i=0;i<7;i++)
			for (int j=0;j<7;j++){
				curr_lvl_kv[i][j]=0;
				curr_lvl_akt[i][j]=0;
				}
	}

//postavljamo za trenutni nivo matricu kvadrata(kao i aktivnih kvadrata)
void set_current_level(int lvl){
		
	int i=0,j=0;
	
	
	int mati=nivo_dimenzije[lvl-1][0];
	int matj=nivo_dimenzije[lvl-1][1];
	
	for(i=0;i<mati;i++)
		for(j=0;j<matj;j++){
			switch(lvl){
			case 1: 
					curr_lvl_kv[i][j]=lvl1_kvadrati[i][j];
			        curr_lvl_akt[i][j]=lvl1_aktivni[i][j];
					break;
			case 2: 
					curr_lvl_kv[i][j]=lvl2_kvadrati[i][j];
			        curr_lvl_akt[i][j]=lvl2_aktivni[i][j];
					break;
			case 3: 
					curr_lvl_kv[i][j]=lvl3_kvadrati[i][j];
			        curr_lvl_akt[i][j]=lvl3_aktivni[i][j];
					break;
			case 4: 
					curr_lvl_kv[i][j]=lvl4_kvadrati[i][j];
			        curr_lvl_akt[i][j]=lvl4_aktivni[i][j];
					break;
			case 5: 
					curr_lvl_kv[i][j]=lvl5_kvadrati[i][j];
			        curr_lvl_akt[i][j]=lvl5_aktivni[i][j];
					break;
			case 6: 
					curr_lvl_kv[i][j]=lvl6_kvadrati[i][j];
			        curr_lvl_akt[i][j]=lvl6_aktivni[i][j];
					break;
			case 7: 
					curr_lvl_kv[i][j]=lvl7_kvadrati[i][j];
			        curr_lvl_akt[i][j]=lvl7_aktivni[i][j];
					break;
			case 8: 
					curr_lvl_kv[i][j]=lvl8_kvadrati[i][j];
			        curr_lvl_akt[i][j]=lvl8_aktivni[i][j];
					break;
			case 9: 
					curr_lvl_kv[i][j]=lvl9_kvadrati[i][j];
			        curr_lvl_akt[i][j]=lvl9_aktivni[i][j];
					break;
			case 10: 
					curr_lvl_kv[i][j]=lvl10_kvadrati[i][j];
			        curr_lvl_akt[i][j]=lvl10_aktivni[i][j];
					break;
			case 11: 
					curr_lvl_kv[i][j]=lvl11_kvadrati[i][j];
			        curr_lvl_akt[i][j]=lvl11_aktivni[i][j];
					break;
			}
		}
	
	
	}

//pomocna funkcija za ispisivanje nivoa

void print_level_info() {

	glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    
    gluOrtho2D(0.0, 800, 0.0, 800);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    //pozicija teksta
    glRasterPos2i(20, 780);
    char nivo_tekst[] = "NIVO : ";
	//trenutni nivo
	char ch = NIVO + '0';
	
	strncat(nivo_tekst, &ch, 1); 

    void * font = GLUT_BITMAP_HELVETICA_18;
    
    for (int i=0;i<strlen(nivo_tekst);i++)
    {
		char c = nivo_tekst[i];
		glColor3d(1.0, 0.0, 0.0);
		glutBitmapCharacter(font, c);
	}
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();	
	
	
	
	}
