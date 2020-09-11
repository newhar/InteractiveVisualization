#include "XWindow.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <GL/glut.h>
#include "Bitmap.h"
using namespace std;

//function declaration
void ReadOBJ(const char* filename);
void initGL();
void createProgram();
char* ReadFile(char *filename);
GLuint createShader(char* src, GLenum type);
void initTexture();
void initNomTexture();
void display();
void ExitProgram();


//Global variables

GLuint program, vertShader, fragShader = 0;
int shoulder = 0, elbow = 0;
GLuint textureID1, textureID2, textureID3, textureID4;

Display                 *dpy;
Window                  root;
GLint                   att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
XVisualInfo             *vi;
Colormap                cmap;
XSetWindowAttributes    swa;
Window                  win;
GLXContext              glc;
XWindowAttributes       gwa;

static void drawBox(GLfloat size)
{	
	GLfloat n[6][3] =
	{
		{-1.0, 0.0, 0.0},
		{0.0, 1.0, 0.0},
		{1.0, 0.0, 0.0},
		{0.0, -1.0, 0.0},
		{0.0, 0.0, 1.0},
		{0.0, 0.0, -1.0}
	};
	GLint faces[6][4] =
	{
	{0, 1, 2, 3},
	{3, 2, 6, 7},
	{7, 6, 5, 4},
	{4, 5, 1, 0},
	{5, 6, 2, 1},
	{7, 4, 0, 3}
	};
	GLfloat v[8][3];
	GLint i;
	size = 1;
	
	v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size/2;
    v[4][0] = v[5][0] = v[6][0] = v[7][0] = size/2;
    v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size/2;
    v[2][1] = v[3][1] = v[6][1] = v[7][1] = size/2;
    v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size/2;
    v[1][2] = v[2][2] = v[5][2] = v[6][2] = size/2;
	
	
	//Initialize vertex & normal value 
	for(i = 5; i >= 0; i--) {
					glBegin(GL_QUADS);
					glNormal3fv(&n[i][0]);
					glTexCoord2f(1.0f, 1.0f);
					glVertex3fv(&v[faces[i][0]][0]);
					glTexCoord2f(1.0f, 0.0f);
					glVertex3fv(&v[faces[i][1]][0]);
					glTexCoord2f(0.0f, 0.0f);
					glVertex3fv(&v[faces[i][2]][0]);
					glTexCoord2f(0.0f, 1.0f);
					glVertex3fv(&v[faces[i][3]][0]);
					glEnd();
				}
}


void keyPressEvent(char* key_string){
	if(strncmp(key_string, "Up", 2) == 0){
		shoulder = (shoulder+5)%360;
	}else if(strncmp(key_string, "Down", 4) == 0){
		shoulder = (shoulder-5)%360;
	}else if(strncmp(key_string, "Right", 5) == 0){
		elbow = (elbow+5)%360;
	}else if(strncmp(key_string, "Left", 4) == 0){
		elbow = (elbow-5)%360;
	}
}

void initLight(){
	float lightKa[4] = {1.0,1.0,1.0,1.0};
	float lightKd[4] = {1.0,1.0,1.0,1.0};
	float lightKs[4] = {1.0,1.0,1.0,1.0};
	float lightPos[4] = {0.0f,0.0,1.0,1.0};
	
	
	
	float matKa[4] = {0.3,0.3,0.1,1.0f};
	float matKd[4] = {0.6,0.5,0.3,1.0f};
	float matKs[4] = {1.0,1.0,1.0,1.0f};
	float matShininess = 50.0f;
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightKa);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightKd);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightKs);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, matKa);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matKd);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matKs);
	glMaterialfv(GL_FRONT, GL_SHININESS, &matShininess);	
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	
}
int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	/*Initialize Here*/
	dpy = XOpenDisplay(NULL);
	if(dpy == NULL) {
        printf("\n\tcannot connect to X server\n\n");
        //exit(0);
	} 
	root = DefaultRootWindow(dpy);
	vi = glXChooseVisual(dpy, 0, att);
	if(vi == NULL) {
        printf("\n\tno appropriate visual found\n\n");
        //exit(0);
	} 
	else {
        printf("\n\tvisual %p selected\n", (void *)vi->visualid); /* %p creates hexadecimal output like in glxinfo */
	}
	cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
	swa.colormap = cmap;
	swa.event_mask = KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | StructureNotifyMask	;
	win = XCreateWindow(dpy, root, 0, 0, 600, 600, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
	XMapWindow(dpy, win);
	XStoreName(dpy, win, "2016320272_YeongJinGwon");
	glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
	glXMakeCurrent(dpy, win, glc); 
	glEnable(GL_DEPTH_TEST); 
	XEvent xev;
	
	
	initGL();
    initLight();
	initTexture();
	initNomTexture();

	while(1) {
       display();
	   XNextEvent(dpy, &xev);
	   if(xev.type == ConfigureNotify){
		    printf("ConfigureNotify\n");
			printf("Window ConfigureInformation position: (%d,%d)\n",xev.xconfigure.x,xev.xconfigure.y);
			printf("Window ConfigureInformation position: (%d,%d)\n",xev.xconfigure.width,xev.xconfigure.height);
	   }else if(xev.type == KeyPress){
			char *key_string = XKeysymToString(XkbKeycodeToKeysym(dpy, xev.xkey.keycode, 0, 0));
			keyPressEvent(key_string);
	   }else if(xev.type == KeyRelease){
		   	printf("KeyRelease Call\n");
			printf("[%s] Key is Released\n",XKeysymToString(XkbKeycodeToKeysym(dpy, xev.xkey.keycode, 0, 0)));
	   }else if(xev.type == ButtonPress){
			printf("ButtonPress Call\n");
			printf("Pointer Position: (%d,%d)\n",xev.xbutton.x,xev.xbutton.y);
			printf("Mouse %d Button is pressed\n",xev.xbutton.button);
	   }else if(xev.type == ButtonRelease){
			printf("ButtonRelease Call\n");
			printf("Pointer Position: (%d,%d)\n",xev.xbutton.x,xev.xbutton.y);
			printf("Mouse %d Button is Released\n",xev.xbutton.button);
	   }else if(xev.type == MotionNotify){
			printf("MotionNotify Call\n");
			printf("Pointer Position: (%d,%d)\n",xev.xmotion.x,xev.xmotion.y);
	   }
	}
}
	
