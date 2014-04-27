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
    line(){
	next = NULL;
    };
};
struct obj{
    obj* next;
    line* firstLine;
    line* lastLine;
    int type;
    int layer;
    float r;
    float g;
    float b;
    obj(){
	next = NULL;
	firstLine = NULL;
	lastLine = NULL;
	r = 0;
	g = 0;
	b = 0;
    };
};
#endif
