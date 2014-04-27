#ifndef STRUCTS_H
#define STRUCTS_H
// Vértice, linha, objeto, respectivamente
struct vertex{
    float x;
    float y;
};

struct line{
    line* next;
    vertex v1, v2;
    line() {
        next=NULL;
    }
};

struct obj{
    obj *next;
    line* firstLine;
    line* lastLine;
    obj() {
        next=NULL;
        firstLine=NULL;
        lastLine=NULL;
    }
};
#endif
