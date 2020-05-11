#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

#define TIMER_INTERVAL 20
#define TIMER_ID 0
#define PI 3.14159265359

static void on_display();
static void on_reshape(int width, int height);
static void on_keyboard(unsigned char key, int x, int y);
static void on_keyboard2(int key, int x, int y);
static void on_timer(int id);

static void draw_square(float x_coord, float z_coord);
static void draw_square2(float x_coord, float z_coord);
static void draw_grid();

//globalne promenljive za animaciju
float animation_parameter = 0;
int animation_ongoing = 0;


//pozicija igraca, inicijalno na sredini prvog kvadrata
GLfloat position[] = {1.5,0.1,0.5};

//smer kretanja igraca koji je u pocetku u smeru z ose
int direction  = 2;
//da li je pozitivan smer, u pocetku jeste pa je jednak 1
int positive_dir = 1;

//brzina kretanja lopte
float vel_incr = 0.02;


int NIVO = 1;

//matrice za pozicije kvadrata kao i aktivnih kvadrata, po nivoima
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

//provera da li je se kvadrat iscrtava
int provera(float x, float z){
	
	if(((position[0] <=x+0.5) && (position[0] >= x-0.5)) && ((position[2] <= z+0.5) && (position[2] >= z-0.5)))
		return 1;
		

	return 0;
	
	
	}


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





int main(int argc, char **argv)
{
    /* Inicijalizuje se GLUT. */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    /* Kreira se prozor. */
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    /* Registruju se callback funkcije */
    glutDisplayFunc(on_display);
    glutReshapeFunc(on_reshape);
    glutKeyboardFunc(on_keyboard);
    glutSpecialFunc(on_keyboard2);

    /* Obavlja se OpenGL inicijalizacija. */
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
	//postavlja se osvetljenje
    float light_position[] = {-1, 1, 1, 0};
    float light_ambient[] = {.3f, .3f, .3f, 1};
    float light_diffuse[] = {.7f, .7f, .7f, 1};
    float light_specular[] = {.7f, .7f, .7f, 1};

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

	glClearColor(0.1,1,1,1);
	glEnable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);
	//pozivamo jer obrcemo smer x ose
	glFrontFace(GL_CW);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);
    glutMainLoop();

  return 0;
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

//Pomocna funkcija za iscrtavanje kvadrata
void draw_square(float x_coord, float z_coord) {
	glPushMatrix();	
    glBegin(GL_QUADS);
    glVertex3f(x_coord-0.5f, 0.0f, z_coord-0.5f);  
    glVertex3f(x_coord-0.5f, 0.0f, z_coord+0.5f); 
    glVertex3f(x_coord+0.5f, 0.0f, z_coord+0.5f); 
    glVertex3f(x_coord+0.5f, 0.0f, z_coord-0.5f); 
    glEnd();	
	glPopMatrix();
	}

void draw_square2(float x_coord, float z_coord){
	glPushMatrix();
	glTranslatef(x_coord,0,z_coord);
	glScalef(1,0.1,1);
	glutSolidCube(1);
	glPopMatrix();
	}



//funkcije za kontrolu kretanja i pokretanje igrice

void on_keyboard(unsigned char key, int x, int y) {
    switch(key) {
        case 'g':
        case 'G':
		if(!animation_ongoing){
			animation_ongoing=1;
			glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
		}
            break;
        case 27:
          exit(0);
          break;
          
        glutPostRedisplay();
    }
}


void on_keyboard2(int key, int x, int y){
	switch(key){
	case GLUT_KEY_UP :
			if(animation_ongoing){
				positive_dir=1;
				direction=2;
				animation_parameter= 0;
				glutPostRedisplay();
			}

		break;
	case GLUT_KEY_RIGHT :
			if(animation_ongoing){
				positive_dir=1;
				direction=0;
				animation_parameter= 0;
				glutPostRedisplay();
			}
	    break;
	case GLUT_KEY_DOWN :
			if(animation_ongoing){
				positive_dir=0;
				direction=2;
				animation_parameter= 0;
				glutPostRedisplay();
			}
	    break;

	case GLUT_KEY_LEFT :
			if(animation_ongoing){
				positive_dir=0;
				direction=0;
				animation_parameter= 0;
				glutPostRedisplay();
			}
			
	    break;	
	
		}
	}



void on_timer(int id) {
    if (id == TIMER_ID) {

		animation_parameter+=vel_incr;

    }

    glutPostRedisplay();

  
	if (animation_ongoing){
		glutTimerFunc(TIMER_INTERVAL,on_timer, TIMER_ID);
	}

}

