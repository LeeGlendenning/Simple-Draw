//============================================================================
// Name        : SimpleDraw.cpp
// Author      : Lee Glendenning
// Version     : Sept. 18, 2014
// Description : Simple drawing program.
//============================================================================


// OpenGL and C headers
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
// other include if you split the your design into several files, it is a good practice to do so


// C++ headers for file I/O
#include <fstream>
#include <iostream>
using namespace std;

// function prototype
//#define some constant here

// global variables
GLsizei winWidth = 640, winHeight = 480; // variables and initial for width and height of display window
GLint tool = 0, type = 1, style = 1, selected = -1, selection = 0, move = 0,
xbegin, ybegin, tempx1, tempy1, tempx2, tempy2, ptCtr = 0, numObj = 0;
GLfloat red = 1.0, green = 0.0, blue = 0.0;

// data structures, you define the structures for the drawing objects, you can use the data structure given below
class modelObject {
	public:
		GLint x1, y1, x2, y2, t, s; // t is for types of object: 1 for rectangle, 2 for cicle; s for drawing styles: 1 for filled, 2 for outline
		GLfloat r, g, b; // RGB color
} list[1000]; // this is an array data structure, you can also use link list for storing objects

// function section
// initial function to set up OpenGL state variable other than default.
void init(void) {
	glClearColor(1.0, 1.0, 1.0, 0.0); // Set display-window color to white
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, winWidth, winHeight, 0.0);
	glColor3f(1.0, 0.0, 0.0);//red
	glFlush();
}

void setPixel(GLint x, GLint y) {
	glPointSize(1.0);
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
}

void circleMidpoint(int x1, int y1, int x2, int y2){//unfilled
	int radius = sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
	int p = 1 - radius;
	int x = 0;
	int y = radius;
	//middle pixels
	setPixel(x1 - radius, y1);
	setPixel(x1 + radius, y1);
	setPixel(x1, y1 - radius);
	setPixel(x1, y1 + radius);
	while(x < y){
		x ++;
		if (p < 0){
			p += x + x + 1;
		}else{
			y --;
			p += x + x - y - y + 1;
		}
		//bottom semicircle
		setPixel(x1 + x, y1 + y);
		setPixel(x1 - x, y1 + y);
		setPixel(x1 + y, y1 + x);
		setPixel(x1 - y, y1 + x);
		setPixel(x1 + x, y1 - y);
		setPixel(x1 - x, y1 - y);
		setPixel(x1 + y, y1 - x);
		setPixel(x1 - y, y1 - x);
	}
}

void circleMidpointFilled(int x1, int y1, int x2, int y2, int i){//unfilled
	int radius = sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
	int p = 1 - radius;
	int x = 0;
	int y = radius;
	//middle pixels
	setPixel(x1 - radius, y1);
	setPixel(x1 + radius, y1);
	setPixel(x1, y1 - radius);
	setPixel(x1, y1 + radius);
	if (list[i].s == 1){//middle line
		glBegin(GL_LINES);
		glVertex2f(x1 - radius, y1);
		glVertex2f(x1 + radius, y1);
		glEnd();
	}
	while(x < y){
		x ++;
		if (p < 0){
			p += x + x + 1;
		}else{
			y --;
			p += x + x - y - y + 1;
		}
		//bottom semicircle
		setPixel(x1 + x, y1 + y);
		setPixel(x1 - x, y1 + y);
		setPixel(x1 + y, y1 + x);
		setPixel(x1 - y, y1 + x);
		//top semicircle
		setPixel(x1 + x, y1 - y);
		setPixel(x1 - x, y1 - y);
		setPixel(x1 + y, y1 - x);
		setPixel(x1 - y, y1 - x);
		if (list[i].s == 1){//fill
			glBegin(GL_LINES);
			glVertex2f(x1 - x, y1 + y);
			glVertex2f(x1 + x, y1 + y);
			glVertex2f(x1 - y, y1 + x);
			glVertex2f(x1 + y, y1 + x);
			glVertex2f(x1 - x, y1 - y);
			glVertex2f(x1 + x, y1 - y);
			glVertex2f(x1 - y, y1 - x);
			glVertex2f(x1 + y, y1 - x);
			glEnd();
		}
	}
}

