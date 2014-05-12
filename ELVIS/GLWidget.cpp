#include <QtGui/QMouseEvent>
#include <GL/glu.h>
#include <iostream>
#include <math.h>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <sstream>
#include "GLWidget.h"
#include "stdio.h"
#include "structs.h"
#include "drawFunctions.h"
#include "editFunctions.h"
using namespace std;

// Opcao de COMANDO
int OPTION = 1;
bool grid = false;
// Posição anterior e atual que o mouse clicou, respectivamente
int pos1X = 0;
int pos1Y = 0;
int pos2X = 0;
int pos2Y = 0;
// Valores do screen pan
int panX = 0;
int panY = 0;
int auxpanX = 0;
int auxpanY = 0;
// Tamanho do viewport, utilizado para corrigir a posição do mouse
int screenH, screenW;
float zoom = 1;
// Clique inicial
bool click = false;
// Clique para copia
bool cp = false;
// Valor necessário para corrigir o mouse
int mouseH = 0;
// Ponteiro da cabeça da lista encadeada
obj* firstObj = NULL;
// Ponteiro da cauda da lista encadeada
obj* lastObj = NULL;

line* markedLine = NULL;
line* markedLine2 = NULL;
obj* markedObj = NULL;

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent)
{
    setMouseTracking(true);
}
/**
 * Inicialização do openGL. Função utilizada apenas pelo Qt.
 */
void GLWidget::initializeGL() {
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glEnable(GL_POLYGON_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.95, 0.95, 0.95, 1);
}
/**
 * Redimensionamentoda janela e o plano de desenho (canvas)
 */
void GLWidget::resizeGL(int w, int h) {
    mouseH = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h); // set origin to bottom left corner
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    screenH = h;
    screenW = w;
}
/**
 * Desenho do marcador de ponto de controle
 */
void GLWidget::drawSquareMarker(float x, float y, int size){
    glColor3f(1, 0, 0);
    bresenham(x - size, y - size, x + size, y - size );
    bresenham(x + size, y - size, x + size, y + size );
    bresenham(x + size, y + size, x - size, y + size );
    bresenham(x - size, y + size, x - size, y - size );
}
/**
 * Desenho do marcador de ponto de controle, caso o objeto tenha sido selecionado
 */
void GLWidget::drawSelSquareMarker(float x, float y, int size, int color){
    glColor3f(0, 0, 0);
    bresenham(x - size, y - size, x + size, y - size );
    bresenham(x + size, y - size, x + size, y + size );
    bresenham(x + size, y + size, x - size, y + size );
    bresenham(x - size, y + size, x - size, y - size );
    if(color == 1)
	glColor3f(0, 1, 0);
    else glColor3f(1, 0, 0);
    for(int i = -size + 1; i <= size - 1; i++){
	bresenham(x - size + 1, y + i, x + size, y + i);
    }
}
/**
 * Função de pintura/desenho na tela. Ela é chamada na abertura do programa, quando a janela é
 * alterada (resizeGL()), ou quando é chamada manualmente (updateGL()).
 *
 * Nessa função, é desenhada a grade de desenho (caso esta tenha sido ativada), e todos os objetos
 * pertencentes à lista duplamente encadeada. Ele também trata de abrir os agrupamentos, caso
 * necessário.
 */
