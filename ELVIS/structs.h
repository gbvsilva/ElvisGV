#ifndef STRUCTS_H
#define STRUCTS_H
// Vértice, linha, objeto, respectivamente
struct vertex{
    int x;
    int y;
};

struct obj;

struct line{
    obj* top;
    line* nextLine;
    line* previousLine;
    bool marked;
    vertex v1, v2;
    line(){
        nextLine = NULL;
        previousLine = NULL;
	marked = false;
    }
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

struct obj{
    obj* nextObj;
    obj* previousObj;
    line* firstLine;
    line* lastLine;
    circle* c;
    elipse* elip;
    float r;
    float g;
    float b;
    bool marked;
    obj(){
        nextObj = NULL;
        previousObj = NULL;
        firstLine = NULL;
        lastLine = NULL;
        c=NULL;
        elip=NULL;
        r = 0;
        g = 0;
        b = 0;
	marked = false;
    }
};
#endif
