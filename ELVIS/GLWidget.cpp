#include <QtGui/QMouseEvent>
#include <GL/glu.h>
#include <iostream>
#include <math.h>
#include "GLWidget.h"
#include "stdio.h"
#include "structs.h"
#include "drawFunctions.h"
#define OPTION 1

// Comando a ser executado
int command = 0;
// Quantidade de linhas que falta serem desenhadas
int lineCounter = 0;
// Quantidade de linhas de uma polilinha
int lineSize = 3;
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
    glColor3f(0,0,1);
    //glPushMatrix( );

    obj* objPt;
    objPt = head;

    while(objPt != NULL){
        /* Isso aqui ta uma confusao da porra, mas mais tarde teremos que concertar. */
        if(OPTION==1) {
            line* linePt;
            linePt = objPt->firstLine;
            while(linePt != NULL){
                glBegin(GL_LINES);
                glVertex2f(linePt->v1.x, linePt->v1.y);
                glVertex2f(linePt->v2.x, linePt->v2.y);
                glEnd();
                linePt = linePt->next;
            }


        }else if(OPTION==2) {
            circle *c = objPt->c;
            midPtCircle(c->center.x,c->center.y, c->radius);
        }
        objPt = objPt->next;
    }
}
void GLWidget::mousePressEvent(QMouseEvent *event) {
    mouseClick();
    printf("Clicou!\n");

    /* Desenhar linha */
    if(OPTION == 1) {

        if(event->button() == Qt::LeftButton) {
            if(click==false) {
                pos1X = event->x();
                pos1Y = mouseH - event->y();
                pos2X = pos1X;
                pos2Y = pos1Y;
                click=true;
            }else {
                pos1X = pos2X;
                pos1Y = pos2Y;
                pos2X = event->x();
                pos2Y = mouseH - event->y();
            }
            /*if(click == false){
                click = true;
                lineCounter = lineSize;
            }
            else{*/
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
            /*lineCounter--;
                if(lineCounter == 0){
                    click = false;
                    lineCounter = 0;
                }*/
        }else if(event->button() == Qt::MiddleButton) {
            printf("MIDDLE BUTTON\n");
            click=false;
        }
    }
    /* Desenhar circulo */
    else if(OPTION==2) {
        if(click == false) {
            click=true;

            if(head == NULL) {
                head = new obj();
                last = head;
            }else {
                last->next = new obj();
                last = last->next;
            }

            last->c = new circle();
            last->c->center.x = pos2X;
            last->c->center.y = pos2Y;
        }
        else {
            last->c->radius = sqrt(pow((pos2X - pos1X), 2) + pow((pos2Y - pos1Y), 2));
            paintGL();
            updateGL();
            click=false;
        }
    }
}


void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    mouseMovement();
    printf("%d, %d\n", event->x(), event->y());
    if(click == true) {
        if(OPTION==1) {
            last->lastLine->v1.x = pos2X;
            last->lastLine->v1.y = pos2Y;
            last->lastLine->v2.x = event->x();
            last->lastLine->v2.y = mouseH - event->y();
        }else if(OPTION==2) {
            last->c->radius = sqrt(pow((event->x() - pos2X), 2) + pow(((mouseH - event->y()) - pos2Y), 2));

        }
        paintGL();
        updateGL();
    }
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
