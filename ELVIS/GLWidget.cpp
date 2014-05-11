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
bool grid = false;
// Posição anterior e atual que o mouse clicou, respectivamente
int pos1X = 0;
int pos1Y = 0;
int pos2X = 0;
int pos2Y = 0;
int panX = 0;
int panY = 0;
int auxpanX = 0;
int auxpanY = 0;
int screenH, screenW;
float zoom = 1;
// Clique inicial
bool click = false;
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
    screenH = h;
    screenW = w;
}

void GLWidget::drawSquareMarker(int x, int y, int size){
    glColor3f(1, 0, 0);
    bresenham(x - size, y - size, x + size, y - size );    
    bresenham(x + size, y - size, x + size, y + size );    
    bresenham(x + size, y + size, x - size, y + size );    
    bresenham(x - size, y + size, x - size, y - size );
}

void GLWidget::drawSelSquareMarker(int x, int y, int size){
    glColor3f(0, 0, 0);
    bresenham(x - size, y - size, x + size, y - size );    
    bresenham(x + size, y - size, x + size, y + size );    
    bresenham(x + size, y + size, x - size, y + size );    
    bresenham(x - size, y + size, x - size, y - size );
    glColor3f(1, 0, 0);
    for(int i = -size + 1; i <= size - 1; i++){
	bresenham(x - size + 1, y + i, x + size, y + i);
    }
}

