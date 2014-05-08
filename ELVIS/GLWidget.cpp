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

void GLWidget::paintGL() {
    obj* objPt;
    line* linePt;
    circle* c;
    elipse* elip;
    
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);
        
    objPt = firstObj;
    while(objPt != NULL){
	linePt = objPt->firstLine;
	c = objPt->c;
	elip = objPt->elip;
	// Desenhando objetos com suas respectivas cores
	glColor3f(objPt->r, objPt->g, objPt->b);
	
	if(linePt != NULL) {
	    linePt = objPt->firstLine;
	    while(linePt != NULL) {
		bresenham(linePt->v1.x, linePt->v1.y, linePt->v2.x, linePt->v2.y);
		if(linePt->marked == true){
		    bresenham(linePt->v2.x - 5, linePt->v2.y - 5, linePt->v2.x + 5, linePt->v2.y - 5 );    
		    bresenham(linePt->v2.x + 5, linePt->v2.y - 5, linePt->v2.x + 5, linePt->v2.y + 5 );    
		    bresenham(linePt->v2.x + 5, linePt->v2.y + 5, linePt->v2.x - 5, linePt->v2.y + 5 );    
		    bresenham(linePt->v2.x - 5, linePt->v2.y + 5, linePt->v2.x - 5, linePt->v2.y - 5 );
		    bresenham(linePt->v1.x - 5, linePt->v1.y - 5, linePt->v1.x + 5, linePt->v1.y - 5 );    
		    bresenham(linePt->v1.x + 5, linePt->v1.y - 5, linePt->v1.x + 5, linePt->v1.y + 5 );    
		    bresenham(linePt->v1.x + 5, linePt->v1.y + 5, linePt->v1.x - 5, linePt->v1.y + 5 );    
		    bresenham(linePt->v1.x - 5, linePt->v1.y + 5, linePt->v1.x - 5, linePt->v1.y - 5 );
		}
		linePt = linePt->nextLine;
	    }
	}
	else if(c != NULL) {
	    midPtCircle(c->center.x, c->center.y, c->radius);
	}
	else if(elip != NULL) {
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
	/* FALTA DECLARAR O CASO DE ABORTAR A CRIACAO DA LINHA */
	if(event->button() == Qt::LeftButton) {
	    if(click==false) {
		pos1X = event->x();
		pos1Y = mouseH - event->y();
		pos2X = pos1X;
		pos2Y = pos1Y;
		click = true;
	    }
	    else {
		pos1X = pos2X;
		pos1Y = pos2Y;
		pos2X = event->x();
		pos2Y = mouseH - event->y();
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
	    if(lastObj->firstLine == NULL){
		lastObj->firstLine = new line();
		lastObj->lastLine = lastObj->firstLine;
	    }
	    else{
		lastObj->lastLine->nextLine = new line();
		lastObj->lastLine->nextLine->previousLine = lastObj->lastLine;
		lastObj->lastLine = lastObj->lastLine->nextLine;
	    }
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
	if(click == false) {
	    click = true;
	    pos1X = event->x();
	    pos1Y = mouseH - event->y();

	    if(firstObj == NULL) {
		firstObj = new obj();
		lastObj = firstObj;
	    }else {
		lastObj->nextObj = new obj();
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
	if(event->button() == Qt::LeftButton) {
	    if(click==false) {
		click=true;
		pos1X = event->x();
		pos1Y = mouseH - event->y();

		if(firstObj==NULL) {
		    firstObj = new obj();
		    lastObj=firstObj;
		}else {
		    lastObj->nextObj = new obj();
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
    /* Operacao de CLIPPING */
    else if(OPTION == 4){
	line* aux;
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
	    lastObj = lastObj->nextObj;
	}
	lastObj->firstLine = new line();
	lastObj->lastLine = lastObj->firstLine;
	lastObj->lastLine->v1.x = pos1X;
	lastObj->lastLine->v1.y = pos1Y;
	lastObj->lastLine->v2.x = pos1X;
	lastObj->lastLine->v2.y = pos2Y;

	lastObj->lastLine->nextLine = new line();
	lastObj->lastLine = lastObj->lastLine->nextLine;
	lastObj->lastLine->v1.x = pos1X;
	lastObj->lastLine->v1.y = pos2Y;
	lastObj->lastLine->v2.x = pos2X;
	lastObj->lastLine->v2.y = pos2Y;

	lastObj->lastLine->nextLine = new line();
	lastObj->lastLine = lastObj->lastLine->nextLine;
	lastObj->lastLine->v1.x = pos2X;
	lastObj->lastLine->v1.y = pos2Y;
	lastObj->lastLine->v2.x = pos2X;
	lastObj->lastLine->v2.y = pos1Y;

	lastObj->lastLine->nextLine = new line();
	lastObj->lastLine = lastObj->lastLine->nextLine;
	lastObj->lastLine->v1.x = pos2X;
	lastObj->lastLine->v1.y = pos1Y;
	lastObj->lastLine->v2.x = pos1X;
	lastObj->lastLine->v2.y = pos1Y;

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
	    markedLine = NULL;
	}
	updateGL();
    }
    else if(OPTION == 8){
	bool foundLine = true;
	objPt = firstObj;
	float m;
	int x0, y0, x1, y1, clipSize;
	clipSize = 4;

	pos1X = event->x();
	pos1Y = mouseH - event->y();
	// Remove marcação da linha anterior
	if(markedLine != NULL){
	    markedLine->marked = false;
	    markedLine = NULL;
	}
	if(objPt != NULL && objPt->firstLine != NULL){
	    while(objPt != NULL){
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
			printf("X");
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
	}
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
	    lastObj->c->radius = sqrt(pow((pos2Y - pos1X), 2) + pow((pos2Y - pos1Y), 2));
	}
	else if(OPTION == 3) {
	    lastObj->elip->rx = pos2X - pos1X;
	    lastObj->elip->ry = pos2Y - pos1Y;
	}
	else if(OPTION == 4){
	    lastObj->firstLine->v2.y = pos2Y;
	    lastObj->lastLine = lastObj->firstLine->nextLine;
	    lastObj->lastLine->v1.y = pos2Y;
	    lastObj->lastLine->v2.x = pos2X;
	    lastObj->lastLine->v2.y = pos2Y;
	    lastObj->lastLine = lastObj->lastLine->nextLine;
	    lastObj->lastLine->v1.x = pos2X;
	    lastObj->lastLine->v1.y = pos2Y;
	    lastObj->lastLine->v2.x = pos2X;
	    lastObj->lastLine = lastObj->lastLine->nextLine;
	    lastObj->lastLine->v1.x = pos2X;
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

void clean(){
    click = false;
    pos1X = 0;
    pos1Y = 0;
    pos2X = 0;
    pos2Y = 0;
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
	    clean();
	    break;
	case Qt::Key_2:
	    OPTION = 2;
	    clean();
	    break;
	case Qt::Key_3:
	    OPTION = 3;
	    clean();
	    break;
	case Qt::Key_7:
	    OPTION = 7;
	    clean();
	    break;
	case Qt::Key_4:
	    OPTION = 4;
	    clean();
	    break;
	case Qt::Key_8:
	    OPTION = 8;
	    clean();
	    break;
	default:
	    event->ignore();
	    break;
    }
}
