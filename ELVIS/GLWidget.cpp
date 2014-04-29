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
obj* head = NULL;
// Ponteiro da cauda da lista encadeada
obj* last = NULL;

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
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);
    obj* objPt;
    line* linePt;
    circle* c;
    elipse* elip;

    objPt = head;
    while(objPt != NULL){
	linePt = objPt->firstLine;
	c = objPt->c;
	elip=objPt->elip;
	glColor3f(objPt->r, objPt->g, objPt->b);
	/* Desenhando objetos com suas respectivas cores */
	if(linePt != NULL) {
	    linePt = objPt->firstLine;
	    while(linePt != NULL) {
		bresenham(linePt->v1.x, linePt->v1.y, linePt->v2.x, linePt->v2.y);
		linePt = linePt->next;
	    }

	}
	else if(c != NULL) {
	    midPtCircle(c->center.x,c->center.y, c->radius);
	}else if(elip != NULL) {
	    midPtElipse(elip->center.x, elip->center.y, elip->rx, elip->ry);
	}
	objPt = objPt->next;
    }
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
    mouseClick();

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
	    }else {
		pos1X = pos2X;
		pos1Y = pos2Y;
		pos2X = event->x();
		pos2Y = mouseH - event->y();
	    }

	    if(head == NULL){

		head = new obj();
		last = head;
	    }
	    else{
		last->next = new obj();
		last = last->next;
	    }

	    if(last->firstLine == NULL){

		last->firstLine = new line();
		last->lastLine = last->firstLine;
	    }
	    else{
		last->lastLine->next = new line();
		last->lastLine = last->lastLine->next;
	    }

	    last->lastLine->v1.x = pos1X;
	    last->lastLine->v1.y = pos1Y;
	    last->lastLine->v2.x = pos2X;
	    last->lastLine->v2.y = pos2Y;

	    updateGL();


	}else if(event->button() == Qt::MiddleButton) {
	    click=false;
	}
    }
    /* Desenhar CIRCULO */
    else if(OPTION==2) {

	if(click == false) {
	    click = true;
	    pos1X = event->x();
	    pos1Y = mouseH - event->y();

	    if(head == NULL) {
		head = new obj();
		last = head;
	    }else {
		last->next = new obj();
		last = last->next;
	    }

	    last->c = new circle();
	    last->c->center.x = pos1X;
	    last->c->center.y = pos1Y;
	}
	else {
	    pos2X = event->x();
	    pos2Y = mouseH - event->y();
	    last->c->radius = sqrt(pow((pos2X - pos1X), 2) + pow((pos2Y - pos1Y), 2));

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

		if(head==NULL) {
		    head = new obj();
		    last=head;
		}else {
		    last->next = new obj();
		    last = last->next;
		}

		last->elip = new elipse();
		last->elip->center.x=pos1X;
		last->elip->center.y=pos1Y;
	    }else {
		pos2X = event->x();
		pos2Y = mouseH - event->y();
		last->elip->rx = pos2X - pos1X;
		last->elip->ry = pos2Y - pos1Y;
		updateGL();
		click=false;
	    }
	}
    }
    /* Operacao de CLIPPING */
    else if(OPTION == 4){

    }
    else if(OPTION == 8){
	obj* objPt;
	line* linePt;
	//line* foundLine = NULL;
	bool ycross = false;
	bool xcross = false;
	objPt = head;
	float m;
	int x0, y0, x1, y1, clipSize;
	clipSize = 8;

	pos1X = event->x();
	pos1Y = mouseH - event->y();
	if(objPt != NULL && objPt->firstLine != NULL){
	    while(objPt != NULL){
		linePt = objPt->firstLine;
		while(linePt != NULL){
		    bool found = true;
		    // Casos trivias
		    if(linePt->v1.x < pos1X - 2 && linePt->v2.x < pos1X - 2) found = false;
		    else if(linePt->v1.y < pos1Y - 2 && linePt->v2.y < pos1Y - 2) found = false;
		    else if(linePt->v1.x > pos1X + 2 && linePt->v2.x > pos1X + 2) found = false;
		    else if(linePt->v1.y > pos1Y + 2 && linePt->v2.y > pos1Y + 2) found = false;
		    // Casos não triviais
		    else{
			printf("Não trivial\n");
			xcross = false;
			ycross = false;
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
			found = false;
			if(y0 + m*(pos1X - clipSize - x0) > pos1Y - clipSize && y0 + m*(pos1X - clipSize - x0) < pos1Y + clipSize) found  = true;
			else if(y0 + m*(pos1X + clipSize - x0) > pos1Y - clipSize && y0 + m*(pos1X + clipSize - x0) < pos1Y + clipSize) found = true;
			else if(x0 + 1/m*(pos1Y - clipSize - y0) > pos1X - clipSize && x0 + 1/m*(pos1Y - clipSize - y0) < pos1X + clipSize) found = true;
			else if(x0 + 1/m*(pos1Y + clipSize - y0) > pos1X - clipSize && x0 + 1/m*(pos1Y + clipSize - y0) < pos1X + clipSize) found = true;
		    }
		    if(found == true){
			objPt->r = 1;
		    }

		    updateGL();
		    linePt = linePt->next;
		}
		objPt = objPt->next;
	    }
	}
    }
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    mouseMovement();
    //printf("%d, %d\n", event->x(), event->y());
    if(click == true) {
	if(OPTION==1) {
	    last->lastLine->v1.x = pos2X;
	    last->lastLine->v1.y = pos2Y;
	    last->lastLine->v2.x = event->x();
	    last->lastLine->v2.y = mouseH - event->y();
	}else if(OPTION==2) {
	    last->c->radius = sqrt(pow((event->x() - pos1X), 2) + pow(((mouseH - event->y()) - pos1Y), 2));
	}else if(OPTION==3) {
	    pos2X=event->x();
	    pos2Y=mouseH - event->y();
	    last->elip->rx = pos2X - pos1X;
	    last->elip->ry = pos2Y - pos1Y;
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
	    OPTION=1;
	    clean();
	    break;
	case Qt::Key_2:
	    OPTION=2;
	    clean();
	    break;
	case Qt::Key_3:
	    OPTION=3;
	    clean();
	    break;
	case Qt::Key_8:
	    OPTION=8;
	    clean();
	    break;
	default:
	    event->ignore();
	    break;
    }
}
