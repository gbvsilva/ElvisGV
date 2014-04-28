#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glu.h>
#include "structs.h"
#include "drawFunctions.h"

void bresenham( int x1, int y1, int x2, int y2 ) {
    vertex p1, p2;
    long dist;
    int i, varX, varY, x, y, pk;
    p1.x = round( x1 );
    p1.y = round( y1 );
    p2.x = round( x2 );
    p2.y = round( y2 );
    // Casos triviais
    // 0°
    if(p1.y == p2.y){
        dist = p2.x - p1.x;
        if(dist > 0){
            printf("<Case 1.a>\n");
            for(i = 0; i < dist; i++){
                glBegin( GL_POINTS );
                glVertex2i( p1.x+i, p1.y );
                glEnd( );
            }
        }
        else{
            printf("<Case 1.b>\n");
            dist *= -1;
            for(i = 0; i < dist; i++){
                glBegin( GL_POINTS );
                glVertex2i( p2.x-i, p1.y );
                glEnd( );
            }
        }
    }
    // 90°
    else if(p1.x == p2.x){
        dist = p2.y - p1.y;
        if(dist > 0){
            printf("<Case 2.a>\n");
            for(i = 0; i < dist; i++){
                glBegin( GL_POINTS );
                glVertex2i( p1.x, p1.y+i );
                glEnd( );
            }
        }
        else{
            printf("<Case 2.b>\n");
            dist *= -1;
            for(i = 0; i < dist; i++){
                glBegin( GL_POINTS );
                glVertex2i( p2.x, p1.y-i );
                glEnd( );
            }
        }
    }
    // 45°
    else if((p2.x-p1.x) == (p2.y - p1.y)){
        dist = p2.x - p1.x;
        if(dist > 0){
            printf("<Case 3.a>\n");
            for(i = 0; i < dist; i++){
                glBegin( GL_POINTS );
                glVertex2i( p1.x+i, p1.y+i );
                glEnd( );
            }
        }
        else{
            printf("<Case 3.b>\n");
            dist *= -1;
            for(i = 0; i < dist; i++){
                glBegin( GL_POINTS );
                glVertex2i( p2.x-i, p1.y-i );
                glEnd( );
            }
        }
    }
    // 125°
    else if((p2.x-p1.x) == -1*(p2.y - p1.y)){
        dist = p2.x - p1.x;
        if(dist > 0){
            printf("<Case 4.a>\n");
            for(i = 0; i < dist; i++){
                glBegin( GL_POINTS );
                glVertex2i( p1.x+i, p1.y-i );
                glEnd( );
            }
        }
        else{
            printf("<Case 4.b>\n");
            dist *= -1;
            for(i = 0; i < dist; i++){
                glBegin( GL_POINTS );
                glVertex2i( p2.x-i, p1.y+i );
                glEnd( );
            }
        }
    }
    //Casos não triviais
    else{
        varX = p2.x - p1.x;
        varY = p2.y - p1.y;
        // Entre -45° e 45°
        if(abs(varX) > abs(varY)){
            x = p1.x;
            y = p1.y;
            if(varX > 0 && varY > 0){
                printf("<Case 5.a>\n");
                pk = 2*varY - varX;
                glBegin( GL_POINTS );
                glVertex2i( p1.x, p1.y );
                glEnd( );
                for(x = p1.x+1; x < p2.x; x++){
                    if(pk < 0){
                        pk += 2*varY;
                    }
                    else{
                        y++;
                        pk += 2*varY - 2*varX;
                    }
                    glBegin( GL_POINTS );
                    glVertex2i( x, y );
                    glEnd( );
                }
            }
            else if(varX < 0 && varY > 0){
                printf("<Case 5.b>\n");
                varX *= -1;
                pk = 2*varY - varX;
                glBegin( GL_POINTS );
                glVertex2i( p1.x, p1.y );
                glEnd( );
                for(x = p1.x-1; x > p2.x; x--){
                    if(pk < 0){
                        pk += 2*varY;
                    }
                    else{
                        y++;
                        pk += 2*varY - 2*varX;
                    }
                    glBegin( GL_POINTS );
                    glVertex2i( x, y );
                    glEnd( );
                }
            }
            else if(varX > 0 && varY < 0){
                printf("<Case 5.c>\n");
                varY *= -1;
                pk = 2*varY - varX;
                glBegin( GL_POINTS );
                glVertex2i( p1.x, p1.y );
                glEnd( );
                for(x = p1.x+1; x < p2.x; x++){
                    if(pk < 0){
                        pk += 2*varY;
                    }
                    else{
                        y--;
                        pk += 2*varY - 2*varX;
                    }
                    glBegin( GL_POINTS );
                    glVertex2i( x, y );
                    glEnd( );
                }
            }
            else if(varX < 0 && varY < 0){
                printf("<Case 5.d>\n");
                varX *= -1;
                varY *= -1;
                pk = 2*varY - varX;
                glBegin( GL_POINTS );
                glVertex2i( p1.x, p1.y );
                glEnd( );
                for(x = p1.x-1; x > p2.x; x--){
                    if(pk < 0){
                        pk += 2*varY;
                    }
                    else{
                        y--;
                        pk += 2*varY - 2*varX;
                    }
                    glBegin( GL_POINTS );
                    glVertex2i( x, y );
                    glEnd( );
                }
            }
        }
        // Entre 45° e 135°
        else{
            x = p1.x;
            y = p1.y;
            if(varX > 0 && varY > 0){
                printf("<Case 6.a>\n");
                pk = 2*varX - varY;
                glBegin( GL_POINTS );
                glVertex2i( p1.x, p1.y );
                glEnd( );
                for(y = p1.y+1; y < p2.y; y++){
                    if(pk < 0){
                        pk += 2*varX;
                    }
                    else{
                        x++;
                        pk += 2*varX - 2*varY;
                    }
                    glBegin( GL_POINTS );
                    glVertex2i( x, y );
                    glEnd( );
                }
            }
            else if(varX < 0 && varY > 0){
                printf("<Case 6.b>\n");
                varX *= -1;
                pk = 2*varX - varY;
                glBegin( GL_POINTS );
                glVertex2i( p1.x, p1.y );
                glEnd( );
                for(y = p1.y+1; y < p2.y; y++){
                    if(pk < 0){
                        pk += 2*varX;
                    }
                    else{
                        x--;
                        pk += 2*varX - 2*varY;
                    }
                    glBegin( GL_POINTS );
                    glVertex2i( x, y );
                    glEnd( );
                }
            }
            else if(varX > 0 && varY < 0){
                printf("<Case 6.c>\n");
                varY *= -1;
                pk = 2*varX - varY;
                glBegin( GL_POINTS );
                glVertex2i( p1.x, p1.y );
                glEnd( );
                for(y = p1.y-1; y > p2.y; y--){
                    if(pk < 0){
                        pk += 2*varX;
                    }
                    else{
                        x++;
                        pk += 2*varX - 2*varY;
                    }
                    glBegin( GL_POINTS );
                    glVertex2i( x, y );
                    glEnd( );
                }
            }
            else if(varX < 0 && varY < 0){
                printf("<Case 6.d>\n");
                varX *= -1;
                varY *= -1;
                pk = 2*varX - varY;
                glBegin( GL_POINTS );
                glVertex2i( p1.x, p1.y );
                glEnd( );
                for(y = p1.y-1; y > p2.y; y--){
                    if(pk < 0){
                        pk += 2*varX;
                    }
                    else{
                        x--;
                        pk += 2*varX - 2*varY;
                    }
                    glBegin( GL_POINTS );
                    glVertex2i( x, y );
                    glEnd( );
                }
            }
        }
    }
}

