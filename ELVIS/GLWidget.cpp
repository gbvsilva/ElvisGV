#include <QtGui/QMouseEvent>
#include <GL/glu.h>
#include <iostream>
#include <math.h>
#include "GLWidget.h"
#include "stdio.h"
#include "structs.h"
#include "drawFunctions.h"

// Opcao da forma de desenho
int OPTION = 1;
// Posição anterior e atual que o mouse clicou, respectivamente
int pos1X = 0;
int pos1Y = 0;
int pos2X = 0;
int pos2Y = 0;
// Clique inicial
bool click = false;
// Valor necessário para corrigir o mouse
int mouseH = 0;
// Ponteiro da cabeça da lista encadeada
obj* firstObj = NULL;
// Ponteiro da cauda da lista encadeada
obj* lastObj = NULL;

line* markedLine = NULL;
obj* markedObj = NULL;

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent)
{
    setMouseTracking(true);
}

void GLWidget::initializeGL() {
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glEnable(GL_POLYGON_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.95, 0.95, 0.95, 1);
}

void GLWidget::resizeGL(int w, int h) {
    mouseH = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h); // set origin to bottom left corner
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void drawSquareMarker(int x, int y, int size){
    bresenham(x - size, y - size, x + size, y - size );    
    bresenham(x + size, y - size, x + size, y + size );    
    bresenham(x + size, y + size, x - size, y + size );    
    bresenham(x - size, y + size, x - size, y - size );
}

