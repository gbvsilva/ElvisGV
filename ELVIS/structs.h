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
    line(){
        next = NULL;
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
    obj* next;
    line* firstLine;
    line* lastLine;
    circle* c;
    int type;
    int layer;
    float r;
    float g;
    float b;
    obj(){
        next = NULL;
        firstLine = NULL;
        lastLine = NULL;
        c=NULL;
        r = 0;
        g = 0;
        b = 0;
    }
};
#endif