// this function draws the list of objects
void drawList() {
	glClear(GL_COLOR_BUFFER_BIT); // Clear display window.

	for (int i = 1; i <= numObj; i++) {

		glColor3f(list[i].r, list[i].g, list[i].b);//set colour

		if (list[i].t == 1){//rectangle
			if (list[i].s == 1){//fill
				glBegin(GL_QUADS);
					glVertex2f(list[i].x1,list[i].y1);
					glVertex2f(list[i].x2,list[i].y1);
					glVertex2f(list[i].x2,list[i].y2);
					glVertex2f(list[i].x1,list[i].y2);
				glEnd();
			}else{//outline
				glBegin(GL_LINE_LOOP);
					glVertex2f(list[i].x1,list[i].y1);
					glVertex2f(list[i].x2,list[i].y1);
					glVertex2f(list[i].x2,list[i].y2);
					glVertex2f(list[i].x1,list[i].y2);
				glEnd();
			}
		}else if(list[i].t == 2){//circle
			if (list[i].s == 1){//fill
				circleMidpointFilled(list[i].x1,list[i].y1,list[i].x2,list[i].y2, i);
			}else{//outline
				circleMidpoint(list[i].x1,list[i].y1,list[i].x2,list[i].y2);
			}
		}
		glutSwapBuffers();
	}

	if (selection == 1 && selected != -1){
			glColor3f(1 - list[selected].r, 1 - list[selected].g, 1 - list[selected].b);//set inverse colour
			//printf("%d", selected);
			if (list[selected].t == 1){//square
				glBegin(GL_LINE_LOOP);
					glVertex2f(list[selected].x1,list[selected].y1);
					glVertex2f(list[selected].x2,list[selected].y1);
					glVertex2f(list[selected].x2,list[selected].y2);
					glVertex2f(list[selected].x1,list[selected].y2);
				glEnd();
			}else{//circle
				//printf("%d, %d\n", list[selected].x1, list[selected].y1);
				circleMidpoint(list[selected].x1, list[selected].y1, list[selected].x2, list[selected].y2);
			}
		}

	glFlush();
}

void doSelection(int xMouse, int yMouse){
	selected = -1;
	for(int i = numObj; i > 0; i --){
		//for squares
		if (list[i].x1 > list[i].x2){
			int temp = list[i].x2;
			list[i].x2 = list[i].x1;
			list[i].x1 = temp;
		}
		if (list[i].y1 > list[i].y2){
			int temp = list[i].y2;
			list[i].y2 = list[i].y1;
			list[i].y1 = temp;
		}

		if (list[i].t == 1 && xMouse <= list[i].x2 && xMouse >= list[i].x1 && yMouse <= list[i].y2 && yMouse >= list[i].y1){//mouse click is inside this object
			//printf("square at %d\n", i);
			selected = i;
			break;
		//for circles
		}else if (list[i].t == 2 && xMouse <= list[i].x2 && xMouse >= (list[i].x1 - (list[i].x2 - list[i].x1)) && yMouse <= list[i].y2 && yMouse >= (list[i].y1 - (list[i].y2 - list[i].y1))){//mouse click is inside this object
			//printf("circle at %d\n", i);
			selected = i;
			break;
		}
	}
}

