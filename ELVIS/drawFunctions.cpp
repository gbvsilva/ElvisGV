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

void midPtElipse(int xc, int yc, int rx, int ry) {
    int rx2 = rx*rx;
    int ry2 = ry*ry;
    int twoRx2 = 2*rx2;
    int twoRy2 = 2*ry2;
    int p;
    int x=0;
    int y=ry;
    int px=0;
    int py= twoRx2*y;

    void elipsePlotPts(int,int,int,int);

    elipsePlotPts(xc,yc,x,y);
    /* Regiao 1 */
    p = round(ry2 - (rx2*ry) + (0.25*rx2));
    while(px < py) {
        ++x;
        px += twoRy2;
        if(p < 0) {
            p += ry2 + px;
        }else {
            --y;
            py -= twoRx2;
            p += ry2 + px - py;
        }
        elipsePlotPts(xc,yc,x,y);
    }

    /* Regiao 2 */
    p = round(ry2*(x+0.5)*(x+0.5) + rx2*(y-1)*(y-1) - rx2*ry2);
    while(y > 0) {
        --y;
        py -= twoRx2;
        if(p > 0) {
            p += rx2 - py;
        }else {
            ++x;
            px += twoRy2;
            p += rx2 - py + px;
        }
        elipsePlotPts(xc,yc,x,y);
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
