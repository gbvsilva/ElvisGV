#ifndef STRUCTS_H
#define STRUCTS_H
// Vértice, linha, objeto, respectivamente
struct obj;

struct vertex{
    int x;
    int y;
};

struct line{
    obj* top;
    line* nextLine;
    line* previousLine;
    vertex v1, v2;
    line(){
	nextLine = NULL;
	previousLine = NULL;
    }
};

struct rectangle{
    vertex v1;
    vertex v2;
    vertex v3;
    vertex v4;
};

struct circle{
    int radius;
    vertex center;
    circle() {
	radius=10.0;
    }
};

struct elipse {
    int rx, ry;
    vertex center;
    elipse() {
	rx=ry=0;
    }
};

struct color{
    float r;
    float g;
    float b;
    color(){
	r = 0;
	g = 0;
	b = 0;
    }
};

struct obj{
    // Fazem com que a lista seja encadeada
    obj* nextObj;
    obj* previousObj;
    // Definem o subtipo do objeto
    vertex v;
    line* firstLine;
    line* lastLine;
    circle* c;
    elipse* elip;
    rectangle* rec;
    // Cores do objeto
    color* fillColor;
    color* lineColor;
    // Utilizados apenas se o objeto é um grupo
    obj* group;
    obj* endGroup;
    // Marcadores
    bool marked;// Seleção múltipla
    bool mainGroup;// Marcação do grupo principal
    // Layer
    int layer;
    obj(){
	nextObj = NULL;
	previousObj = NULL;
	firstLine = NULL;
	lastLine = NULL;
	c = NULL;
	elip = NULL;
	rec = NULL;
	fillColor = NULL;
	group = NULL;
	endGroup = NULL;
	lineColor = new color();
	fillColor = NULL;
	marked = false;
	mainGroup = false;
	layer = 0;
    }
};
#endif