void mouseDraw(GLint button, GLint action, GLint xMouse, GLint yMouse) {
	//printf("mousedraw call");
	if (action == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {
		if (selection == 1){//using select tool
			doSelection(xMouse, yMouse);
			//used when moving
			//printf("%d, %d", xbegin, ybegin);
			xbegin = xMouse;
			ybegin = yMouse;
		}else{//using a drawing tool
			numObj++;
			list[numObj].x1 = xMouse;
			list[numObj].y1 = yMouse;
			list[numObj].x2 = xMouse;
			list[numObj].y2 = yMouse;
			list[numObj].r = red;
			list[numObj].g = green;
			list[numObj].b = blue;
			list[numObj].t = type;
			list[numObj].s = style;
			//printf("%d, %d\n", list[numObj].x1, list[numObj].y1); // for debugging
		}

		glutPostRedisplay();
	}
	glFlush();
}



// this function takes the mouse position while moving mouse, use this for intermediate drawing
void Motion(GLint x, GLint y) {
	if (move == 1){
		if (xbegin == 0 && ybegin == 0){
			xbegin = x;
			ybegin = y;
		}
		list[selected].x1 = tempx1 + (x - xbegin);
		list[selected].x2 = tempx2 + (x - xbegin);
		list[selected].y1 = tempy1 + (y - ybegin);
		list[selected].y2 = tempy2 + (y - ybegin);
		//printf("did work\n");
		//printf("x = %d, xbegin = %d", x, xbegin);
	}else if(selection == 0){
		list[numObj].x2 = x;
		list[numObj].y2 = y;
	}
	// redisplay the object list after the above insertion. It will give a rubber band effect
	glutPostRedisplay();
	glFlush();
}

void deleteObj(){
	selection = 0;
	for (int i = selected; i < numObj; i ++){
		list[i] = list[i + 1];
	}
	glutPostRedisplay();
	glFlush();
}

//put element at back of array
void bringToFront(){
	modelObject temp = list[selected];
	for (int i = selected; i < numObj; i ++){
		list[i] = list[i + 1];
	}
	list[numObj] = temp;
}

//put element at front of array
void sendToBack(){
	modelObject temp = list[selected];
	for (int i = selected; i > 1; i --){
		list[i] = list[i - 1];
	}
	list[1] = temp;
}

void saveSVG(){
	FILE *outFile;
	outFile = fopen("output.xml", "w");
	char tmp[500];
	int objWidth, objHeight; //store calculated height and width

	//write header data for SVG
	sprintf(tmp, "<?xml version=\"1.0\" standalone=\"no\"?>\n");
	fputs(tmp, outFile);
	sprintf(tmp, "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"\n");
	fputs(tmp, outFile);
	sprintf(tmp, "\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n");
	fputs(tmp, outFile);
	sprintf(
			tmp,
			"<svg width=\"%d\" height=\"%d\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\">\n",
			winWidth, winHeight);
	fputs(tmp, outFile);

	int topX, topY;
	for (int i = 1; i <= numObj; i ++){
		if (list[i].t == 1) {//square
			if (list[i].x2 >= list[i].x1){
				objWidth = list[i].x2 - list[i].x1;
				topX = list[i].x1;
			}else{
				objWidth = list[i].x1 - list[i].x2;
				topX = list[i].x2;
			}

			if (list[i].y2 >= list[i].y1)
			{
				objHeight = list[i].y2 - list[i].y1;
				topY = list[i].y1;
			}else{
				objHeight = list[i].y1 - list[i].y2;
				topY = list[i].y2;
			}

			if (list[i].s == 1) {//fill
				sprintf(tmp, "<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" style=\"fill:rgb(%d,%d,%d);stroke-width:1;stroke:rgb(%d,%d,%d)\"/>\n", topX, topY, objWidth, objHeight, (int) (list[i].r * 255), (int) (list[i].g * 255), (int) (list[i].b * 255), (int) (list[i].r * 255), (int) (list[i].g * 255), (int) (list[i].b * 255));
				fputs(tmp, outFile);
			}else if(list[i].s == 2){//outline
				sprintf(tmp, "<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" style=\"fill:none;stroke-width:1;stroke:rgb(%d,%d,%d)\"/>\n", topX, topY, objWidth, objHeight, (int) (list[i].r * 255), (int) (list[i].g * 255), (int) (list[i].b * 255));
				fputs(tmp, outFile);
			}
		}else{//circle
			int r;
			if (list[i].x2 >= list[i].x1){
				r = (list[i].x2 - list[i].x1)/2;
				topX = list[i].x1;
			}else{
				r = (list[i].x1 - list[i].x2)/2;
				topX = list[i].x2;
			}

			if (list[i].y2 >= list[i].y1)
			{
				topY = list[i].y1;
			}else{
				topY = list[i].y2;
			}

			if (list[i].s == 1) {//fill
				sprintf(tmp, "<circle cx=\"%d\" cy=\"%d\" r=\"%d\" style=\"fill:rgb(%d,%d,%d);stroke-width:1;stroke:rgb(%d,%d,%d)\"/>\n", topX, topY, r, (int) (list[i].r * 255), (int) (list[i].g * 255), (int) (list[i].b * 255), (int) (list[i].r * 255), (int) (list[i].g * 255), (int) (list[i].b * 255));
				fputs(tmp, outFile);
			}else if(list[i].s == 2){//outline
				sprintf(tmp, "<circle cx=\"%d\" cy=\"%d\" r=\"%d\" style=\"fill:none;stroke-width:1;stroke:rgb(%d,%d,%d)\"/>\n", topX, topY, r, (int) (list[i].r * 255), (int) (list[i].g * 255), (int) (list[i].b * 255));
				fputs(tmp, outFile);
			}
		}
	}
	fputs("</svg>", outFile);
	fclose(outFile);
}

void openSVG(){
	FILE *in;
	char int_chr;
	char prev;
	numObj = 0;
	selection = 0;
	move = 0;

	glutPostRedisplay();

	in = fopen("output.xml", "r");

	glutPostRedisplay();

	in = fopen("output.xml", "r");

	do {
		prev = int_chr;
		int_chr = fgetc(in);

		if (int_chr == 'r' && prev == '<') {//parse rectangle
			numObj++;
			list[numObj].t = 1;

			fscanf(in, "ect x=\" %d \" y=\" %d \" width=\" %d \" height=\" %d \" style=\"fill:", &list[numObj].x1, &list[numObj].y1, &list[numObj].x2, &list[numObj].y2);
			list[numObj].x2 += list[numObj].x1;
			list[numObj].y2 += list[numObj].y1;

			if ((int_chr = fgetc(in)) == 'r') {
				fscanf(in, "gb( %f , %f , %f )", &list[numObj].r,
						&list[numObj].g, &list[numObj].b);
				list[numObj].s = 1;
			} else {
				fscanf(in, "one;stroke-width:1;stroke:rgb( %f , %f , %f )",
						&list[numObj].r, &list[numObj].g, &list[numObj].b);
				list[numObj].s = 2;
			}

			//convert colors back to float
			list[numObj].r = list[numObj].r / 255;
			list[numObj].g = list[numObj].g / 255;
			list[numObj].b = list[numObj].b / 255;

			//printf("rect x1=%d, y1=%d, x2=%d, y2=%d r=%f g=%f b=%f s=%d t=%d\n", list[numObj].x1, list[numObj].y1, list[numObj].x2, list[numObj].y2, list[numObj].r, list[numObj].g, list[numObj].b, list[numObj].s, list[numObj].t);
		}
		if (int_chr == 'c' && prev == '<') {//parse circle
			numObj++;
			list[numObj].t = 2;

			int radius;
			fscanf(in, "ircle cx=\" %d \" cy=\" %d \" r=\" %d \" style=\"fill:", &list[numObj].x1, &list[numObj].y1, &radius);
			list[numObj].x2 = radius*2;
			list[numObj].y2 = radius*2;
			list[numObj].x2 += list[numObj].x1;
			list[numObj].y2 += list[numObj].y1;

			if ((int_chr = fgetc(in)) == 'r') {
				fscanf(in, "gb( %f , %f , %f )", &list[numObj].r,
						&list[numObj].g, &list[numObj].b);
				list[numObj].s = 1;
			} else {
				fscanf(in, "one;stroke-width:1;stroke:rgb( %f , %f , %f )",
						&list[numObj].r, &list[numObj].g, &list[numObj].b);
				list[numObj].s = 2;
			}

			//convert colors back to float
			list[numObj].r = list[numObj].r / 255;
			list[numObj].g = list[numObj].g / 255;
			list[numObj].b = list[numObj].b / 255;

			//printf("circle x1=%d, y1=%d, x2=%d, y2=%d r=%f g=%f b=%f s=%d t=%d\n", list[numObj].x1, list[numObj].y1, list[numObj].x2, list[numObj].y2, list[numObj].r, list[numObj].g, list[numObj].b, list[numObj].s, list[numObj].t);
		}
	} while  (int_chr != EOF);

	fclose(in);
}

#define GL_BGR_EXT 0x80E0

void saveBitmap(const char *ptrcFileName, int nX, int nY, int nWidth, int nHeight)
{
    BITMAPFILEHEADER bf;
    BITMAPINFOHEADER bi;

    unsigned char *ptrImage = (unsigned char*) malloc(sizeof(unsigned char)*nWidth*nHeight*3 + (4-(3*nWidth)%4)*nHeight);


     FILE *ptrFile = fopen(ptrcFileName, "wb");

        //read pixels from framebuffer
        glReadPixels(nX, nY, nWidth, nHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, ptrImage );


        // set memory buffer for bitmap header and informaiton header
         memset(&bf, 0, sizeof(bf));
        memset(&bi, 0, sizeof(bi));

       // configure the headers with the give parameters

        bf.bfType = 0x4d42;
        bf.bfSize = sizeof(bf) + sizeof(bi) + nWidth*nHeight*3 + (4-(3*nWidth)%4)*nHeight;
        bf.bfOffBits = sizeof(bf) + sizeof(bi);
        bi.biSize = sizeof(bi);
        bi.biWidth = nWidth + nWidth%4;
        bi.biHeight = nHeight;
        bi.biPlanes = 1;
        bi.biBitCount = 24;
        bi.biSizeImage = nWidth*nHeight*3 + (4-(3*nWidth)%4)*nHeight;


    // to files
       fwrite(&bf, sizeof(bf), 1, ptrFile);
        fwrite(&bi, sizeof(bi), 1, ptrFile);
        fwrite(ptrImage, sizeof(unsigned char), nWidth*nHeight*3 + (4-(3*nWidth)%4)*nHeight, ptrFile);

        fclose(ptrFile);
        free(ptrImage);

}

// reshape function for resized the window
void winReshapeFcn(GLint newWidth, GLint newHeight) {
	/* Reset viewport and projection parameters */
	glViewport(0, 0, newWidth, newHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, GLdouble(newWidth), GLdouble(newHeight), 0.0);
	/* Reset display-window size parameters. */
	winWidth = newWidth;
	winHeight = newHeight;
	drawList();
	glFlush();
}

//menu variables
static int menu_id;
static int draw_submenu;
static int edit_submenu;
static int styles_submenu;
static int colours_submenu;
static int files_submenu;
static int window;

void menu(int num){
	if(num == 0){//close application
		glutDestroyWindow(window);
		exit(0);
	}else if(num == 1){
		for (int i = 0; i <= numObj; i ++){
			list[i].x1 = 0;
			list[i].x2 = 0;
			list[i].y1 = 0;
			list[i].y2 = 0;
			list[i].s = 0;
			list[i].t = 0;
			list[i].r = 0.0;
			list[i].g = 0.0;
			list[i].b = 0.0;
		}
		numObj = -1;
	}else if (num == 2){//square
		selection = 0;//not select tool
		move = 0;//not move tool
		type = 1;
	}else if(num == 3){//circle
		selection = 0;//not select tool
		move = 0;//not move tool
		type = 2;
	}else if(num == 4){//fill
		move = 0;
		style = 1;
	}else if(num == 5){//outline
		move = 0;
		style = 2;
	}else if(num == 6){//set colour to red
		move = 0;
		red = 1.0;
		green = 0.0;
		blue = 0.0;
	}else if(num == 7){//set colour to green
		move = 0;
		red = 0.0;
		green = 1.0;
		blue = 0.0;
	}else if(num == 8){//set colour to blue
		move = 0;
		red = 0.0;
		green = 0.0;
		blue = 1.0;
	}else if(num == 9){//save SVG
		saveSVG();//write XML file
	}else if(num == 10){//open SVG
		openSVG();
	}else if(num == 11){//export BMP
		saveBitmap("output.bmp", 0, 0, winWidth, winHeight);
	}else if (num == 12){
		move = 0;//not move tool
		selected = -1;
		selection = 1;
	}else if (num == 13){
		move = 0;
		selection = 0;
	}else if (num == 14){
		move = 0;
		deleteObj();
	}else if (num == 15){
		move = 0;
		bringToFront();
	}else if (num == 16){
		move = 0;
		sendToBack();
	}else if(num == 17){
		tempx1 = list[selected].x1;
		tempy1 = list[selected].y1;
		tempx2 = list[selected].x2;
		tempy2 = list[selected].y2;
		xbegin = 0;
		ybegin = 0;
		move = 1;
	}
	numObj ++;
	glutPostRedisplay();
}



void createMenu(void){

	draw_submenu = glutCreateMenu(menu);
	glutAddMenuEntry("Square", 2);
	glutAddMenuEntry("Circle", 3);
	menu_id = glutCreateMenu(menu);

	styles_submenu = glutCreateMenu(menu);
	glutAddMenuEntry("Fill", 4);
	glutAddMenuEntry("Outline", 5);
	menu_id = glutCreateMenu(menu);

	colours_submenu = glutCreateMenu(menu);
	glutAddMenuEntry("Red", 6);
	glutAddMenuEntry("Green", 7);
	glutAddMenuEntry("Blue", 8);
	menu_id = glutCreateMenu(menu);

	files_submenu = glutCreateMenu(menu);
	glutAddMenuEntry("Save SVG", 9);
	glutAddMenuEntry("Open SVG", 10);
	glutAddMenuEntry("Export to Bitmap", 11);
	menu_id = glutCreateMenu(menu);

	edit_submenu = glutCreateMenu(menu);
	glutAddMenuEntry("Select", 12);
	glutAddMenuEntry("Deselect", 13);
	glutAddMenuEntry("Delete", 14);
	glutAddMenuEntry("Bring To Front", 15);
	glutAddMenuEntry("Send To Back", 16);
	glutAddMenuEntry("Move", 17);
	menu_id = glutCreateMenu(menu);

	glutAddMenuEntry("New", 1);
	glutAddSubMenu("Draw", draw_submenu);
	glutAddSubMenu("Edit", edit_submenu);
	glutAddSubMenu("Styles", styles_submenu);
	glutAddSubMenu("Colours", colours_submenu);
	glutAddSubMenu("Files", files_submenu);
	glutAddMenuEntry("Quit", 0);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// main function
int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("SimpleDraw by Lee Glendenning");

	init();

	// register call back funtions
	glutDisplayFunc(drawList);
	glutReshapeFunc(winReshapeFcn);
	glutMouseFunc(mouseDraw);
	glutMotionFunc(Motion);

	// declare submenu variables and set menu here
	createMenu();

	glutMainLoop();

}