void GLWidget::paintGL() {
    // Auxiliares
    obj* objPt;
    obj* groupObj;
    line* linePt;
    circle* circPt;
    elipse* elipPt;
    rectangle* recPt;
    vertex v1, v2;
    // Tamanho da grade
    int gridSize;

    gridSize = 40;
    groupObj = NULL;
    objPt = firstObj;

    // Limpeza da tela
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);

    // Desenho da grade
    if(grid){
	glColor3f(0.9, 0.9, 0.9);
	for(int i = gridSize/2; i <= screenW/zoom; i+= gridSize)
	    bresenham((i - panX % gridSize)*zoom, 0, (i - panX % gridSize)*zoom, screenH);
	for(int i = gridSize/2; i <= screenH/zoom; i+= gridSize)
	    bresenham(0, (i - panY % gridSize)*zoom, screenW, (i - panY % gridSize)*zoom);

	glColor3f(0.7, 0.7, 0.7);
	for(int i = 0; i <= screenW/zoom; i+= gridSize)
	    bresenham((i - panX % gridSize)*zoom, 0, (i - panX % gridSize)*zoom, screenH);
	for(int i = 0; i <= screenH/zoom; i+= gridSize)
	    bresenham(0, (i - panY % gridSize)*zoom, screenW, (i - panY % gridSize)*zoom);
    }
    // Desenho de todos os objetos
    while(objPt != NULL){
	linePt = objPt->firstLine;
	circPt = objPt->c;
	elipPt = objPt->elip;
	recPt = objPt->rec;
	// Retirada dos objetos de um agrupamento
	if(objPt->group != NULL){
	    // Primeira passagem, desenhar todos os objetos do conjunto
	    if(objPt->mainGroup == false){
		objPt->mainGroup = true;
		if(groupObj == NULL) groupObj = objPt;
		objPt = objPt->group;
	    }
	    // Segunda passagem, desenhar o objeto seguinte
	    else{
		if(groupObj == objPt){
		    groupObj = NULL;
		    objPt->mainGroup = false;
		    // Caso tenha sido selecionado
		    if((markedObj != NULL && markedObj == objPt) ||
			    (markedLine != NULL && markedLine == linePt) ||
			    (markedLine != NULL && markedLine == linePt->previousLine) ||
			    (objPt->marked == true)){
			drawSelSquareMarker((objPt->rec->v1.x - panX)*zoom,
				(objPt->rec->v1.y - panY)*zoom, 5, 0);
			drawSelSquareMarker((objPt->rec->v2.x - panX)*zoom,
				(objPt->rec->v2.y - panY)*zoom, 5, 0);
			drawSelSquareMarker((objPt->rec->v3.x - panX)*zoom,
				(objPt->rec->v3.y - panY)*zoom, 5, 0);
			drawSelSquareMarker((objPt->rec->v4.x - panX)*zoom,
				(objPt->rec->v4.y - panY)*zoom, 5, 0);
		    }
		    // Caso ela não esteja em um grupo, marcação normal
		    else if(groupObj == NULL){
			drawSquareMarker((objPt->rec->v1.x - panX)*zoom,
				(objPt->rec->v1.y - panY)*zoom, 5);
			drawSquareMarker((objPt->rec->v2.x - panX)*zoom,
				(objPt->rec->v2.y - panY)*zoom, 5);
			drawSquareMarker((objPt->rec->v3.x - panX)*zoom,
				(objPt->rec->v3.y - panY)*zoom, 5);
			drawSquareMarker((objPt->rec->v4.x - panX)*zoom,
				(objPt->rec->v4.y - panY)*zoom, 5);
		    }
		    objPt = objPt->nextObj;
		}
	    }
	}
	// Demais objetos
	else{
	    if(objPt->marked == true) markedObj = objPt;
	    // Polilinhas
	    if(linePt != NULL) {
		linePt = objPt->firstLine;
		while(linePt != NULL) {
		    // Desenho de uma linha
		    glColor3f(objPt->lineColor->r, objPt->lineColor->g, objPt->lineColor->b);
		    bresenham((linePt->v1.x - panX)*zoom, (linePt->v1.y - panY)*zoom,
			    (linePt->v2.x - panX)*zoom, (linePt->v2.y - panY)*zoom);
		    // Caso a linha ou objeto esteja selecionado, usar marcação especial
		    if((markedObj != NULL && markedObj == objPt) ||
			    (markedLine != NULL && markedLine == linePt) ||
			    (markedLine != NULL && markedLine == linePt->previousLine)){
			drawSelSquareMarker((linePt->v1.x - panX)*zoom, (linePt->v1.y - panY)*zoom, 5, 0);
		    }
		    // Caso ela não esteja em um grupo, marcação normal
		    else if(groupObj == NULL){
			drawSquareMarker((linePt->v1.x - panX)*zoom, (linePt->v1.y - panY)*zoom, 5);
		    }
		    linePt = linePt->nextLine;
		}
		// Caso a linha ou objeto esteja selecionado, usar marcação especial
		if(markedLine == objPt->lastLine || markedObj == objPt || objPt->marked == true){
		    drawSelSquareMarker((objPt->lastLine->v2.x - panX)*zoom,
			    (objPt->lastLine->v2.y - panY)*zoom, 5, 0);
		}
		// Caso ela não esteja em um grupo, marcação normal
		else if(groupObj == NULL){
		    drawSquareMarker((objPt->lastLine->v2.x - panX)*zoom,
			    (objPt->lastLine->v2.y - panY)*zoom, 5);
		}
	    }
	    // Desenho de retângulo
	    else if(recPt != NULL){
		glColor3f(objPt->lineColor->r, objPt->lineColor->g, objPt->lineColor->b);
		bresenham((recPt->v1.x - panX)*zoom, (recPt->v1.y - panY)*zoom,
			(recPt->v2.x - panX)*zoom, (recPt->v2.y - panY)*zoom);
		bresenham((recPt->v2.x - panX)*zoom, (recPt->v2.y - panY)*zoom,
			(recPt->v3.x - panX)*zoom, (recPt->v3.y - panY)*zoom);
		bresenham((recPt->v3.x - panX)*zoom, (recPt->v3.y - panY)*zoom,
			(recPt->v4.x - panX)*zoom, (recPt->v4.y - panY)*zoom);
		bresenham((recPt->v4.x - panX)*zoom, (recPt->v4.y - panY)*zoom,
			(recPt->v1.x - panX)*zoom, (recPt->v1.y - panY)*zoom);
		// Caso possua preenchimento
		if(objPt->fillColor != NULL){
		    glColor3f(objPt->fillColor->r, objPt->fillColor->g, objPt->fillColor->b);
		    if(recPt->v1.x < recPt->v3.x){
			v1.x = recPt->v1.x;
			v2.x = recPt->v3.x;
		    }
		    else{
			v1.x = recPt->v3.x;
			v2.x = recPt->v1.x;
		    }
		    if(recPt->v1.y < recPt->v3.y){
			v1.y = recPt->v1.y;
			v2.y = recPt->v3.y;
		    }
		    else{
			v1.y = recPt->v3.y;
			v2.y = recPt->v1.y;
		    }
		    for(int i = v1.y+1; i < v2.y; i++){
			bresenham(v1.x+1, i, v2.x, i);
		    }
		}
		// Caso esteja selecionado, marcação especial
		if(markedObj != NULL && markedObj == objPt){
		    drawSelSquareMarker((recPt->v1.x - panX)*zoom, (recPt->v1.y - panY)*zoom, 5, 0);
		    drawSelSquareMarker((recPt->v2.x - panX)*zoom, (recPt->v2.y - panY)*zoom, 5, 0);
		    drawSelSquareMarker((recPt->v3.x - panX)*zoom, (recPt->v3.y - panY)*zoom, 5, 0);
		    drawSelSquareMarker((recPt->v4.x - panX)*zoom, (recPt->v4.y - panY)*zoom, 5, 0);
		}
		// Caso contrário
		else if(groupObj == NULL){
		    drawSquareMarker((recPt->v1.x - panX)*zoom, (recPt->v1.y - panY)*zoom, 5);	
		    drawSquareMarker((recPt->v2.x - panX)*zoom, (recPt->v2.y - panY)*zoom, 5);	
		    drawSquareMarker((recPt->v3.x - panX)*zoom, (recPt->v3.y - panY)*zoom, 5);	
		    drawSquareMarker((recPt->v4.x - panX)*zoom, (recPt->v4.y - panY)*zoom, 5);
		}
	    }
	    // Desenho de circunferência
	    else if(circPt != NULL) {
		glColor3f(objPt->lineColor->r, objPt->lineColor->g, objPt->lineColor->b);
		midPtCircle((circPt->center.x - panX)*zoom,
			(circPt->center.y - panY)*zoom,
			(circPt->radius)*zoom);
		// Caso tenha sido selecionado
		if(markedObj != NULL && markedObj == objPt){
		    drawSelSquareMarker((circPt->center.x - panX)*zoom,
			    (circPt->center.y - panY)*zoom, 5, 0);
		    drawSelSquareMarker((circPt->center.x - panX + circPt->radius)*zoom,
			    (circPt->center.y - panY)*zoom,  5, 0);
		}
		// Caso contrário
		else if(groupObj == NULL){
		    drawSquareMarker((circPt->center.x - panX)*zoom,
			    (circPt->center.y - panY)*zoom, 5);
		    drawSquareMarker((circPt->center.x - panX + circPt->radius)*zoom,
			    (circPt->center.y - panY)*zoom,  5);
		}
	    }
	    // Desenho de elipse
	    else if(elipPt != NULL) {
		glColor3f(objPt->lineColor->r, objPt->lineColor->g, objPt->lineColor->b);
		midPtElipse((elipPt->center.x - panX)*zoom,
			(elipPt->center.y - panY)*zoom,
			(elipPt->rx - panX)*zoom,
			(elipPt->ry - panY)*zoom);
	    }
	    objPt = objPt->nextObj;
	}
    }
}

/**
 * Função que captura o clique do mouse
 */