void initTexture(){
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &textureID1);
	glBindTexture(GL_TEXTURE_2D, textureID1);
	Bitmap bmp = Bitmap::bitmapFromFile("textures/name_tex.jpg");
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp.width(), bmp.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, bmp.pixelBuffer());

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	
	glUniform1i(glGetUniformLocation(program, "tex"),0);
	//glBindTexture(GL_TEXTURE_2D, 0);
	
	
	
	
	glActiveTexture(GL_TEXTURE1);
	glGenTextures(1, &textureID2);
	glBindTexture(GL_TEXTURE_2D, textureID2);
	Bitmap bmp3 = Bitmap::bitmapFromFile("textures/id_tex.jpg");
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp3.width(), bmp3.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, bmp3.pixelBuffer());

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	
	glUniform1i(glGetUniformLocation(program, "tex"),0);
	//glBindTexture(GL_TEXTURE_2D, 0);
	
	
	
}

void initNomTexture(){
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &textureID3);
	glBindTexture(GL_TEXTURE_2D, textureID3);
	Bitmap bmp2 = Bitmap::bitmapFromFile("textures/name.jpg");
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp2.width(), bmp2.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, bmp2.pixelBuffer());

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	
	glUniform1i(glGetUniformLocation(program, "normal_tex"),0);
	//glBindTexture(GL_TEXTURE_2D, 0);
	
	glActiveTexture(GL_TEXTURE1);
	glGenTextures(1, &textureID4);
	glBindTexture(GL_TEXTURE_2D, textureID4);
	Bitmap bmp4 = Bitmap::bitmapFromFile("textures/id.jpg");
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp4.width(), bmp4.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, bmp4.pixelBuffer());

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	
	glUniform1i(glGetUniformLocation(program, "normal_tex"),0);
	//glBindTexture(GL_TEXTURE_2D, 0);
}
	
void initGL(){
	glewInit();
	createProgram();

	glEnable(GL_DEPTH_TEST);

}
void createProgram(){
	char* vert;
	char* frag;
	vert = ReadFile("Vertex.glsl");
	frag = ReadFile("Fragment.glsl");
	printf("%s\n",vert);
	printf("%s\n",frag);
	
	vertShader = createShader(vert, GL_VERTEX_SHADER);
	fragShader = createShader(frag, GL_FRAGMENT_SHADER);
	GLuint p = glCreateProgram();
	glAttachShader(p, vertShader);
	glAttachShader(p, fragShader);
	glLinkProgram(p);
	
	program = p;
}
char* ReadFile(char *filename){
   char *buffer = NULL;
   int string_size, read_size;
   FILE *handler = fopen(filename, "r");

   if (handler){
	   fseek(handler, 0, SEEK_END);
       string_size = ftell(handler);
       rewind(handler);
       buffer = (char*) malloc(sizeof(char) * (string_size + 1) );
       read_size = fread(buffer, sizeof(char), string_size, handler);
	   buffer[string_size] = '\0';
       if (string_size != read_size)
       {
           free(buffer);
           buffer = NULL;
       }
        fclose(handler);
    }
    return buffer;
}

GLuint createShader(char* src, GLenum type){
	GLuint shader;
	shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);
	return shader;
}

void display(){
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glUseProgram(program);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(85.0, 1.0, 1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-1.5, 0.0, -5.0);
	glPushMatrix();
	
		glRotatef(20, 1, 0, 1);
		glPushMatrix();
			
			glTranslatef(-1.0, 0.0, 0.0);
			glRotatef(shoulder, 0.0, 0.0, 1.0);
			glTranslatef(1.0, 0.0, 0.0);
			glPushMatrix();		
				glScalef(2.0, 0.4, 1.0);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, textureID1);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, textureID3);
				 //glutSolidTeapot(0.5f);
				drawBox(1);
			glPopMatrix();
			glTranslatef(1.0, 0.0, 0.0);
			glRotatef(elbow, 0.0, 0.0, 1.0);
			glTranslatef(1.0, 0.0, 0.0);
			glPushMatrix();
				glScalef(2.0, 0.4, 1.0);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, textureID2);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, textureID4);
				drawBox(1);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
		
	
	//glUseProgram(0);
	glXSwapBuffers(dpy, win);
	
	//glXSwapBuffers(dpy, win);
}

void ExitProgram() {
	glXMakeCurrent(dpy, None, NULL);
	glXDestroyContext(dpy, glc);
	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);
}

