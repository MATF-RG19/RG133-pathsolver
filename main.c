#include <GL/glut.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "funkcije.h"


#define TIMER_INTERVAL 20
#define TIMER_ID 0
#define PI 3.14159265359

static void on_display();
static void on_reshape(int width, int height);
static void on_keyboard(unsigned char key, int x, int y);
static void on_keyboard2(int key, int x, int y);
static void on_timer(int id);


//globalne promenljive za animaciju
float animation_parameter = 0;
int animation_ongoing = 0;




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
			if(NIVO==10)
				exit(EXIT_SUCCESS);
			NIVO+=1;
			set_zeroes();
			
			
			set_current_level(NIVO);
			
			set_player_position(NIVO);
		}
	}
    glPopMatrix();
	
	// stampamo tekst 
		
	
	print_level_info();
	





    
    glutSwapBuffers();

}