void GLWidget::paintGL() {
    obj* objPt;
    obj* groupObj;
    line* linePt;
    circle* circPt;
    elipse* elipPt;
    rectangle* recPt;
    int gridSize; // Tamanho da grade

    gridSize = 40;
    objPt = firstObj;
    groupObj = NULL;

    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);

    //Desenho da grade
    if(grid){
	/*
	glColor3f(0.9, 0.9, 0.9);
	for(int i = -gridSize; i < (screenW+gridSize)/zoom; i+= gridSize)
	    bresenham((i + gridSize/2 - panX % gridSize)*zoom, 0, (i + gridSize/2 - panX % gridSize)*zoom, (screenH)/zoom);
	for(int i = -gridSize; i < screenH+gridSize; i+= gridSize)
	    bresenham(0, (i+gridSize/2 - panY % gridSize)*zoom, (screenW)/zoom, (i+gridSize/2 - panY % gridSize)*zoom);

	glColor3f(0.7, 0.7, 0.7);
	for(int i = -gridSize; i < (screenW+gridSize)/zoom; i+= gridSize)
	    bresenham((i - panX % gridSize)*zoom, 0, (i - panX % gridSize)*zoom, (screenH)/zoom);
	for(int i = -gridSize; i < screenH+gridSize; i+= gridSize)
	    bresenham(0, (i - panY % gridSize)*zoom, (screenW)/zoom, (i - panY % gridSize)*zoom);
	*/
	
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
    // Desenho dos objetos
    while(objPt != NULL){
	linePt = objPt->firstLine;
	circPt = objPt->c;
	elipPt = objPt->elip;
	recPt = objPt->rec;
	// Caso o objeto seja um grupo
	if(objPt->group != NULL){
	    // Primeira passagem, desenhar todos os objetos do conjunto
	    if(objPt->marked == false){
		objPt->marked = true;
		if(groupObj == NULL) groupObj = objPt;
		// TO-DO: DESENHAR VÉRTICES DO RETÂNGULO
		objPt = objPt->group;
	    }
	    // Segunda passagem, desenhar o objeto seguinte
	    else{
		if(groupObj == objPt){
		    groupObj = NULL;
		    /*
		    if((markedObj != NULL && markedObj == objPt) ||
			    (markedLine != NULL && markedLine == linePt) ||
			    (markedLine != NULL && markedLine == linePt->previousLine) ||
			    (objPt->marked == true)){
			drawSelSquareMarker((objPt->rec->v1.x - panX)*zoom, (objPt->rec->v1.y - panY)*zoom, 5);
			drawSelSquareMarker((objPt->rec->v2.x - panX)*zoom, (objPt->rec->v2.y - panY)*zoom, 5);
			drawSelSquareMarker((objPt->rec->v3.x - panX)*zoom, (objPt->rec->v3.y - panY)*zoom, 5);
			drawSelSquareMarker((objPt->rec->v4.x - panX)*zoom, (objPt->rec->v4.y - panY)*zoom, 5);
		    }
		    // Caso ela não esteja em um grupo, marcação normal
		    else if(groupObj == NULL){
			drawSquareMarker((objPt->rec->v1.x - panX)*zoom, (objPt->rec->v1.y - panY)*zoom, 5);
			drawSquareMarker((objPt->rec->v2.x - panX)*zoom, (objPt->rec->v2.y - panY)*zoom, 5);
			drawSquareMarker((objPt->rec->v3.x - panX)*zoom, (objPt->rec->v3.y - panY)*zoom, 5);
			drawSquareMarker((objPt->rec->v4.x - panX)*zoom, (objPt->rec->v4.y - panY)*zoom, 5);
		    }
		    */
		}
		objPt->marked = false;
		objPt = objPt->nextObj;
	    }
	}
	// Caso contrário
	else{
	    // Desenhando polilinhas
	    if(linePt != NULL) {
		linePt = objPt->firstLine;
		while(linePt != NULL) {
		    // Desenho de uma linha
		    glColor3f(objPt->lineColor->r, objPt->lineColor->g, objPt->lineColor->b);
		    bresenham((linePt->v1.x - panX)*zoom, (linePt->v1.y - panY)*zoom,
			    (linePt->v2.x - panX)*zoom, (linePt->v2.y - panY)*zoom);
		    // Caso a linha ou objeto esteja selecionado , usar marcação especial
		    if((markedObj != NULL && markedObj == objPt) ||
			    (markedLine != NULL && markedLine == linePt) ||
			    (markedLine != NULL && markedLine == linePt->previousLine) ||
			    (objPt->marked == true)){
			drawSelSquareMarker((linePt->v1.x - panX)*zoom, (linePt->v1.y - panY)*zoom, 5);
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
			    (objPt->lastLine->v2.y - panY)*zoom, 5);
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
		bresenham((recPt->v4.x - panX)*zoom, (recPt->v4.y - panY)*zoom, (recPt->v1.x - panX)*zoom, (recPt->v1.y - panY)*zoom);
		// Caso esteja selecionado, marcação especial
		if(markedObj != NULL && markedObj == objPt){
		    drawSelSquareMarker((recPt->v1.x - panX)*zoom, (recPt->v1.y - panY)*zoom, 5);	
		    drawSelSquareMarker((recPt->v2.x - panX)*zoom, (recPt->v2.y - panY)*zoom, 5);	
		    drawSelSquareMarker((recPt->v3.x - panX)*zoom, (recPt->v3.y - panY)*zoom, 5);	
		    drawSelSquareMarker((recPt->v4.x - panX)*zoom, (recPt->v4.y - panY)*zoom, 5);
		}
		// Caso contrário
		else{
		    drawSquareMarker((recPt->v1.x - panX)*zoom, (recPt->v1.y - panY)*zoom, 5);	
		    drawSquareMarker((recPt->v2.x - panX)*zoom, (recPt->v2.y - panY)*zoom, 5);	
		    drawSquareMarker((recPt->v3.x - panX)*zoom, (recPt->v3.y - panY)*zoom, 5);	
		    drawSquareMarker((recPt->v4.x - panX)*zoom, (recPt->v4.y - panY)*zoom, 5);
		}
	    }
	    // Desenho de circunferência
	    else if(circPt != NULL) {
		glColor3f(objPt->lineColor->r, objPt->lineColor->g, objPt->lineColor->b);
		midPtCircle((circPt->center.x - panX)*zoom, (circPt->center.y - panY)*zoom, (circPt->radius)*zoom);
		if(markedObj != NULL && markedObj == objPt){
		    drawSelSquareMarker((circPt->center.x - panX)*zoom, (circPt->center.y - panY)*zoom, 5);
		    drawSelSquareMarker((circPt->center.x - panX + circPt->radius)*zoom, (circPt->center.y - panY)*zoom,  5);
		}
		// Caso contrário
		else{
		    drawSquareMarker((circPt->center.x - panX)*zoom, (circPt->center.y - panY)*zoom, 5);
		    drawSquareMarker((circPt->center.x - panX + circPt->radius)*zoom, (circPt->center.y - panY)*zoom,  5);
		}
	    }
	    // Desenho de elipse
	    else if(elipPt != NULL) {
		glColor3f(objPt->lineColor->r, objPt->lineColor->g, objPt->lineColor->b);
		midPtElipse((elipPt->center.x - panX)*zoom, (elipPt->center.y - panY)*zoom, (elipPt->rx - panX)*zoom, (elipPt->ry - panY)*zoom);
	    }
	    objPt = objPt->nextObj;
	}
    }
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
    mouseClick();
    obj* objPt;
    line* linePt;
    // Criação de polilinha
    if(OPTION == 1) {
	clearMarkers();
	if(event->button() == Qt::LeftButton) {
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

	    lastObj->lastLine->v1.x = pos1X;
	    lastObj->lastLine->v1.y = pos1Y;
	    lastObj->lastLine->v2.x = pos2X;
	    lastObj->lastLine->v2.y = pos2Y;
	    updateGL();
	}
	else if(event->button() == Qt::MiddleButton) click = false;
    }
    /* Desenhar CIRCULO */
    else if(OPTION==2) {
	clearMarkers();
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
	else {
	    pos2X = event->x()/zoom + panX;
	    pos2Y = (mouseH - event->y())/zoom + panY;
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
	    }else {
		pos2X = event->x()/zoom + panX;
		pos2Y = (mouseH - event->y())/zoom + panY;
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
	updateGL();
    }
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
	updateGL();
    }
    // Seleção de linha
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
    // Seleção de objeto
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
		if(foundLine == true){
		    // Seleção para agrupamento
		    if(OPTION == 10){
			if(objPt->marked == false) objPt->marked = true;
			else objPt->marked = false;
		    }
		    else markedObj = objPt;
		}
		linePt = linePt->nextLine;
	    }
	    // Seleção da circunferência
	    else if(objPt->c != NULL){
		posDist = sqrt(pow((abs(objPt->c->center.x - pos1X)), 2) +
			pow((abs(objPt->c->center.y - pos1Y)), 2));
		if(objPt->c->radius > posDist - clipSize &&
			objPt->c->radius < posDist + clipSize) markedObj = objPt;
	    }
	    // Seleção de retângulo
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
	    objPt = objPt->nextObj;
	}
	updateGL();
    }
    //Screen pan
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
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    mouseMovement();
    pos2X = event->x()/zoom + panX;
    pos2Y = (mouseH - event->y())/zoom + panY;
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
	    markedLine2->v1.x = pos2X;
	    markedLine2->v1.y = pos2Y;
	    markedLine2->previousLine->v2.x = pos2X;
	    markedLine2->previousLine->v2.y = pos2Y;
	}
	else if(OPTION == 20){
	    if(click == true){
		pos2X = event->x()/zoom;
		pos2Y = (mouseH - event->y())/zoom;
		panX = auxpanX + pos1X - pos2X;
		panY = auxpanY + pos1Y - pos2Y;
	    }
	}
	updateGL();
    }
}

