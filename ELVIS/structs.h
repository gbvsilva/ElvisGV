#ifndef STRUCTS_H
#define STRUCTS_H
// Vértice, linha, objeto, respectivamente
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
    line* lastLine = NULL;
};
#endif