void GLWidget::mousePressEvent(QMouseEvent *event) {
    mouseClick();
    obj* objPt;
    line* linePt;
    // Criação de polilinha
    if(OPTION == 1) {
	clearMarkers();
	if(event->button() == Qt::LeftButton) {
	    // Primeiro clique do desenho
	    if(click == false) {
		pos1X = event->x()/zoom + panX;
		pos1Y = (mouseH - event->y())/zoom + panY;
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
	    // Cliques restantes
	    else {
		pos1X = pos2X;
		pos1Y = pos2Y;
		pos2X = event->x()/zoom + panX;
		pos2Y = (mouseH - event->y())/zoom + panY;
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
	    // Captura das posições
	    lastObj->lastLine->v1.x = pos1X;
	    lastObj->lastLine->v1.y = pos1Y;
	    lastObj->lastLine->v2.x = pos2X;
	    lastObj->lastLine->v2.y = pos2Y;
	}
	else if(event->button() == Qt::MiddleButton){
	    click = false;
	    markedObj = lastObj;
	    decreaseLayer(0);
	    clearMarkers();
	}
	else if(event->button() == Qt::RightButton){
	    click = false;
	    objPt = lastObj;
	    lastObj = lastObj->previousObj;
	    if(lastObj != NULL){
		lastObj->nextObj = NULL;
	    }
	    else firstObj = NULL;
	    delete objPt;
	}
	updateGL();
    }
    // Desenho de circunferência
    else if(OPTION==2) {
	clearMarkers();
	// Primeiro clique
	if(click == false) {
	    click = true;
	    pos1X = event->x()/zoom + panX;
	    pos1Y = (mouseH - event->y())/zoom + panY;

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
	// Segundo clique
	else {
	    pos2X = event->x()/zoom + panX;
	    pos2Y = (mouseH - event->y())/zoom + panY;
	    lastObj->c->radius = sqrt(pow((pos2X - pos1X), 2) + pow((pos2Y - pos1Y), 2));

	    markedObj = lastObj;
	    decreaseLayer(0);
	    clearMarkers();
	    updateGL();
	    click=false;
	}
    }
    // Desenho de elipse
    else if(OPTION == 3) {
	clearMarkers();
	if(event->button() == Qt::LeftButton) {
	    // Primeiro clique
	    if(click==false) {
		click=true;
		pos1X = event->x()/zoom + panX;
		pos1Y = (mouseH - event->y())/zoom + panY;

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
	    }
	    // Segundo clique
	    else {
		pos2X = event->x()/zoom + panX;
		pos2Y = (mouseH - event->y())/zoom + panY;
		lastObj->elip->rx = pos2X - pos1X;
		lastObj->elip->ry = pos2Y - pos1Y;
		updateGL();
		click=false;
	    }
	}
    }
    // Desenho de retângulo
    else if(OPTION == 4){
	rectangle* rec;
	clearMarkers();
	// Primeiro clique
	if(click == false){
	    pos1X = event->x()/zoom + panX;
	    pos1Y = (mouseH - event->y())/zoom + panY;
	    pos2X = pos1X;
	    pos2Y = pos1Y;
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
	    click = true;
	}
	// Segundo clique
	else{
	    rec = lastObj->rec;
	    pos2X = event->x()/zoom + panX;
	    pos2Y = (mouseH - event->y())/zoom + panY;
	    rec->v2.y = pos2Y;
	    rec->v3.x = pos2X;
	    rec->v3.y = pos2Y;
	    rec->v4.x = pos2X;
	    click = false;
	}
	markedObj = lastObj;
	decreaseLayer(0);
	clearMarkers();
	updateGL();
    }
    // Criação de um novo vértice em uma polilinha
    else if(OPTION == 7){
	if(click == false){
	    if(markedLine != NULL){
		line* newLine = new line();
		newLine->previousLine = markedLine;
		newLine->nextLine = markedLine->nextLine;
		newLine->top = markedLine->top;
		newLine->v1.x = pos2X;
		newLine->v1.y = pos2Y;
		newLine->v2.x = markedLine->v2.x;
		newLine->v2.y = markedLine->v2.y;

		markedLine->v2.x = pos2X;
		markedLine->v2.y = pos2Y;
		markedLine->nextLine = newLine;

		if(newLine->nextLine != NULL) newLine->nextLine->previousLine = newLine;
		else newLine->top->lastLine = newLine;
		markedLine2 = newLine;
		click = true;
	    }
	}
	else{
	    markedLine2 = NULL;
	    click = false;
	}
	clearMarkers();

    }
    // Seleção de vértice
    else if(OPTION == 12){
	bool foundVertex;
	float m;
	int posDist;
	int x0, y0, x1, y1;
	int clipSize = 4;

	clearMarkers();
	objPt = firstObj;

	pos1X = event->x()/zoom + panX;
	pos1Y = (mouseH - event->y())/zoom + panY;

	while(objPt != NULL){
	    linePt = objPt->firstLine;

	    // Seleção de polilinhas
	    if(linePt != NULL) while(linePt != NULL){
		foundVertex = true;
		if(linePt->v1.x < pos1X - clipSize &&
			linePt->v1.x > pos1Y + clipSize) foundVertex = false;
		if(linePt->v1.y < pos1Y - clipSize &&
			linePt->v1.y > pos1Y + clipSize) foundVertex = false;
		if(foundVertex == true){
		    markedObj = objPt;
		    markedLine = linePt;
		}
		if(linePt->nextLine == NULL){
		    if(linePt->v2.x < pos1X - clipSize &&
			    linePt->v2.x > pos1X + clipSize) foundVertex = false;
		    if(linePt->v2.y < pos1Y - clipSize &&
			    linePt->v2.y > pos1Y + clipSize) foundVertex = false;
		    if(foundVertex == true){
			markedObj = objPt;
			markedLine = NULL;
		    }
		}
	    }
	    // Seleção da circunferência
	    else if(objPt->c != NULL){
		if(objPt->c->center.x < pos1X - clipSize &&
			objPt->c->center.x > pos1X + clipSize) foundVertex = false;
		if(objPt->c->center.y < pos1Y - clipSize &&
			objPt->c->center.y > pos1Y + clipSize) foundVertex = false;
		if(objPt->c->center.x < pos1X - clipSize + objPt->c->radius &&
			objPt->c->center.x > pos1X + clipSize + objPt->c->radius ) foundVertex = false;
		if(objPt->c->center.y < pos1Y - clipSize + objPt->c->radius &&
			objPt->c->center.y > pos1Y + clipSize + objPt->c->radius) foundVertex = false;
		if(foundVertex == true) markedObj = objPt;
	    }
	    // Seleção de retângulo
	    // else if(objPt->rec != NULL){
	    // }
	    // Seleção da elipse
	    // else if(){
	    // }
	    // Seleção para o agrupamento
	    if(markedObj == objPt && OPTION == 10){
		markedObj = NULL;
		if(objPt->marked == false) objPt->marked = true;
		else objPt->marked = false;
	    }
	    objPt = objPt->nextObj;
	}
	if(markedObj != NULL && cp==true) {
	    OPTION=21;
	    click=true;
	    lastObj->nextObj = copy(markedObj);
	    lastObj->nextObj->previousObj = lastObj;
	    lastObj = lastObj->nextObj;
	}
    }

    // Seleção de uma linha da polilinha
    else if(OPTION == 8){
	bool foundLine;
	objPt = firstObj;
	float m;
	int x0, y0, x1, y1, clipSize;

	clipSize = 4;
	clearMarkers();

	pos1X = event->x()/zoom + panX;
	pos1Y = (mouseH - event->y())/zoom + panY;

	while(objPt != NULL){
	    linePt = objPt->firstLine;
	    while(linePt != NULL){

		// Seleção de polilinhas
		foundLine = true;
		// Casos trivias
		if(linePt->v1.x < pos1X - clipSize &&
			linePt->v2.x < pos1X - clipSize) foundLine = false;
		else if(linePt->v1.y < pos1Y - clipSize &&
			linePt->v2.y < pos1Y - clipSize) foundLine = false;
		else if(linePt->v1.x > pos1X + clipSize &&
			linePt->v2.x > pos1X + clipSize) foundLine = false;
		else if(linePt->v1.y > pos1Y + clipSize &&
			linePt->v2.y > pos1Y + clipSize) foundLine = false;
		// Casos não triviais
		else{
		    foundLine = false;
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
		    if( y0 + m*(pos1X - clipSize - x0) > pos1Y - clipSize &&
			    y0 + m*(pos1X - clipSize - x0) < pos1Y + clipSize) foundLine  = true;
		    else if(y0 + m*(pos1X + clipSize - x0) > pos1Y - clipSize &&
			    y0 + m*(pos1X + clipSize - x0) < pos1Y + clipSize) foundLine = true;
		    else if(x0 + 1.0/m*(pos1Y - clipSize - y0) > pos1X - clipSize &&
			    x0 + 1.0/m*(pos1Y - clipSize - y0) < pos1X + clipSize) foundLine = true;
		    else if(x0 + 1.0/m*(pos1Y + clipSize - y0) > pos1X - clipSize &&
			    x0 + 1.0/m*(pos1Y + clipSize - y0) < pos1X + clipSize) foundLine = true;
		}
		if(foundLine == true){
		    markedLine = linePt;
		}
		linePt = linePt->nextLine;
	    }
	    objPt = objPt->nextObj;
	}
	updateGL();
    }
    /*
     * Seleção de objetos. Caso a opção 9 tenha sido selecionada, apenas um objeto será selecionado.
     * Caso seja a opção 10, vários objetos serão selecionados para o agrupamento.
     */
    else if(OPTION == 9 || OPTION == 10){
	bool foundLine;
	float m;
	int posDist;
	int x0, y0, x1, y1;
	int clipSize = 4;

	clearMarkers();
	objPt = firstObj;

	pos1X = event->x()/zoom + panX;
	pos1Y = (mouseH - event->y())/zoom + panY;
	// Leitura de todos os objetos
	while(objPt != NULL){
	    linePt = objPt->firstLine;
	    // Seleção de polilinhas
	    if(linePt != NULL) while(linePt != NULL){
		foundLine = true;
		// Casos trivias
		if(linePt->v1.x < pos1X - clipSize &&
			linePt->v2.x < pos1X - clipSize) foundLine = false;
		else if(linePt->v1.y < pos1Y - clipSize &&
			linePt->v2.y < pos1Y - clipSize) foundLine = false;
		else if(linePt->v1.x > pos1X + clipSize &&
			linePt->v2.x > pos1X + clipSize) foundLine = false;
		else if(linePt->v1.y > pos1Y + clipSize &&
			linePt->v2.y > pos1Y + clipSize) foundLine = false;
		// Casos não triviais
		else{
		    foundLine = false;
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
		    if(y0 + m*(pos1X - clipSize - x0) > pos1Y - clipSize &&
			    y0 + m*(pos1X - clipSize - x0) < pos1Y + clipSize) foundLine  = true;
		    else if(y0 + m*(pos1X + clipSize - x0) > pos1Y - clipSize &&
			    y0 + m*(pos1X + clipSize - x0) < pos1Y + clipSize) foundLine = true;
		    else if(x0 + 1.0/m*(pos1Y - clipSize - y0) > pos1X - clipSize &&
			    x0 + 1.0/m*(pos1Y - clipSize - y0) < pos1X + clipSize) foundLine = true;
		    else if(x0 + 1.0/m*(pos1Y + clipSize - y0) > pos1X - clipSize &&
			    x0 + 1.0/m*(pos1Y + clipSize - y0) < pos1X + clipSize) foundLine = true;
		}
		if(foundLine == true) markedObj = objPt;
		linePt = linePt->nextLine;
	    }

	    // Seleção da circunferência
	    else if(objPt->c != NULL){
		posDist = sqrt(pow((abs(objPt->c->center.x - pos1X)), 2) +
			pow((abs(objPt->c->center.y - pos1Y)), 2));
		if(objPt->c->radius > posDist - clipSize &&
			objPt->c->radius < posDist + clipSize) markedObj = objPt;
	    }

	    // Seleção de retângulo. Cada lado é checado
	    else if(objPt->rec != NULL){
		foundLine = true;
		if(objPt->rec->v1.x < pos1X - clipSize &&
			objPt->rec->v2.x < pos1X - clipSize) foundLine = false;
		else if(objPt->rec->v1.y < pos1Y - clipSize &&
			objPt->rec->v2.y < pos1Y - clipSize) foundLine = false;
		else if(objPt->rec->v1.x > pos1X + clipSize &&
			objPt->rec->v2.x > pos1X + clipSize) foundLine = false;
		else if(objPt->rec->v1.y > pos1Y + clipSize &&
			objPt->rec->v2.y > pos1Y + clipSize) foundLine = false;
		if(foundLine == true) markedObj = objPt;
		foundLine = true;
		if(objPt->rec->v2.x < pos1X - clipSize &&
			objPt->rec->v3.x < pos1X - clipSize) foundLine = false;
		else if(objPt->rec->v2.y < pos1Y - clipSize &&
			objPt->rec->v3.y < pos1Y - clipSize) foundLine = false;
		else if(objPt->rec->v2.x > pos1X + clipSize &&
			objPt->rec->v3.x > pos1X + clipSize) foundLine = false;
		else if(objPt->rec->v2.y > pos1Y + clipSize &&
			objPt->rec->v3.y > pos1Y + clipSize) foundLine = false;
		if(foundLine == true) markedObj = objPt;
		foundLine = true;
		if(objPt->rec->v3.x < pos1X - clipSize &&
			objPt->rec->v4.x < pos1X - clipSize) foundLine = false;
		else if(objPt->rec->v3.y < pos1Y - clipSize &&
			objPt->rec->v4.y < pos1Y - clipSize) foundLine = false;
		else if(objPt->rec->v3.x > pos1X + clipSize &&
			objPt->rec->v4.x > pos1X + clipSize) foundLine = false;
		else if(objPt->rec->v3.y > pos1Y + clipSize &&
			objPt->rec->v4.y > pos1Y + clipSize) foundLine = false;
		if(foundLine == true) markedObj = objPt;
		foundLine = true;
		if(objPt->rec->v4.x < pos1X - clipSize &&
			objPt->rec->v1.x < pos1X - clipSize) foundLine = false;
		else if(objPt->rec->v4.y < pos1Y - clipSize &&
			objPt->rec->v1.y < pos1Y - clipSize) foundLine = false;
		else if(objPt->rec->v4.x > pos1X + clipSize &&
			objPt->rec->v1.x > pos1X + clipSize) foundLine = false;
		else if(objPt->rec->v4.y > pos1Y + clipSize &&
			objPt->rec->v1.y > pos1Y + clipSize) foundLine = false;
		if(foundLine == true) markedObj = objPt;
		foundLine = false;

	    }

	    // Seleção da elipse
	    // else if(){
	    // }

	    // Caso a seleção seja para o agrupamento
	    if(markedObj == objPt && OPTION == 10){
		markedObj = NULL;
		if(objPt->marked == false) objPt->marked = true;
		else objPt->marked = false;
	    }
	    objPt = objPt->nextObj;
	}
	// Seleção para cópia
	if(markedObj != NULL && cp==true) {
	    OPTION=21;
	    click=true;
	    lastObj->nextObj = copy(markedObj);
	    lastObj->nextObj->previousObj = lastObj;
	    lastObj = lastObj->nextObj;
	}
    }
    // Screen pan
    else if(OPTION == 20){
	if(click == false){
	    pos1X = (event->x())/zoom;
	    pos1Y = (mouseH - event->y())/zoom;
	    click = true;
	}
	else{
	    auxpanX = panX;
	    auxpanY = panY;
	    click = false;
	}
    }
    // Colocar desenho copiado
    else if(OPTION == 21) {
	if(event->button() == Qt::LeftButton) {
	    if(click == true) {
		if(lastObj->c != NULL ) {
		    lastObj->c->center.x = event->x() - (pos1X - markedObj->c->center.x);
		    lastObj->c->center.y = (mouseH - event->y()) - (pos1Y - markedObj->c->center.y);
		}else if(lastObj->rec != NULL) {

		}
	    }
	}
	cp=false;
	clearMouse();
	clearMarkers();
    }
    updateGL();
}

/*
 *  Captura de movimento do mouse
 */
void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    mouseMovement();
    pos2X = event->x()/zoom + panX;
    pos2Y = (mouseH - event->y())/zoom + panY;
    // Caso um clique tenha sido feito anteriormente
    if(click == true) {
	// Desenho da polilinha em tempo real
	if(OPTION == 1) {
	    lastObj->lastLine->v2.x = pos2X;
	    lastObj->lastLine->v2.y = pos2Y;
	}
	// Desenho da circunferência em tempo real
	else if(OPTION == 2) {
	    lastObj->c->radius = sqrt(pow((pos2X - pos1X), 2) + pow((pos2Y - pos1Y), 2));
	}
	// Desenho da elipse em tempo real
	else if(OPTION == 3) {
	    lastObj->elip->rx = pos2X - pos1X;
	    lastObj->elip->ry = pos2Y - pos1Y;
	}
	// Desenho do retângulo em tempo real
	else if(OPTION == 4){
	    lastObj->rec->v2.y = pos2Y;
	    lastObj->rec->v3.x = pos2X;
	    lastObj->rec->v3.y = pos2Y;
	    lastObj->rec->v4.x = pos2X;
	}
	// Adição de um novo vértice
	else if(OPTION == 7){
	    markedLine2->v1.x = pos2X;
	    markedLine2->v1.y = pos2Y;
	    markedLine2->previousLine->v2.x = pos2X;
	    markedLine2->previousLine->v2.y = pos2Y;
	}
	// Screen pan
	else if(OPTION == 20){
	    pos2X = event->x()/zoom;
	    pos2Y = (mouseH - event->y())/zoom;
	    panX = auxpanX + pos1X - pos2X;
	    panY = auxpanY + pos1Y - pos2Y;

	}else if(OPTION == 21) {
	    /* Operacao de copia. Inicia-se movendo o objeto. */
	    /* SEM TRATAR O ZOOM E O SPAN */
	    if(lastObj->c != NULL) {
		lastObj->c->center.x = event->x() - (pos1X - markedObj->c->center.x);
		lastObj->c->center.y = (mouseH - event->y()) - (pos1Y - markedObj->c->center.y);
		//drawSelSquareMarker((lastObj->c->center.x - panX), (lastObj->c->center.y - panY), 5, 1);
		//drawSelSquareMarker((lastObj->c->center.x - panX + lastObj->c->radius), (lastObj->c->center.y - panY), 5, 1);
	    }else if(lastObj->rec != NULL) {
		lastObj->rec->v1.x = event->x() - (pos1X - markedObj->rec->v1.x);
		lastObj->rec->v1.y = (mouseH - event->y()) - (pos1Y - markedObj->rec->v1.y);
		lastObj->rec->v2.x = lastObj->rec->v1.x;
		lastObj->rec->v2.y = (mouseH - event->y()) - (pos1Y - markedObj->rec->v2.y);
		lastObj->rec->v3.x = event->x() - (pos1X - markedObj->rec->v3.x);
		lastObj->rec->v3.y = lastObj->rec->v2.y;
		lastObj->rec->v4.x = lastObj->rec->v3.x;
		lastObj->rec->v4.y = lastObj->rec->v1.y;
	    }

	}
	updateGL();
    }
}

// Limpa as informações relativas ao mouse
void GLWidget::clearMouse(){
    click = false;
    pos1X = 0;
    pos1Y = 0;
    pos2X = 0;
    pos2Y = 0;
}

// Salva todos os objetos em um arquivo
void GLWidget::save(){
    ofstream save;
    obj* objPt;
    obj* groupObj;
    line* linePt;
    circle* circPt;
    elipse* elipPt;
    rectangle* recPt;

    save.open("save.elv");
    save << "BEGINSAVE ";
    std::ostringstream ss;
    objPt = firstObj;

    while(objPt != NULL){
	ss << "\nBEGINOBJ ";
	ss << objPt->layer;
	ss << " ";
	if(objPt->fillColor == NULL){
	    ss << "-1 -1 -1 ";
	}
	else{
	    ss << objPt->fillColor->r;
	    ss << " ";
	    ss << objPt->fillColor->g;
	    ss << " ";
	    ss << objPt->fillColor->b;
	    ss << " ";
	}
	ss << objPt->lineColor->r;
	ss << " ";
	ss << objPt->lineColor->g;
	ss << " ";
	ss << objPt->lineColor->b;
	ss << " ";
	linePt = objPt->firstLine;
	circPt = objPt->c;
	recPt = objPt->rec;
	elipPt = objPt->elip;

	if(objPt->group != NULL){
	    if(objPt->marked == false){
		ss << "GROUP < ";
		objPt->marked = true;    
	    }
	    else{
		ss << "GROUP > ";
		objPt->marked = false;
	    }
	}
	else{
	    if(linePt != NULL){
		while(linePt != NULL){
		    ss << "LINE ";
		    ss << linePt->v1.x;
		    ss << " "; 
		    ss << linePt->v1.y;
		    ss << " ";
		    ss << linePt->v2.x;
		    ss << " "; 
		    ss << linePt->v2.y;
		    ss << " ";
		    linePt = linePt->nextLine;
		}
	    }
	    else if(recPt != NULL){
		ss << "REC ";
		ss << recPt->v1.x;
		ss << " "; 
		ss << recPt->v1.y;
		ss << " "; 
		ss << recPt->v2.x;
		ss << " "; 
		ss << recPt->v2.y;
		ss << " "; 
		ss << recPt->v3.x;
		ss << " ";
		ss << recPt->v3.y;
		ss << " "; 
		ss << recPt->v4.x;
		ss << " ";
		ss << recPt->v4.y;
		ss << " ";
	    }
	    else if(circPt != NULL){
		ss << "CIRC ";
		ss << circPt->center.x; 
		ss << " ";
		ss << circPt->center.y; 
		ss << " ";
		ss << circPt->radius; 
		ss << " ";
	    }
	    else if(elipPt != NULL){
		ss << "ELIP ";
		ss << elipPt->center.x; 
		ss << " ";
		ss << elipPt->center.y; 
		ss << " ";
		ss << elipPt->rx; 
		ss << " ";
		ss << elipPt->ry; 
		ss << " ";
	    }
	}
	if(objPt->group != NULL && objPt->marked == true) objPt = objPt->group;
	else objPt = objPt->nextObj;
	ss << "ENDOBJ ";
    }
    std::string s(ss.str());
    save << s;
    save << "\nENDSAVE ";
    save.close();
}

void GLWidget::saveSVG(){
    ofstream save;
    obj* objPt;
    obj* groupObj;
    line* linePt;
    circle* circPt;
    elipse* elipPt;
    rectangle* recPt;

    save.open("save.svg");
    save << "<!DOCTYPE html>\n";
    save << "<html>\n";
    save << "<body>\n";
    save << "<svg height=\"" << screenH << "\" width=\"" << screenW << "\">\n";
    std::ostringstream ss;
    objPt = firstObj;

    while(objPt != NULL){
	linePt = objPt->firstLine;
	circPt = objPt->c;
	recPt = objPt->rec;
	elipPt = objPt->elip;

	if(objPt->group != NULL){
	    if(objPt->marked == false) objPt->marked = true;    
	    else objPt->marked = false;
	}
	else{
	    if(linePt != NULL){
		while(linePt != NULL){
		    if(linePt == objPt->firstLine) ss << "<polyline points=\"";
		    ss << linePt->v1.x;
		    ss << ","; 
		    ss << screenH - linePt->v1.y;
		    ss << " ";
		    if(linePt->nextLine == NULL){
			ss << linePt->v2.x;
			ss << ","; 
			ss << screenH - linePt->v2.y;
			ss << " ";
		    }
		    linePt = linePt->nextLine;
		}
		ss << "\" style=\"fill:none;stroke:rgb(";
		ss << (int)(objPt->lineColor->r * 255); 
		ss << ",";
		ss << (int)(objPt->lineColor->g * 255); 
		ss << ",";
		ss << (int)(objPt->lineColor->b * 255); 
		ss << ")\"/>\n";
	    }
	    else if(recPt != NULL){
		ss << "<rect x=\"";
		if(recPt->v1.x < recPt->v3.x) ss << recPt->v1.x;
		else ss << recPt->v3.x;
		ss << "\" y=\"";
		if(recPt->v1.y < recPt->v3.y) ss << screenH - recPt->v1.y;
		else ss << screenH - recPt->v3.y;
		ss << "\" width=\"";
		if(recPt->v1.x < recPt->v3.x) ss << recPt->v3.x - recPt->v1.x;
		else ss << recPt->v3.x - recPt->v1.x;
		ss << "\" height=\"";
		if(recPt->v1.y < recPt->v3.y) ss << recPt->v3.y - recPt->v1.y;
		else ss << recPt->v1.y - recPt->v3.y;
		ss << "\" style=\"fill:rgb(";
		if(objPt->fillColor == NULL) ss << "black";
		else{
		    ss << (int)(objPt->fillColor->r * 255); 
		    ss << ",";
		    ss << (int)(objPt->fillColor->g * 255); 
		    ss << ",";
		    ss << (int)(objPt->fillColor->b * 255); 
		}
		ss << ");stroke:rgb(";
		ss << (int)(objPt->lineColor->r * 255); 
		ss << ",";
		ss << (int)(objPt->lineColor->g * 255); 
		ss << ",";
		ss << (int)(objPt->lineColor->b * 255); 
		ss << ");fill-opacity:";
		if(objPt->fillColor == NULL) ss << "0";
		else ss << "1";
		ss << "\"/>\n";
	    }
	    else if(circPt != NULL){
		ss << "<circle cx=\"";
		ss << circPt->center.x;
		ss << "\" cy=\"";
		ss << screenH - circPt->center.y;
		ss << "\" r=\"";
		ss << circPt->radius;
		ss << "\" style=\"fill:rgb(";
		if(objPt->fillColor == NULL) ss << "black";
		else{
		    ss << (int)(objPt->fillColor->r * 255); 
		    ss << ",";
		    ss << (int)(objPt->fillColor->g * 255); 
		    ss << ",";
		    ss << (int)(objPt->fillColor->b * 255); 
		}
		ss << ");stroke:rgb(";
		ss << (int)(objPt->lineColor->r * 255); 
		ss << ",";
		ss << (int)(objPt->lineColor->g * 255); 
		ss << ",";
		ss << (int)(objPt->lineColor->b * 255); 
		ss << ");fill-opacity:";
		if(objPt->fillColor == NULL) ss << "0";
		else ss << "1";
		ss << "\"/>\n";

	    }
	    else if(elipPt != NULL){
		ss << "<ellipse cx=\"";
		ss << elipPt->center.x;
		ss << "\" cy=\"";
		ss << screenH - elipPt->center.y;
		ss << "\" rx=\"";
		ss << elipPt->rx;
		ss << "\" ry=\"";
		ss << elipPt->ry;
		ss << "\" style=\"fill:rgb(";
		if(objPt->fillColor == NULL) ss << "black";
		else{
		    ss << (int)(objPt->fillColor->r * 255); 
		    ss << ",";
		    ss << (int)(objPt->fillColor->g * 255); 
		    ss << ",";
		    ss << (int)(objPt->fillColor->b * 255); 
		}
		ss << ");stroke:rgb(";
		ss << (int)(objPt->lineColor->r * 255); 
		ss << ",";
		ss << (int)(objPt->lineColor->g * 255); 
		ss << ",";
		ss << (int)(objPt->lineColor->b * 255); 
		ss << ");fill-opacity:";
		if(objPt->fillColor == NULL) ss << "0";
		else ss << "1";
		ss << "\"/>\n";
	    }
	}
	if(objPt->group != NULL && objPt->marked == true) objPt = objPt->group;
	else objPt = objPt->nextObj;
    }
    std::string s(ss.str());
    save << s;
    save << "</svg>\n";
    save << "</body>\n";
    save << "</html>\n";
    save.close();
}

// Carrega todos os objetos de um arquivo
void GLWidget::load(){
    ifstream load;
    obj* objPt;
    obj* groupObj;
    line* linePt;
    circle* circPt;
    elipse* elipPt;
    rectangle* recPt;
    string s;
    float aux;
    
    load.open("save.elv");
    objPt = firstObj;
    delete objPt;
    objPt = lastObj;
    delete objPt;
    firstObj = NULL;
    lastObj = NULL;

    load >> s;
    if(s.compare("BEGINSAVE") == 0 && firstObj == NULL){
	load >> s;
	while(s.compare("BEGINOBJ") == 0){
	    if(firstObj == NULL){
		objPt = new obj();
		firstObj = objPt;
		lastObj = objPt;
	    }
	    else{
		objPt->nextObj = new obj();
		objPt->nextObj->previousObj = objPt;
		objPt = objPt->nextObj;
		lastObj = objPt;
	    }

	    load >> objPt->layer;
	    load >> aux;
	    if(aux != -1){
		objPt->fillColor = new color();
		objPt->fillColor->r = aux;
		load >> objPt->fillColor->g;
		load >> objPt->fillColor->b;
	    }
	    else load >> s >> s;
	    load >> objPt->lineColor->r;
	    load >> objPt->lineColor->g;
	    load >> objPt->lineColor->b;
	    load >> s;
	    while(s.compare("ENDOBJ") != 0){
		if(s.compare("LINE") == 0){
		    if(objPt->firstLine == NULL){
			objPt->firstLine = new line();
			objPt->lastLine = objPt->firstLine;
		    }
		    else{
			objPt->lastLine->nextLine = new line();
			objPt->lastLine = objPt->lastLine->nextLine;
		    }
		    linePt = objPt->lastLine;
		    load >> linePt->v1.x;
		    load >> linePt->v1.y;
		    load >> linePt->v2.x;
		    load >> linePt->v2.y;

		}
		else if(s.compare("REC") == 0){
		    objPt->rec = new rectangle();
		    recPt = objPt->rec;
		    load >> recPt->v1.x;
		    load >> recPt->v1.y;
		    load >> recPt->v2.x;
		    load >> recPt->v2.y;
		    load >> recPt->v3.x;
		    load >> recPt->v3.y;
		    load >> recPt->v4.x;
		    load >> recPt->v4.y;
		}
		else if(s.compare("CIRC") == 0){
		    objPt->c = new circle();
		    circPt = objPt->c;
		    load >> circPt->center.x;
		    load >> circPt->center.y;
		    load >> circPt->radius;
		}
		else if(s.compare("ELIP") == 0){
		    objPt->elip = new elipse();
		    elipPt = objPt->elip;
		    load >> elipPt->center.x;
		    load >> elipPt->center.y;
		    load >> elipPt->rx;
		    load >> elipPt->ry;
		}
		load >> s;
	    }
	    load >> s;
	    cout << s << "\n";
	}
	printf("\nEND OF LOADING\n");
	updateGL();
    }
}

/**
 * Limpa todos os marcadores
 */
void GLWidget::clearMarkers(){
    markedLine = NULL;
    markedLine2 = NULL;
    markedObj = NULL;
}

/**
 * Deleta o objeto ou linha selecionada
 */
void GLWidget::delSelected(){
    line* linePt;
    // Remoção de linha
    if(markedLine != NULL){
	if(markedLine->previousLine == NULL && markedLine->nextLine == NULL){
	    markedObj = markedLine->top;
	}
	else if(markedLine->previousLine == NULL){
	    markedLine->nextLine->previousLine = NULL;
	    markedLine->nextLine->v1.x = markedLine->v1.x;
	    markedLine->nextLine->v1.y = markedLine->v1.y;
	    markedLine->top->firstLine = markedLine->nextLine;
	}
	else if(markedLine->nextLine == NULL){
	    markedLine->previousLine->nextLine = NULL;
	    markedLine->previousLine->v2.x = markedLine->v2.x;
	    markedLine->previousLine->v2.y = markedLine->v2.y;
	    markedLine->top->lastLine = markedLine->previousLine;
	}
	else{
	    markedLine->previousLine->nextLine = markedLine->nextLine;
	    markedLine->nextLine->previousLine = markedLine->previousLine;
	    markedLine->previousLine->v2.x = (markedLine->v1.x + markedLine->v2.x) / 2;
	    markedLine->previousLine->v2.y = (markedLine->v1.y + markedLine->v2.y) / 2;
	    markedLine->nextLine->v1.x = (markedLine->v1.x + markedLine->v2.x) / 2;
	    markedLine->nextLine->v1.y = (markedLine->v1.y + markedLine->v2.y) / 2;
	}
	delete markedLine;
    }
    // Remoção de objeto
    else if(markedObj != NULL){
	if(markedObj->firstLine != NULL){
	    linePt = markedObj->lastLine;
	    while(linePt != markedObj->firstLine){
		linePt = linePt->previousLine;
		delete linePt->nextLine;
	    }
	    delete linePt;
	}

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
	delete markedObj;
	markedObj = NULL;
    }
    clearMarkers();
}

/**
 * Cria um novo agrupamento
 */
void GLWidget::createGroup(){
    bool newGroup;
    obj* objPt;
    obj* objPt2;
    obj* groupObj;
    line* linePt;
    circle* circPt;
    elipse* elipPt;
    rectangle* recPt;

    newGroup = false;
    objPt = firstObj;

    while(objPt != NULL){
	objPt2 = objPt->nextObj;
	if(objPt->marked == true){
	    objPt->marked = false;
	    if(newGroup == false){
		newGroup = true;
		lastObj->nextObj = new obj();
		lastObj->nextObj->previousObj = lastObj;
		lastObj = lastObj->nextObj;
		lastObj->rec = new rectangle();
		lastObj->group = objPt;
		if(objPt->nextObj != NULL) objPt->nextObj->previousObj = objPt->previousObj;
		if(objPt->previousObj != NULL) objPt->previousObj->nextObj = objPt->nextObj;
		else firstObj = objPt->nextObj;
		objPt->previousObj = lastObj;
		objPt->nextObj = lastObj;
		lastObj->endGroup = objPt;
	    }
	    else{
		if(objPt->nextObj != NULL) objPt->nextObj->previousObj = objPt->previousObj;
		if(objPt->previousObj != NULL) objPt->previousObj->nextObj = objPt->nextObj;
		else firstObj = objPt->nextObj;
		markedObj->nextObj = objPt;
		objPt->previousObj = markedObj;
		objPt->nextObj = lastObj;
		lastObj->endGroup = objPt;
	    }
	    markedObj = objPt;
	}
	objPt = objPt2;
    }
    // Cálculo do tamanho do grupo
    if(newGroup == true){
	lastObj->rec->v1.x = screenW;
	lastObj->rec->v1.y = screenH;
	lastObj->rec->v3.x = 40;
	lastObj->rec->v3.y = 40;
	objPt = lastObj->group;
	while(objPt != lastObj){
	    // Tamanho das polilinhas
	    if(objPt->firstLine != NULL){
		linePt = objPt->firstLine;
		while(linePt != NULL){
		    if(linePt->v1.x < linePt->v2.x){
			if(lastObj->rec->v1.x > linePt->v1.x) lastObj->rec->v1.x = linePt->v1.x;
			if(lastObj->rec->v3.x < linePt->v2.x) lastObj->rec->v3.x = linePt->v2.x;
		    }
		    else{
			if(lastObj->rec->v1.x > linePt->v2.x) lastObj->rec->v1.x = linePt->v2.x;
			if(lastObj->rec->v3.x < linePt->v1.x) lastObj->rec->v3.x = linePt->v1.x;
		    }
		    if(linePt->v1.y < linePt->v2.y){
			if(lastObj->rec->v1.y > linePt->v1.y) lastObj->rec->v1.y = linePt->v1.y;
			if(lastObj->rec->v3.y < linePt->v2.y) lastObj->rec->v3.y = linePt->v2.y;
		    }
		    else{
			if(lastObj->rec->v1.y > linePt->v2.y) lastObj->rec->v1.y = linePt->v2.y;
			if(lastObj->rec->v3.y < linePt->v1.y) lastObj->rec->v3.y = linePt->v1.y;
		    }
		    linePt = linePt->nextLine;
		}
	    }
	    // Tamanho dos retângulos e agrupamentos
	    else if(objPt->rec != NULL){
		recPt = objPt->rec;
		if(recPt->v1.x < recPt->v3.x){
		    if(lastObj->rec->v1.x > recPt->v1.x) lastObj->rec->v1.x = recPt->v1.x;
		    if(lastObj->rec->v3.x < recPt->v3.x) lastObj->rec->v3.x = recPt->v3.x;
		}
		else{
		    if(lastObj->rec->v1.x > recPt->v3.x) lastObj->rec->v1.x = recPt->v3.x;
		    if(lastObj->rec->v3.x < recPt->v1.x) lastObj->rec->v3.x = recPt->v1.x;
		}
		if(recPt->v1.y < recPt->v3.y){
		    if(lastObj->rec->v1.y > recPt->v1.y) lastObj->rec->v1.y = recPt->v1.y;
		    if(lastObj->rec->v3.y < recPt->v3.y) lastObj->rec->v3.y = recPt->v3.y;
		}
		else{
		    if(lastObj->rec->v1.y > recPt->v3.y) lastObj->rec->v1.y = recPt->v3.y;
		    if(lastObj->rec->v3.y < recPt->v1.y) lastObj->rec->v3.y = recPt->v1.y;
		}
	    }
	    // Tamanho das circunferências
	    else if(objPt->c != NULL){
		circPt = objPt->c;
		if(lastObj->rec->v1.x > circPt->center.x - circPt->radius)
		    lastObj->rec->v1.x = circPt->center.x - circPt->radius;
		if(lastObj->rec->v1.y > circPt->center.y - circPt->radius)
		    lastObj->rec->v1.y = circPt->center.y - circPt->radius;
		if(lastObj->rec->v3.x < circPt->center.x + circPt->radius)
		    lastObj->rec->v3.x = circPt->center.x + circPt->radius;
		if(lastObj->rec->v3.y < circPt->center.y + circPt->radius)
		    lastObj->rec->v3.y = circPt->center.y + circPt->radius;
	    }
	    objPt = objPt->nextObj;

	}
	lastObj->rec->v2.x = lastObj->rec->v1.x;
	lastObj->rec->v2.y = lastObj->rec->v3.y;
	lastObj->rec->v4.x = lastObj->rec->v3.x;
	lastObj->rec->v4.y = lastObj->rec->v1.y;
    }
    clearMarkers();
}

/**
 * Desfaz um agrupamento
 */
void GLWidget::undoGroup(){
    obj* objPt;
    if(markedObj != NULL){
	if(markedObj->group != NULL){
	    markedObj->group->previousObj = markedObj->previousObj;
	    if(markedObj != firstObj) markedObj->previousObj->nextObj = markedObj->group;
	    else firstObj = markedObj->group;
	    markedObj->endGroup->nextObj = markedObj->nextObj;
	    if(markedObj != lastObj) markedObj->nextObj->previousObj = markedObj->endGroup;
	    else lastObj = markedObj->endGroup;


	    objPt = markedObj->endGroup;
	    markedObj = markedObj->group;
	    while(markedObj != objPt){
		//decreaseLayer(0);
		markedObj = markedObj->nextObj;
	    }
	}
    }
    clearMarkers();
}

void GLWidget::mouseClick(){
    emit mouseClicked();
}

void GLWidget::mouseMovement(){
    emit mouseMoved();
}

/**
 * Função de debug, mostra todos os objetos em uma lista no terminal
 */
void GLWidget::objDebug(){
    obj* objPt;
    int count;

    count = 0;
    objPt = firstObj;

    printf("\n");
    while(objPt != NULL){
	printf("%i, %i: ", count, objPt->layer);
	if(objPt->marked == true) printf("TRUE ");
	else printf("FALSE ");

	if(objPt->group != NULL){
	    if(objPt->marked == false){
		printf("group < ");
		printf("%i-%i %i-%i %i-%i %i-%i ", objPt->rec->v1.x, objPt->rec->v1.y,
			objPt->rec->v2.x ,objPt->rec->v2.y,
			objPt->rec->v3.x, objPt->rec->v3.y,
			objPt->rec->v4.x, objPt->rec->v4.y);
		objPt->marked = true;
	    }
	    else{
		printf("group > ");
		objPt->marked = false;
	    }
	}
	else if(objPt->firstLine != NULL) printf("line ");
	else if(objPt->rec != NULL){
	    printf("rec %i-%i %i-%i %i-%i %i-%i ", objPt->rec->v1.x, objPt->rec->v1.y,
		    objPt->rec->v2.x ,objPt->rec->v2.y,
		    objPt->rec->v3.x, objPt->rec->v3.y,
		    objPt->rec->v4.x, objPt->rec->v4.y);
	}
	else if(objPt->c != NULL) printf("circle ");
	else if(objPt->elip != NULL) printf("ellipse ");
	if(objPt == firstObj) printf("FIRSTOBJ ");
	if(objPt == lastObj) printf("LASTOBJ ");
	if(objPt == markedObj) printf("MARKEDOBJ ");
	if(objPt->group != NULL && objPt->marked == true) objPt = objPt->group;
	else objPt = objPt->nextObj;
	printf("\n");
	count++;
    }
}

/**
 * Transfere o objeto para uma camada superior
 */
void GLWidget::increaseLayer(int incValue){
    obj* objPt;
    if(markedObj != NULL){

	markedObj->layer += incValue;
	objPt = markedObj->nextObj;
	while(objPt != NULL && (objPt->layer <= markedObj->layer))
	    objPt = objPt->nextObj;
	if(objPt != markedObj->nextObj){
	    if(markedObj == firstObj) firstObj = markedObj->nextObj;		
	    else markedObj->previousObj->nextObj = markedObj->nextObj;
	    markedObj->nextObj->previousObj = markedObj->previousObj;
	    if(objPt == NULL){
		markedObj->previousObj = lastObj;
		markedObj->previousObj->nextObj = markedObj;
		markedObj->nextObj = NULL;
		lastObj = markedObj;
	    }
	    else{
		markedObj->previousObj = objPt->previousObj;
		markedObj->nextObj = objPt;
		markedObj->nextObj->previousObj = markedObj;
		markedObj->previousObj->nextObj = markedObj;
	    }
	}
    }
    clearMarkers();
}

/**
 * Transfere o objeto para uma camada inferior 
 */
void GLWidget::decreaseLayer(int decValue){
    obj* objPt;

    if(markedObj != NULL){
	markedObj->layer -= decValue;
	if(markedObj->layer < 0) markedObj->layer = 0;
	objPt = markedObj->previousObj;
	while(objPt != NULL && (objPt->layer > markedObj->layer))
	    objPt = objPt->previousObj;
	if(objPt != markedObj->previousObj){
	    if(markedObj == lastObj) lastObj = markedObj->previousObj;		
	    else markedObj->nextObj->previousObj = markedObj->previousObj;
	    markedObj->previousObj->nextObj = markedObj->nextObj;
	    objDebug();
	    if(objPt == NULL){
		markedObj->nextObj = lastObj;
		markedObj->nextObj->previousObj = markedObj;
		markedObj->previousObj = NULL;
		firstObj = markedObj;
	    }
	    else{
		markedObj->nextObj = objPt->nextObj;
		markedObj->previousObj = objPt;
		markedObj->previousObj->nextObj = markedObj;
		markedObj->nextObj->previousObj = markedObj;
	    }
	}
    }
    clearMarkers();
}

/**
 * Função de captura de botões pressionados
 */
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
	    break;
	case Qt::Key_G:
	    if(grid == true) grid = false;
	    else grid = true;
	    updateGL();
	    break;
	case Qt::Key_I:
	    zoom += 0.1;
	    updateGL();
	    break;
	case Qt::Key_O:
	    if(zoom >= 0.3) zoom -= 0.1;
	    updateGL();
	    break;
	case Qt::Key_5:
	    OPTION = 20;
	    updateGL();
	    break;
	case Qt::Key_0:
	    if(OPTION == 10){
		OPTION = 1;
		createGroup();
	    }
	    else OPTION = 10;
	    updateGL();
	    break;
	case Qt::Key_U:
	    undoGroup();
	    updateGL();
	    break;
	case Qt::Key_D:
	    objDebug();
	    break;
	case Qt::Key_L:
	    load();
	    break;
	case Qt::Key_Q:
	    decreaseLayer(1);
	    updateGL();
	    break;
	case Qt::Key_W:
	    increaseLayer(1);
	    updateGL();
	    break;
	case Qt::Key_V:
	    saveSVG();
	    updateGL();
	    break;
	case Qt::Key_C:
	    printf("COPY\n");
	    OPTION = 9;
	    cp=true;
	    click=false;
	    break;
	case Qt::Key_S:
	    save();
	    break;
	default:
	    event->ignore();
	    break;
    }
}