void GLWidget::clearMouse(){
    click = false;
    pos1X = 0;
    pos1Y = 0;
    pos2X = 0;
    pos2Y = 0;
}

void GLWidget::clearMarkers(){
    markedLine = NULL;
    markedLine2 = NULL;
    markedObj = NULL;
}

void GLWidget::delSelected(){
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
	lastObj->rec->v3.x = 0;
	lastObj->rec->v3.x = 0;
	objPt = lastObj->group;
	while(objPt != lastObj){
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
	    else if(objPt->rec != NULL){
		recPt = objPt->rec;
		if(recPt->v1.x < recPt->v2.x){
		    if(objPt->rec->v1.x > recPt->v1.x) objPt->rec->v1.x = recPt->v1.x;
		    if(objPt->rec->v3.x < recPt->v3.x) objPt->rec->v3.x = recPt->v3.x;
		}
		else{
		    if(objPt->rec->v1.x > recPt->v3.x) objPt->rec->v1.x = recPt->v3.x;
		    if(objPt->rec->v3.x < recPt->v1.x) objPt->rec->v3.x = recPt->v1.x;
		}
		if(recPt->v1.y < recPt->v2.y){
		    if(objPt->rec->v1.y > recPt->v1.y) objPt->rec->v1.y = recPt->v1.y;
		    if(objPt->rec->v3.y < recPt->v3.y) objPt->rec->v3.y = recPt->v3.y;
		}
		else{
		    if(objPt->rec->v1.y > recPt->v3.y) objPt->rec->v1.y = recPt->v3.y;
		    if(objPt->rec->v3.y < recPt->v1.y) objPt->rec->v3.y = recPt->v1.y;
		}
	    }
	    objPt = objPt->nextObj;
	}
    }
    //*/
    clearMarkers();
}

void undoGroup(){
}

void GLWidget::mouseClick(){
    emit mouseClicked();
}

void GLWidget::mouseMovement(){
    emit mouseMoved();
}

void objDebug(){
    obj* objPt;
    int count;
    
    count = 0;
    objPt = firstObj;

    printf("\n"); 
    while(objPt != NULL){
	printf("%i: ", count);
	if(objPt->marked == true) printf("TRUE ");
	else printf("FALSE ");
	
	if(objPt->group != NULL){
	    if(objPt->marked == false){
		printf("group < ");
		objPt->marked = true;
	    }
	    else{
		printf("group > ");
		objPt->marked = false;
	    }
	}
	else if(objPt->firstLine != NULL) printf("line ");
	else if(objPt->rec != NULL){
	    printf("rec %i-%i %i-%i %i-%i %i-%i ", objPt->rec->v1.x, objPt->rec->v1.y, objPt->rec->v2.x ,objPt->rec->v2.y, objPt->rec->v3.x, objPt->rec->v3.y, objPt->rec->v4.x, objPt->rec->v4.y);
	}
	else if(objPt->c != NULL) printf("circle ");
	else if(objPt->elip != NULL) printf("ellipse ");
	if(objPt == firstObj) printf("FIRSTOBJ ");
	else if(objPt == lastObj) printf("LASTOBJ ");
	if(objPt->group != NULL && objPt->marked == true) objPt = objPt->group;
	else objPt = objPt->nextObj;
	printf("\n");
	count++;
    }
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
	    break;
	case Qt::Key_D:
	    objDebug();
	    break;
	default:
	    event->ignore();
	    break;
    }
}