void GLWidget::paintGL() {
    obj* objPt;
    line* linePt;
    circle* c;
    elipse* elip;
    rectangle* rec;

    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);

    objPt = firstObj;
    while(objPt != NULL){
	linePt = objPt->firstLine;
	c = objPt->c;
	elip = objPt->elip;
	rec = objPt->rec;
	// Desenhando objetos com suas respectivas cores
	if(linePt != NULL) {
	    linePt = objPt->firstLine;
	    while(linePt != NULL) {
		glColor3f(objPt->lineColor->r, objPt->lineColor->g, objPt->lineColor->b);
		bresenham(linePt->v1.x, linePt->v1.y, linePt->v2.x, linePt->v2.y);
		if((markedObj != NULL && markedObj == objPt) || (markedLine != NULL && markedLine == linePt)) glColor3f(255, 0, 0);
		if(markedLine != NULL && markedLine == linePt->previousLine) glColor3f(255, 0, 0);
		drawSquareMarker(linePt->v1.x, linePt->v1.y, 5);
		linePt = linePt->nextLine;
	    }
	    if(markedLine != objPt->lastLine && markedObj != objPt) glColor3f(objPt->lineColor->r, objPt->lineColor->g, objPt->lineColor->b);
	    drawSquareMarker(objPt->lastLine->v2.x, objPt->lastLine->v2.y, 5);
	}
	else if(rec != NULL){
	    glColor3f(objPt->lineColor->r, objPt->lineColor->g, objPt->lineColor->b);
	    bresenham(rec->v1.x, rec->v1.y, rec->v2.x, rec->v2.y);
	    bresenham(rec->v2.x, rec->v2.y, rec->v3.x, rec->v3.y);
	    bresenham(rec->v3.x, rec->v3.y, rec->v4.x, rec->v4.y);
	    bresenham(rec->v4.x, rec->v4.y, rec->v1.x, rec->v1.y);
	    if(markedObj != NULL && markedObj == objPt) glColor3f(255, 0, 0);
	    drawSquareMarker(rec->v1.x, rec->v1.y, 5);	
	    drawSquareMarker(rec->v2.x, rec->v2.y, 5);	
	    drawSquareMarker(rec->v3.x, rec->v3.y, 5);	
	    drawSquareMarker(rec->v4.x, rec->v4.y, 5);	
	}
	else if(c != NULL) {
	    glColor3f(objPt->lineColor->r, objPt->lineColor->g, objPt->lineColor->b);
	    midPtCircle(c->center.x, c->center.y, c->radius);
	    if(markedObj != NULL && markedObj == objPt) glColor3f(255, 0, 0);
	    drawSquareMarker(c->center.x, c->center.y, 5);
	    drawSquareMarker(c->center.x + c->radius, c->center.y, 5);
	}
	else if(elip != NULL) {
	    glColor3f(objPt->lineColor->r, objPt->lineColor->g, objPt->lineColor->b);
	    midPtElipse(elip->center.x, elip->center.y, elip->rx, elip->ry);
	}
	objPt = objPt->nextObj;
    }
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
    mouseClick();
    obj* objPt;
    line* linePt;
    /* Desenhar LINHA */
    if(OPTION == 1) {
	    clearMarkers();
	if(event->button() == Qt::LeftButton) {
	    if(click == false) {
		pos1X = event->x();
		pos1Y = mouseH - event->y();
		pos2X = pos1X;
		pos2Y = pos1Y;
		click = true;
		if(firstObj == NULL){
		    firstObj = new obj();
		    lastObj = firstObj;
		}
		else{
		    lastObj->nextObj = new obj();
		    lastObj->nextObj->previousObj = lastObj;
		    lastObj = lastObj->nextObj;
		}

	    }
	    else {
		pos1X = pos2X;
		pos1Y = pos2Y;
		pos2X = event->x();
		pos2Y = mouseH - event->y();
	    }
	    if(lastObj->firstLine == NULL){
		lastObj->firstLine = new line();
		lastObj->lastLine = lastObj->firstLine;
	    }
	    else{
		lastObj->lastLine->nextLine = new line();
		lastObj->lastLine->nextLine->previousLine = lastObj->lastLine;
		lastObj->lastLine = lastObj->lastLine->nextLine;
	    }
	    lastObj->lastLine->top = lastObj;

	    lastObj->lastLine->v1.x = pos1X;
	    lastObj->lastLine->v1.y = pos1Y;
	    lastObj->lastLine->v2.x = pos2X;
	    lastObj->lastLine->v2.y = pos2Y;
	    updateGL();
	}
	else if(event->button() == Qt::MiddleButton) {
	    click=false;
	}
    }
    /* Desenhar CIRCULO */
    else if(OPTION==2) {
	    clearMarkers();
	if(click == false) {
	    click = true;
	    pos1X = event->x();
	    pos1Y = mouseH - event->y();

	    if(firstObj == NULL) {
		firstObj = new obj();
		lastObj = firstObj;
	    }else {
		lastObj->nextObj = new obj();
		lastObj->nextObj->previousObj = lastObj;
		lastObj = lastObj->nextObj;
	    }

	    lastObj->c = new circle();
	    lastObj->c->center.x = pos1X;
	    lastObj->c->center.y = pos1Y;
	}
	else {
	    pos2X = event->x();
	    pos2Y = mouseH - event->y();
	    lastObj->c->radius = sqrt(pow((pos2X - pos1X), 2) + pow((pos2Y - pos1Y), 2));

	    updateGL();
	    click=false;
	}
    }
    /* Desenhar ELIPSE */
    else if(OPTION == 3) {
	    clearMarkers();
	if(event->button() == Qt::LeftButton) {
	    if(click==false) {
		click=true;
		pos1X = event->x();
		pos1Y = mouseH - event->y();

		if(firstObj==NULL) {
		    firstObj = new obj();
		    lastObj=firstObj;
		}
		else {
		    lastObj->nextObj = new obj();
		    lastObj->nextObj->previousObj = lastObj;
		    lastObj = lastObj->nextObj;
		}

		lastObj->elip = new elipse();
		lastObj->elip->center.x=pos1X;
		lastObj->elip->center.y=pos1Y;
	    }else {
		pos2X = event->x();
		pos2Y = mouseH - event->y();
		lastObj->elip->rx = pos2X - pos1X;
		lastObj->elip->ry = pos2Y - pos1Y;
		updateGL();
		click=false;
	    }
	}
    }
    // Retângulo
    else if(OPTION == 4){
	rectangle* rec;
	    clearMarkers();
	if(click == false){
	    pos1X = event->x();
	    pos1Y = mouseH - event->y();
	    pos2X = pos1X;
	    pos2Y = pos1Y;
	    click = true;
	}
	else{
	    pos1X = pos2X;
	    pos1Y = pos2Y;
	    pos2X = event->x();
	    pos2Y = mouseH - event->y();
	    click = false;
	}
	if(firstObj == NULL){
	    firstObj = new obj();
	    lastObj = firstObj;
	}
	else{
	    lastObj->nextObj = new obj();
	    lastObj->nextObj->previousObj = lastObj;
	    lastObj = lastObj->nextObj;
	}
	lastObj->rec = new rectangle();
	rec = lastObj->rec;
	rec->v1.x = pos1X;
	rec->v1.y = pos1Y;
	rec->v2.x = pos1X;
	rec->v2.y = pos2Y;
	rec->v3.x = pos2X;
	rec->v3.y = pos2Y;
	rec->v4.x = pos2X;
	rec->v4.y = pos1Y;
	updateGL();
    }
    else if(OPTION == 7){
	if(click == false){
	    if(markedLine != NULL){
		line* newLine = new line();
		newLine->previousLine = markedLine;
		newLine->nextLine = markedLine->nextLine;
		newLine->v1.x = pos2X;
		newLine->v1.y = pos2Y;
		newLine->v2.x = markedLine->v2.x;
		newLine->v2.y = markedLine->v2.y;

		markedLine->v2.x = pos2X;
		markedLine->v2.y = pos2Y;
		markedLine->nextLine = newLine;

		if(newLine->nextLine != NULL){
		    newLine->nextLine->previousLine = newLine;
		    newLine->nextLine->v1.x = newLine->v2.x;
		    newLine->nextLine->v1.y = newLine->v2.y;
		}
		markedLine->marked = false;
		markedLine = newLine;
		click = true;
	    }
	}
	else{
	    click = false;
	    clearMarkers();
	}
	updateGL();
    }
    // Seleção de linha
    else if(OPTION == 8){
	bool foundLine = true;
	objPt = firstObj;
	float m;
	int x0, y0, x1, y1, clipSize;
	clipSize = 4;
	clearMarkers();

	pos1X = event->x();
	pos1Y = mouseH - event->y();
	markedLine = NULL;
	markedObj = NULL;
	while(objPt != NULL){
	    // Seleção por linha
	    linePt = objPt->firstLine;
	    while(linePt != NULL){
		foundLine = true;
		// Casos trivias
		if(linePt->v1.x < pos1X - clipSize && linePt->v2.x < pos1X - clipSize) foundLine = false;
		else if(linePt->v1.y < pos1Y - clipSize && linePt->v2.y < pos1Y - clipSize) foundLine = false;
		else if(linePt->v1.x > pos1X + clipSize && linePt->v2.x > pos1X + clipSize) foundLine = false;
		else if(linePt->v1.y > pos1Y + clipSize && linePt->v2.y > pos1Y + clipSize) foundLine = false;
		// Casos não triviais
		else{
		    if(linePt->v1.x > linePt->v2.x){
			x0 = linePt->v2.x;
			y0 = linePt->v2.y;
			x1 = linePt->v1.x;
			y1 = linePt->v1.y;
		    }
		    else{
			x0 = linePt->v1.x;
			y0 = linePt->v1.y;
			x1 = linePt->v2.x;
			y1 = linePt->v2.y;
		    }
		    m = (float)(y1-y0)/(float)(x1-x0);
		    if(	    y0 + m*(pos1X - clipSize - x0) > pos1Y - clipSize &&
			    y0 + m*(pos1X - clipSize - x0) < pos1Y + clipSize) foundLine  = true;
		    else if(    y0 + m*(pos1X + clipSize - x0) > pos1Y - clipSize &&
			    y0 + m*(pos1X + clipSize - x0) < pos1Y + clipSize) foundLine = true;
		    else if(    x0 + 1.0/m*(pos1Y - clipSize - y0) > pos1X - clipSize &&
			    x0 + 1.0/m*(pos1Y - clipSize - y0) < pos1X + clipSize) foundLine = true;
		    else if(    x0 + 1.0/m*(pos1Y + clipSize - y0) > pos1X - clipSize &&
			    x0 + 1.0/m*(pos1Y + clipSize - y0) < pos1X + clipSize) foundLine = true;
		}
		if(foundLine == true){
		    markedLine = linePt;
		}
		linePt = linePt->nextLine;
	    }
	    objPt = objPt->nextObj;
	}
	if(markedLine != NULL) markedLine->marked = true;
	updateGL();
    }
    // Seleção de objeto
    else if(OPTION == 9){
	bool foundLine = true;
	float m;
	int posDist;
	int x0, y0, x1, y1, count;
	int clipSize = 4;
	
	objPt = firstObj;
	clearMarkers();
	clipSize = 4;
	pos1X = event->x();
	pos1Y = mouseH - event->y();

	while(objPt != NULL){
	    linePt = objPt->firstLine;

	    // Seleção de polilinhas
	    if(linePt != NULL) while(linePt != NULL){
		foundLine = true;
		// Casos trivias
		if(linePt->v1.x < pos1X - clipSize && linePt->v2.x < pos1X - clipSize) foundLine = false;
		else if(linePt->v1.y < pos1Y - clipSize && linePt->v2.y < pos1Y - clipSize) foundLine = false;
		else if(linePt->v1.x > pos1X + clipSize && linePt->v2.x > pos1X + clipSize) foundLine = false;
		else if(linePt->v1.y > pos1Y + clipSize && linePt->v2.y > pos1Y + clipSize) foundLine = false;
		// Casos não triviais
		else{
		    if(linePt->v1.x > linePt->v2.x){
			x0 = linePt->v2.x;
			y0 = linePt->v2.y;
			x1 = linePt->v1.x;
			y1 = linePt->v1.y;
		    }
		    else{
			x0 = linePt->v1.x;
			y0 = linePt->v1.y;
			x1 = linePt->v2.x;
			y1 = linePt->v2.y;
		    }
		    m = (float)(y1-y0)/(float)(x1-x0);
		    if(	    y0 + m*(pos1X - clipSize - x0) > pos1Y - clipSize &&
			    y0 + m*(pos1X - clipSize - x0) < pos1Y + clipSize) foundLine  = true;
		    else if(    y0 + m*(pos1X + clipSize - x0) > pos1Y - clipSize &&
			    y0 + m*(pos1X + clipSize - x0) < pos1Y + clipSize) foundLine = true;
		    else if(    x0 + 1.0/m*(pos1Y - clipSize - y0) > pos1X - clipSize &&
			    x0 + 1.0/m*(pos1Y - clipSize - y0) < pos1X + clipSize) foundLine = true;
		    else if(    x0 + 1.0/m*(pos1Y + clipSize - y0) > pos1X - clipSize &&
			    x0 + 1.0/m*(pos1Y + clipSize - y0) < pos1X + clipSize) foundLine = true;
		}
		if(foundLine == true){
		    markedLine = linePt;
		    markedObj = objPt;
		}
		linePt = linePt->nextLine;
	    }
	    // Seleção da circunferência
	    else if(objPt->c != NULL){
		posDist = sqrt(pow((abs(objPt->c->center.x - pos1X)), 2) + pow((abs(objPt->c->center.y - pos1Y)), 2));
		if(objPt->c->radius > posDist - clipSize && objPt->c->radius < posDist + clipSize) markedObj = objPt;
	    }
	    // Seleção de retângulo
	    else if(objPt->rec != NULL){
		count = 0;
		foundLine = true;
		if(objPt->rec->v1.x < pos1X - clipSize && objPt->rec->v2.x < pos1X - clipSize) foundLine = false;
		else if(objPt->rec->v1.y < pos1Y - clipSize && objPt->rec->v2.y < pos1Y - clipSize) foundLine = false;
		else if(objPt->rec->v1.x > pos1X + clipSize && objPt->rec->v2.x > pos1X + clipSize) foundLine = false;
		else if(objPt->rec->v1.y > pos1Y + clipSize && objPt->rec->v2.y > pos1Y + clipSize) foundLine = false;
		if(foundLine == true) count++;
		if(objPt->rec->v2.x < pos1X - clipSize && objPt->rec->v3.x < pos1X - clipSize) foundLine = false;
		else if(objPt->rec->v2.y < pos1Y - clipSize && objPt->rec->v3.y < pos1Y - clipSize) foundLine = false;
		else if(objPt->rec->v2.x > pos1X + clipSize && objPt->rec->v3.x > pos1X + clipSize) foundLine = false;
		else if(objPt->rec->v2.y > pos1Y + clipSize && objPt->rec->v3.y > pos1Y + clipSize) foundLine = false;

		if(foundLine == true) count++;
		if(objPt->rec->v3.x < pos1X - clipSize && objPt->rec->v4.x < pos1X - clipSize) foundLine = false;
		else if(objPt->rec->v3.y < pos1Y - clipSize && objPt->rec->v4.y < pos1Y - clipSize) foundLine = false;
		else if(objPt->rec->v3.x > pos1X + clipSize && objPt->rec->v4.x > pos1X + clipSize) foundLine = false;
		else if(objPt->rec->v3.y > pos1Y + clipSize && objPt->rec->v4.y > pos1Y + clipSize) foundLine = false;

		if(foundLine == true) count++;
		if(objPt->rec->v4.x < pos1X - clipSize && objPt->rec->v1.x < pos1X - clipSize) foundLine = false;
		else if(objPt->rec->v4.y < pos1Y - clipSize && objPt->rec->v1.y < pos1Y - clipSize) foundLine = false;
		else if(objPt->rec->v4.x > pos1X + clipSize && objPt->rec->v1.x > pos1X + clipSize) foundLine = false;
		else if(objPt->rec->v4.y > pos1Y + clipSize && objPt->rec->v1.y > pos1Y + clipSize) foundLine = false;

		if(foundLine == true) count++;
		if(count > 0) markedObj = objPt;
	    }
	    // Seleção da elipse
	    // else if(){
	    // }
	    objPt = objPt->nextObj;
	}
	if(markedLine != NULL) markedLine->marked = true;
	updateGL();
    }
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    mouseMovement();
    //printf("%d, %d\n", event->x(), event->y());
    pos2X = event->x();
    pos2Y = mouseH - event->y();
    if(click == true) {
	if(OPTION == 1) {
	    lastObj->lastLine->v2.x = pos2X;
	    lastObj->lastLine->v2.y = pos2Y;
	}
	else if(OPTION == 2) {
	    lastObj->c->radius = sqrt(pow((pos2X - pos1X), 2) + pow((pos2Y - pos1Y), 2));
	}
	else if(OPTION == 3) {
	    lastObj->elip->rx = pos2X - pos1X;
	    lastObj->elip->ry = pos2Y - pos1Y;
	}
	else if(OPTION == 4){
	    lastObj->rec->v2.y = pos2Y;
	    lastObj->rec->v3.x = pos2X;
	    lastObj->rec->v3.y = pos2Y;
	    lastObj->rec->v4.x = pos2X;

	}
	else if(OPTION == 7){
	    markedLine->v1.x = pos2X;
	    markedLine->v1.y = pos2Y;
	    markedLine->previousLine->v2.x = pos2X;
	    markedLine->previousLine->v2.y = pos2Y;
	}
	updateGL();
    }
}