void on_reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(30, (float) width/height, 1, 1000);
}

//funkcija za iscrtavanje

void on_display() {
  
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //obrcemo x osu
    //https://stackoverflow.com/questions/28632150/opengl-how-to-invert-x-axis
    glScalef (-1.0, 1.0, 1.0);
    
    
    //kamera se pomera sa igracem
    gluLookAt(position[0], position[1]+7, position[2]-10,
              position[0], position[1], position[2],
              0, 1, 0);

	draw_grid();

   	         
	glPushMatrix();

	int i=0,j=0;
	
	//iscrtavanje kvadrata po nivoima
	switch(NIVO) {
		case 1 :
		while(i<2){
			j=0;
			while(j<3){
			//ako nije obrisan, iscrtavamo kvadrat
				if(lvl1_kvadrati[i][j]){
				//provera da li je igrac i dalje na kvadratu ako nije onda ga brisemo, takodje proveravamo da li je igrac presao na neki drugi kvadrat
					if(lvl1_aktivni[i][j]){
						if(provera(i+0.5,j+0.5)){
							postavi_boje(1.0,0.4,0.3,0.9);  
							draw_square2(0.5+i,0.5+j);
						} else {
							//num_of_squares -= 1;
							lvl1_kvadrati[i][j]=0;
							lvl1_aktivni[i][j]=0;
							}
						} else {
							if(provera(i+0.5,j+0.5))
								lvl1_aktivni[i][j]=1;
							postavi_boje(0.9,0.3,0.3,0.9);  
							draw_square2(0.5+i,0.5+j);
							}
					}
				j+=1;
				}
			i+=1;
		}
		break;
		case 2 :
		i=0,j=0;
		while(i<3) {
			j=0;
			while(j<3){
			//ako nije obrisan, iscrtavamo kvadrat
				if(lvl2_kvadrati[i][j]){
				//provera da li je igrac i dalje na kvadratu ako nije onda ga brisemo, takodje proveravamo da li je igrac presao na neki drugi kvadrat
					if(lvl2_aktivni[i][j]){
						if(provera(i+0.5,j+0.5)){
							postavi_boje(1.0,0.4,0.3,0.9);  
							draw_square2(0.5+i,0.5+j);
						} else {
							//num_of_squares -= 1;
							lvl2_kvadrati[i][j]=0;
							lvl2_aktivni[i][j]=0;
							}
						} else {
							if(provera(i+0.5,j+0.5))
								lvl2_aktivni[i][j]=1;
							postavi_boje(0.9,0.3,0.3,0.9);  
							draw_square2(0.5+i,0.5+j);
							}
					}
				j+=1;
				}
			i+=1;
		}
		break;
		case 3:
		i=0;j=0;
		while(i<2) {
			j=0;
			while(j<6){
			//ako nije obrisan, iscrtavamo kvadrat
				if(lvl3_kvadrati[i][j]){
				//provera da li je igrac i dalje na kvadratu ako nije onda ga brisemo, takodje proveravamo da li je igrac presao na neki drugi kvadrat
					if(lvl3_aktivni[i][j]){
						if(provera(i+0.5,j+0.5)){
							postavi_boje(1.0,0.4,0.3,0.9);  
							draw_square2(0.5+i,0.5+j);
						} else {
							//num_of_squares -= 1;
							lvl3_kvadrati[i][j]=0;
							lvl3_aktivni[i][j]=0;
							}
						} else {
							if(provera(i+0.5,j+0.5))
								lvl3_aktivni[i][j]=1;
							postavi_boje(0.9,0.3,0.3,0.9);  
							draw_square2(0.5+i,0.5+j);
							}
					}
				j+=1;
				}
			i+=1;
		}
		break;
		case 4:
		i=0;j=0;
		while(i<3) {
			j=0;
			while(j<5){
			//ako nije obrisan, iscrtavamo kvadrat
				if(lvl4_kvadrati[i][j]){
				//provera da li je igrac i dalje na kvadratu ako nije onda ga brisemo, takodje proveravamo da li je igrac presao na neki drugi kvadrat
					if(lvl4_aktivni[i][j]){
						if(provera(i+0.5,j+0.5)){
							postavi_boje(1.0,0.4,0.3,0.9);  
							draw_square2(0.5+i,0.5+j);
						} else {
							//num_of_squares -= 1;
							lvl4_kvadrati[i][j]=0;
							lvl4_aktivni[i][j]=0;
							}
						} else {
							if(provera(i+0.5,j+0.5))
								lvl4_aktivni[i][j]=1;
							postavi_boje(0.9,0.3,0.3,0.9);  
							draw_square2(0.5+i,0.5+j);
							}
					}
				j+=1;
				}
			i+=1;
		}
		break;	
		case 5:
		i=0;j=0;
		while(i<5) {
			j=0;
			while(j<5){
			//ako nije obrisan, iscrtavamo kvadrat
				if(lvl5_kvadrati[i][j]){
				//provera da li je igrac i dalje na kvadratu ako nije onda ga brisemo, takodje proveravamo da li je igrac presao na neki drugi kvadrat
					if(lvl5_aktivni[i][j]){
						if(provera(i+0.5,j+0.5)){
							postavi_boje(1.0,0.4,0.3,0.9);  
							draw_square2(0.5+i,0.5+j);
						} else {
							//num_of_squares -= 1;
							lvl5_kvadrati[i][j]=0;
							lvl5_aktivni[i][j]=0;
							}
						} else {
							if(provera(i+0.5,j+0.5))
								lvl5_aktivni[i][j]=1;
							postavi_boje(0.9,0.3,0.3,0.9);  
							draw_square2(0.5+i,0.5+j);
							}
					}
				j+=1;
				}
			i+=1;
		}
		break;	
		case 6:
		i=0;j=0;
		while(i<5) {
			j=0;
			while(j<5){
			//ako nije obrisan, iscrtavamo kvadrat
				if(lvl6_kvadrati[i][j]){
				//provera da li je igrac i dalje na kvadratu ako nije onda ga brisemo, takodje proveravamo da li je igrac presao na neki drugi kvadrat
					if(lvl6_aktivni[i][j]){
						if(provera(i+0.5,j+0.5)){
							postavi_boje(1.0,0.4,0.3,0.9);  
							draw_square2(0.5+i,0.5+j);
						} else {
							//num_of_squares -= 1;
							lvl6_kvadrati[i][j]=0;
							lvl6_aktivni[i][j]=0;
							}
						} else {
							if(provera(i+0.5,j+0.5))
								lvl6_aktivni[i][j]=1;
							postavi_boje(0.9,0.3,0.3,0.9);  
							draw_square2(0.5+i,0.5+j);
							}
					}
				j+=1;
				}
			i+=1;
		}
		break;
		case 7:
		i=0;j=0;
		while(i<6) {
			j=0;
			while(j<6){
			//ako nije obrisan, iscrtavamo kvadrat
				if(lvl7_kvadrati[i][j]){
				//provera da li je igrac i dalje na kvadratu ako nije onda ga brisemo, takodje proveravamo da li je igrac presao na neki drugi kvadrat
					if(lvl7_aktivni[i][j]){
						if(provera(i+0.5,j+0.5)){
							postavi_boje(1.0,0.4,0.3,0.9);  
							draw_square2(0.5+i,0.5+j);
						} else {
							//num_of_squares -= 1;
							lvl7_kvadrati[i][j]=0;
							lvl7_aktivni[i][j]=0;
							}
						} else {
							if(provera(i+0.5,j+0.5))
								lvl7_aktivni[i][j]=1;
							postavi_boje(0.9,0.3,0.3,0.9);  
							draw_square2(0.5+i,0.5+j);
							}
					}
				j+=1;
				}
			i+=1;
		}
		break;		
	}
	
	
    glPopMatrix(); 
    
    
    glPushMatrix();
    glPushMatrix();

    postavi_boje(0.3,0.3,0.9,1.0);
    
    switch(NIVO) {
    //pre iscrtavanja lopte tj. igraca provera da li smo na validnom delu mreze kvadrata
    case 1:
		if(!(trunc(position[0])<2 &&(trunc(position[0])>=0)))
			exit(EXIT_FAILURE);  
  
    //TODO umesto prekidanja programa, napraviti animaciju propadanja lopte , pa zatvoriti program
		if(!(lvl1_kvadrati[(int)trunc(position[0])][(int)trunc(position[2])]))
			exit(EXIT_FAILURE); 
		break;
	case 2:
		if(!(trunc(position[0])<3 &&(trunc(position[0])>=0))) 
			exit(EXIT_FAILURE);
		if(!(lvl2_kvadrati[(int)trunc(position[0])][(int)trunc(position[2])]))
			exit(EXIT_FAILURE); 
		break;	
	case 3:
		if(!(trunc(position[2])<6 && (trunc(position[0])>=0)))
			exit(EXIT_FAILURE);
		if(!(lvl3_kvadrati[(int)trunc(position[0])][(int)trunc(position[2])]))
			exit(EXIT_FAILURE); 
		break;
	case 4:
		if(!(trunc(position[2])<5 && (trunc(position[0])>=0)))
			exit(EXIT_FAILURE);
		if(!(lvl4_kvadrati[(int)trunc(position[0])][(int)trunc(position[2])]))
			exit(EXIT_FAILURE); 
		break;	
	case 5:
		if(!(trunc(position[2])<5 && (trunc(position[0])>=0)))
			exit(EXIT_FAILURE);
		if(!(lvl5_kvadrati[(int)trunc(position[0])][(int)trunc(position[2])]))
			exit(EXIT_FAILURE); 
		break;			
	case 6:
		if(!(trunc(position[2])<5 && (trunc(position[0])>=0)))
			exit(EXIT_FAILURE);
		if(!(lvl6_kvadrati[(int)trunc(position[0])][(int)trunc(position[2])]))
			exit(EXIT_FAILURE); 
		break;
	case 7:
		if(!(trunc(position[2])<6 && (trunc(position[0])>=0)))
			exit(EXIT_FAILURE);
		if(!(lvl7_kvadrati[(int)trunc(position[0])][(int)trunc(position[2])]))
			exit(EXIT_FAILURE); 
		break;
	}	
	
	//ako animacija traje pomeramo igraca
	if(animation_ongoing){	
		if(positive_dir)    
			position[direction] += vel_incr;
		else
			position[direction] -= vel_incr;
		}
   
	//pomeramo igraca
    glTranslatef(position[0],position[1],position[2]);		
    //lopta koja se krece po kvadratima
   
    glutSolidSphere(0.1,64,64);
	//proveravamo koliko nam je ostalo kvadrata na nivou
	int num_of_sq = 0;
	switch(NIVO){
		case 1:
			for(i=0;i<2;i++)
				for(j=0;j<3;j++)
					num_of_sq += lvl1_kvadrati[i][j]? 1: 0;
			break;
		case 2:
			for(i=0;i<3;i++)
				for(j=0;j<3;j++)
					num_of_sq += lvl2_kvadrati[i][j]? 1: 0;
			break;
		case 3:
			for(i=0;i<2;i++)
				for(j=0;j<6;j++)
					num_of_sq += lvl3_kvadrati[i][j]? 1: 0;
			break;
		case 4:
			for(i=0;i<3;i++)
				for(j=0;j<5;j++)
					num_of_sq += lvl4_kvadrati[i][j]? 1: 0;
			break;
		case 5:
			for(i=0;i<5;i++)
				for(j=0;j<5;j++)
					num_of_sq += lvl5_kvadrati[i][j]? 1: 0;
			break;
		case 6:
			for(i=0;i<5;i++)
				for(j=0;j<5;j++)
					num_of_sq += lvl6_kvadrati[i][j]? 1: 0;
			break;
		case 7:
			for(i=0;i<6;i++)
				for(j=0;j<6;j++)
					num_of_sq += lvl7_kvadrati[i][j]? 1: 0;
			break;	
		}
    //TODO dodati animaciju slavalja pri prelasku sa nivoa na nivo
    //ako smo ostali sa samo jednim kvadratom onda prelazimo na sledeci nivo
	if(num_of_sq==1){
		if(NIVO==7)
			exit(EXIT_SUCCESS);
		NIVO+=1;
		switch (NIVO){
			case 2:
				position[0] = 0.5;
				position[1] = 0.1;
				position[2] = 0.5;
			break;
			case 3:
				position[0] = 1.5;
				position[1] = 0.1;
				position[2] = 0.5;
			break;
			case 4:
				position[0] = 0.5;
				position[1] = 0.1;
				position[2] = 0.5;
			break;
			case 5:
				position[0] = 2.5;
				position[1] = 0.1;
				position[2] = 1.5;
			break;
			case 6:
				position[0] = 1.5;
				position[1] = 0.1;
				position[2] = 0.5;
			break;
			case 7:
				position[0] = 2.5;
				position[1] = 0.1;
				position[2] = 0.5;
			break;							
		}
	}

    glPopMatrix();
	glPopMatrix();
 

    
    glutSwapBuffers();
}
