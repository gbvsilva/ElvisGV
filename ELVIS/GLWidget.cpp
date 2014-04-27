#include <QtGui/QMouseEvent>
#include <GL/glu.h>
#include "GLWidget.h"
#include "stdio.h"
#include <iostream>

float pos1X = 0;
float pos1Y = 0;
float pos2X = 0;
float pos2Y = 0;
bool click = false;
struct obj* head = NULL;
struct obj* last = NULL;


struct vertex{
    float x;
    float y;
};
struct line{
    line* next = NULL;
    vertex v1, v2;
};
struct obj{
    obj* next = NULL;
    line* firstLine = NULL;
};

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
    obj* pointer;
    pointer = head;
    while(pointer != NULL){
	glBegin(GL_LINES);
	glVertex2f(pointer->firstLine->v1.x, pointer->firstLine->v1.y);
	glVertex2f(pointer->firstLine->v2.x, pointer->firstLine->v2.y);
	glEnd();
	pointer = pointer->next;
    }
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
    mouseClick();
    printf("Clicou!\n");
    pos1X = pos2X;
    pos1Y = pos2Y;
    pos2X = event->x();
    pos2Y = event->y();
    if(click == false) click = true;
    else{
	click = false;
    if(head == NULL){
	    head = new obj();
        last = head;
	    head->firstLine = new line();
	    head->firstLine->v1.x = pos1X;
	    head->firstLine->v1.y = pos1Y;
	    head->firstLine->v2.x = pos2X;
        head->firstLine->v2.y = pos2Y;
	}
    else{
        last->next = new obj();
        last = last->next;
        last->firstLine = new line();
        last->firstLine->v1.x = pos1X;
        last->firstLine->v1.y = pos1Y;
        last->firstLine->v2.x = pos2X;
        last->firstLine->v2.y = pos2Y;
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
