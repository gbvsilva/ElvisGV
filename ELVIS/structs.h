#ifndef STRUCTS_H
#define STRUCTS_H
// Vértice, linha, objeto, respectivamente
struct vertex{
    int x;
    int y;
};

struct line{
    line* next;
    vertex v1, v2;
    line() {
        next=NULL;
    }
};

struct circle{
    int radius;
    vertex center;
    circle() {
        radius=10.0;
    }
};

struct obj{
    obj *next;
    line* firstLine;
    line* lastLine;
    circle* c;
    obj() {
        next=NULL;
        firstLine=NULL;
        lastLine=NULL;
        c=NULL;
    }
};
#endif