void midPtCircle(int xc, int yc, int r) {
    int x=0;
    int y = r;
    int p = 1 - r;
    void circlePlotPoints(int, int, int, int);

    circlePlotPoints(x, y, xc, yc);

    while(x < y) {
        ++x;
        if(p < 0)
            p += 2 * x + 1;
        else {
            --y;
            p += 2 * (x-y) + 1;
        }
        circlePlotPoints(x,y,xc,yc);
    }
}


void circlePlotPoints(int x, int y, int xc, int yc) {
    glBegin(GL_POINTS);
    glVertex2i( xc + x, yc + y );
    glEnd( );

    glBegin(GL_POINTS);
    glVertex2i( xc - x, yc + y );
    glEnd( );

    glBegin(GL_POINTS);
    glVertex2i( xc + x, yc - y );
    glEnd( );

    glBegin(GL_POINTS);
    glVertex2i( xc - x, yc - y );
    glEnd( );

    glBegin(GL_POINTS);
    glVertex2i( xc + y, yc + x );
    glEnd( );

    glBegin(GL_POINTS);
    glVertex2i( xc - y, yc + x );
    glEnd( );

    glBegin(GL_POINTS);
    glVertex2i( xc + y, yc - x );
    glEnd( );

    glBegin(GL_POINTS);
    glVertex2i( xc - y, yc - x );
    glEnd( );
}

