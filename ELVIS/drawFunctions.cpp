#include <stdio.h>
#include <stdlib.h>
#include <GL/glu.h>
#include "structs.h"
#include "drawFunctions.h"
#define ROUND(a) ((int) (a+0.5))

void bresenham( int x1, int y1, int x2, int y2 ) {
    vertex p1, p2;
    long dist;
    int i, varX, varY, x, y, pk;
    p1.x = ROUND( x1 );
    p1.y = ROUND( y1 );
    p2.x = ROUND( x2 );
    p2.y = ROUND( y2 );
    // Casos triviais
    // 0°
    if(p1.y == p2.y){
        dist = p2.x - p1.x;
        if(dist > 0){
            for(i = 0; i < dist; i++){
                glBegin( GL_POINTS );
                glVertex2i( p1.x+i, p1.y );
                glEnd( );
            }
        }
        else{
            dist *= -1;
            for(i = 0; i < dist; i++){
                glBegin( GL_POINTS );
                glVertex2i( p1.x-i, p1.y );
                glEnd( );
            }
        }
    }
    // 90°
    else if(p1.x == p2.x){
        dist = p2.y - p1.y;
        if(dist > 0){
            for(i = 0; i < dist; i++){
                glBegin( GL_POINTS );
                glVertex2i( p1.x, p1.y+i );
                glEnd( );
            }
        }
        else{
            dist *= -1;
            for(i = 0; i < dist; i++){
                glBegin( GL_POINTS );
                glVertex2i( p1.x, p1.y-i );
                glEnd( );
            }
        }
    }
    // 45°
    else if((p2.x-p1.x) == (p2.y - p1.y)){
        dist = p2.x - p1.x;
        if(dist > 0){
            for(i = 0; i < dist; i++){
                glBegin( GL_POINTS );
                glVertex2i( p1.x+i, p1.y+i );
                glEnd( );
            }
        }
        else{
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
            for(i = 0; i < dist; i++){
                glBegin( GL_POINTS );
                glVertex2i( p1.x+i, p1.y-i );
                glEnd( );
            }
        }
        else{
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

void midPtElipse(int xc, int yc, int a, int b) {
    long int a2, b2, twoa2, twob2, foura2, fourb2, mx, my;
    long int d, mida, midb;
    int x, y;

    if(b < 0) b *= -1;
    x=0; y=b;
    a2=a*a; b2=b*b;
    twoa2 = a2+a2; twob2 = b2+b2;
    foura2 = twoa2+twoa2; fourb2 = twob2+twob2;
    mx = fourb2;
    my = foura2*(y-1);
    mida = a2 / 2;
    midb = b2 / 2;
    d = twob2 - a2 - my/2 - mida;

    void elipsePlotPts(int,int,int,int);
    // Regiao 1
    while(d <= my) {
        elipsePlotPts(xc,yc,x,y);
        if(d > 0) {
            d = d - my;
            y = y - 1;
            my = my - foura2;
        }
        d = d + twob2 + mx;
        ++x;
        mx = mx + fourb2;
    }
    d = d - (mx + my)/2 + (b2 - a2) + (mida - midb);

    // Regiao 2
    while(y >= 0) {
        elipsePlotPts(xc,yc,x,y);
        if(d <= 0) {
            d = d + mx;
            ++x;
            mx = mx + fourb2;
        }
        d = d + twoa2 - my;
        --y;
        my = my - foura2;
    }
}

void elipsePlotPts(int xc, int yc, int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(xc + x, yc + y);
    glEnd();

    glBegin(GL_POINTS);
    glVertex2i(xc - x, yc + y);
    glEnd();

    glBegin(GL_POINTS);
    glVertex2i(xc + x, yc - y);
    glEnd();

    glBegin(GL_POINTS);
    glVertex2i(xc - x, yc - y);
    glEnd();
}
