#include <QtGui/QMouseEvent>
#include <GL/glu.h>
#include <iostream>
#include "GLWidget.h"
#include "stdio.h"
#include "structs.h"

// Comando a ser executado
int command = 0;
// Quantidade de linhas que falta serem desenhadas
int lineCounter = 0;
// Quantidade de linhas de uma polilinha
int lineSize = 2;
// Posição anterior e atual que o mouse clicou, respectivamente
float pos1X = 0;
float pos1Y = 0;
float pos2X = 0;
float pos2Y = 0;
// Clique inicial
bool click = false;
// Valor necessário para corrigir o mouse
int mouseH = 0;
// Ponteiro da cabeça da lista encadeada
struct obj* head = NULL;
// Ponteiro da cauda da lista encadeada
struct obj* last = NULL;

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
    glColor3f(1,0,0);
    obj* objPt;
    line* linePt;
    objPt = head;
    while(objPt != NULL){
	linePt = objPt->firstLine;
	while(linePt != NULL){
	    glBegin(GL_LINES);
	    glVertex2f(linePt->v1.x, linePt->v1.y);
	    glVertex2f(linePt->v2.x, linePt->v2.y);
	    glEnd();
	    linePt = linePt->next;
	}
	objPt = objPt->next;
    }
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
    mouseClick();
    printf("Clicou!\n");
    pos1X = pos2X;
    pos1Y = pos2Y;
    pos2X = event->x();
    pos2Y = mouseH - event->y();
    if(click == false){
	click = true;
	lineCounter = lineSize;
    }
    else{
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
	paintGL();
	updateGL();
	lineCounter--;
	if(lineCounter == 0){
	    click = false;
	    lineCounter = 0;
	}
    }
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    mouseMovement();
    printf("%d, %d\n", event->x(), event->y());

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
	default:
	    event->ignore();
	    break;
    }
}
