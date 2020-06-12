#include <GL/glut.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "funkcije.h"
#include "image.h"

#define TIMER_INTERVAL 20
#define TIMER_ID 0
#define PI 3.14159265359

#define TEXTUREFILE1 "textures/quad_texture.bmp"
#define TEXTUREFILE2 "textures/act_quad_texture.bmp"


static void on_display();
static void on_reshape(int width, int height);
static void on_keyboard(unsigned char key, int x, int y);
static void on_keyboard2(int key, int x, int y);
static void on_timer(int id);
static void initialize_textures();

//pomocne funkcije

static void draw_square2(float x_coord, float z_coord, int activated);

static void draw_grid();

static void postavi_nivo(int lvl);

static void postavi_boje(float red, float green, float blue, float transparency);


//globalne promenljive za animaciju
float animation_parameter = 0;
int animation_ongoing = 0;


//imena tekstura
static GLuint tex_names[2];



//smer kretanja igraca koji je u pocetku u smeru z ose
int direction  = 2;
//da li je pozitivan smer, u pocetku jeste pa je jednak 1
//tj da li inkrementiramo ili dekrementiramo kretanje
int positive_dir = 1;

//brzina kretanja lopte
float vel_incr = 0.02;






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
	
	initialize_textures();
	
	glEnable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);
	//pozivamo jer obrcemo smer x ose
	glFrontFace(GL_CW);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);
    glutMainLoop();

  return 0;
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
        case ' ':
 		if(animation_ongoing){
			animation_ongoing=0;
			glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
		}
            break;       
        case 27:
		  glDeleteTextures(1, tex_names);
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


//timer i funkcija za ponovno iscrtavanje prozora

void on_timer(int id) {
    if (id == TIMER_ID) {
		if(!fail_condition)
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
    gluLookAt(position[0], position[1]+7, position[2]-7,
              position[0], position[1], position[2],
              0, 1, 0);

	//draw_grid();

   	         
	glPushMatrix();
	
	//iscrtavanje kvadrata po nivoima
	postavi_nivo(NIVO);
	
	
    glPopMatrix(); 
    
    
    glPushMatrix();

    postavi_boje(0.3,0.3,0.9,1.0);
    
    if(!fail_condition){
	   fail_condition_check(NIVO);
	}
	
	//ako animacija traje pomeramo igraca
	if (animation_ongoing && !fail_condition){
		//u zavisnosti od smera umanjujemo ili povecavamo
		if(positive_dir)    
			position[direction] += vel_incr;
		else
			position[direction] -= vel_incr;
		} else if (fail_condition){
			//idemo malo napred, pa propadamo, ala Pera Kojot	
			if(fail_animation_parameter<=0.5){
				if(positive_dir)    
					position[direction] += vel_incr;
				else
					position[direction] -= vel_incr;
				fail_animation_parameter += vel_incr;
				} else {
					//propadanje po y osi
					if(fail_animation_parameter<=2.5){
						position[1] -= vel_incr;
						fail_animation_parameter += vel_incr;
						} else {
							//dosli smo do kraja animacije propadanja i izlazimo iz programa
							glDeleteTextures(2, tex_names);
							exit(EXIT_FAILURE);
							}
					}
			}
   
	//pomeramo igraca
    glTranslatef(position[0],position[1],position[2]);		

    
    //lopta tj. igrac koji se krece po kvadratima
   
	glPushMatrix();
	
    glutSolidSphere(0.1,64,64);
    
    glPopMatrix();
    
	//proveravamo koliko nam je ostalo kvadrata na nivou
	int num_of_sq = 0;
	
	if(!fail_condition){
		num_of_sq=get_number_of_sq(NIVO);
	}
    //ako smo ostali sa samo jednim kvadratom onda prelazimo na sledeci nivo
    if(!fail_condition){
		if(num_of_sq==1){
			if(NIVO==10){
				glDeleteTextures(2, tex_names);
				exit(EXIT_SUCCESS);
			}
			NIVO+=1;
			set_zeroes();
			
			
			set_current_level(NIVO);
			
			set_player_position(NIVO);
		}
	}
    glPopMatrix();
	
	// stampamo tekst 
		
	
	print_level_info();

    glBindTexture(GL_TEXTURE_2D, 0);

    glutSwapBuffers();

}

//Pomocna funkcija koja ucitava teksture
//kod za rad sa teksturama preuzet sa sedmog casa Racunarske grafike

void initialize_textures(){
	
	Image* image;
	glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV,
              GL_TEXTURE_ENV_MODE,
              GL_REPLACE);
	
	
	
	image = image_init(0, 0);
	image_read(image, TEXTUREFILE1);
	
	glGenTextures(2, tex_names);
	
	
	glBindTexture(GL_TEXTURE_2D, tex_names[0]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
	
	image_read(image, TEXTUREFILE2);
	
	glBindTexture(GL_TEXTURE_2D, tex_names[1]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
	
	
	glBindTexture(GL_TEXTURE_2D, 0);
	
	
	image_done(image);
	
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




//Pomocna funkcija za iscrtavanje kvadrata, activated ima vrednost 0 ili 1 u zavisnosti od toga da li je kvadrat aktivan , tj igrac stoji na njemu ili ne

void draw_square2(float x_coord, float z_coord, int activated){
	glPushMatrix();
	glTranslatef(x_coord,0,z_coord);
	
	glColor3f(0,0,0);
	
	glBindTexture(GL_TEXTURE_2D, tex_names[activated]);
	glBegin(GL_QUADS);
	glNormal3f(0,1,0);
	
	glTexCoord2f(0, 0);
	glVertex3f(0.5,0,0.5);
	
	glTexCoord2f(0, 1);
	glVertex3f(0.5,0,-0.5);
	
	glTexCoord2f(1, 1);
	glVertex3f(-0.5,0,-0.5);
	
	glTexCoord2f(1, 0);
	glVertex3f(-0.5,0,0.5);
	
	glEnd();
	postavi_boje(0,0,0,1);
	
	/* Iskljucujemo aktivnu teksturu */
    glBindTexture(GL_TEXTURE_2D, 0);
	
	glPushMatrix();
	glTranslatef(0,0.005,0);
	glBegin(GL_LINE_STRIP);
	glVertex3f(0.5,0,0.5);
	glVertex3f(0.5,0,-0.5);
	glVertex3f(-0.5,0,-0.5);
	glVertex3f(-0.5,0,0.5);	
	glEnd();
	glPopMatrix();
	glPopMatrix();
	}
	
// pomocna funkcija za postavljanje nivoa, takodje postavlja neaktivne kvadrate
void postavi_nivo(int lvl){
	
	int i=0,j=0;
	
	
	int mati=nivo_dimenzije[lvl-1][0];
	int matj=nivo_dimenzije[lvl-1][1];
	
	
	while(i<mati){
				j=0;
				while(j<matj){
				//ako nije obrisan, iscrtavamo kvadrat
					if(curr_lvl_kv[i][j]){
					//provera da li je igrac i dalje na kvadratu ako nije onda ga brisemo, takodje proveravamo da li je igrac presao na neki drugi kvadrat
						if(curr_lvl_akt[i][j]){
							if(provera(i+0.5,j+0.5)){
								draw_square2(0.5+i,0.5+j, 1);
							} else {
								curr_lvl_kv[i][j]=0;
								curr_lvl_akt[i][j]=0;
								}
							} else {
								if(provera(i+0.5,j+0.5))
									curr_lvl_akt[i][j]=1; 
								draw_square2(0.5+i,0.5+j,0);
								}
						}
					j+=1;
					}
				i+=1;
			}
		
		}