void clearMouse(){
    click = false;
    pos1X = 0;
    pos1Y = 0;
    pos2X = 0;
    pos2Y = 0;
}

void clearMarkers(){
    markedLine = NULL;
    markedObj = NULL;
}

void delSelected(){
    if(markedLine != NULL){
	if(markedLine->previousLine == NULL && markedLine->nextLine == NULL){
	    printf("del.1\n");
	    markedObj = markedLine->top;
	}
	else if(markedLine->previousLine == NULL){
	    printf("del.2\n");
	    markedLine->nextLine->previousLine = NULL;
	    markedLine->nextLine->v1.x = markedLine->v1.x;
	    markedLine->nextLine->v1.y = markedLine->v1.y;
	    markedLine->top->firstLine = markedLine->nextLine;
	}
	else if(markedLine->nextLine == NULL){
	    printf("del.3\n");
	    markedLine->previousLine->nextLine = NULL;
	    printf("X\n");
	    markedLine->previousLine->v2.x = markedLine->v2.x;
	    printf("X\n");
	    markedLine->previousLine->v2.y = markedLine->v2.y;
	    printf("X\n");
	    markedLine->top->lastLine = markedLine->previousLine;
	    printf("X\n");
	}
	else{
	    printf("del.4\n");
	    markedLine->previousLine->nextLine = markedLine->nextLine;
	    markedLine->nextLine->previousLine = markedLine->previousLine;
	    markedLine->previousLine->v2.x = (markedLine->v1.x + markedLine->v2.x) / 2;
	    markedLine->previousLine->v2.y = (markedLine->v1.y + markedLine->v2.y) / 2;
	    markedLine->nextLine->v1.x = (markedLine->v1.x + markedLine->v2.x) / 2;
	    markedLine->nextLine->v1.y = (markedLine->v1.y + markedLine->v2.y) / 2;
	}
	delete markedLine;
    }
    if(markedObj != NULL){
	if(markedObj->previousObj == NULL && markedObj->nextObj == NULL){
	    firstObj = NULL;
	    lastObj = NULL;
	}
	else if(markedObj->previousObj == NULL){
	    markedObj->nextObj->previousObj = NULL;
	    firstObj = markedObj->nextObj;
	}
	else if(markedObj->nextObj == NULL){
	    markedObj->previousObj->nextObj = NULL;
	    lastObj = markedObj->previousObj;
	}
	else{
	    markedObj->previousObj->nextObj = markedObj->nextObj;
	    markedObj->nextObj->previousObj = markedObj->previousObj;
	}
	delete markedLine;
	markedObj = NULL;
    }
    clearMarkers();
}

void GLWidget::mouseClick(){
    emit mouseClicked();
}

void GLWidget::mouseMovement(){
    emit mouseMoved();
}

void GLWidget::keyPressEvent(QKeyEvent* event) {
    switch(event->key()) {
	case Qt::Key_Escape:
	    close();
	    break;
	case Qt::Key_1:
	    OPTION = 1;
	    clearMouse();
	    break;
	case Qt::Key_2:
	    OPTION = 2;
	    clearMouse();
	    break;
	case Qt::Key_3:
	    OPTION = 3;
	    clearMouse();
	    break;
	case Qt::Key_7:
	    OPTION = 7;
	    clearMouse();
	    break;
	case Qt::Key_4:
	    OPTION = 4;
	    clearMouse();
	    break;
	case Qt::Key_8:
	    OPTION = 8;
	    clearMouse();
	    break;
	case Qt::Key_9:
	    OPTION = 9;
	    clearMouse();
	    break;
	case Qt::Key_Delete:
	    delSelected();
	    clearMouse();
	    updateGL();
	case Qt::Key_M:
	    updateGL();
	case Qt::Key_R:
	    updateGL();
	default:
	    event->ignore();
	    break;
    }
}
