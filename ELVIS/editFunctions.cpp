#include <iostream>
#include "structs.h"
#include "editFunctions.h"


obj* copy(obj* form) {
    obj* n = new obj();

    if(form->c != NULL) {
        n->c = new circle();
        n->c->radius = form->c->radius;
        n->c->center.x = form->c->center.x;
        n->c->center.y = form->c->center.y;
    }else if(form->elip != NULL) {
        n->elip = new elipse();
        n->elip->rx = form->elip->rx;
        n->elip->ry = form->elip->ry;
    }else if(form->rec != NULL) {
        n->rec = new rectangle();
        n->rec->v1.x = form->rec->v1.x;
        n->rec->v1.y = form->rec->v1.y;
        n->rec->v2.x = form->rec->v2.x;
        n->rec->v2.y = form->rec->v2.y;
        n->rec->v3.x = form->rec->v3.x;
        n->rec->v3.y = form->rec->v3.y;
        n->rec->v4.x = form->rec->v4.x;
        n->rec->v4.y = form->rec->v4.y;
    }else if(form->firstLine != NULL) {
        n->firstLine = new line();
        line *pt1 = form->firstLine;
        line *pt2 = n->firstLine;
        pt2->v1.x = pt1->v1.x;
        pt2->v1.y = pt1->v1.y;
        pt2->v2.x = pt1->v2.x;
        pt2->v2.y = pt1->v2.y;

        while(pt1 != form->lastLine) {
            pt2->nextLine = new line();
            pt2->nextLine->previousLine = pt2;
            pt2 = pt2->nextLine;
            pt1 = pt1->nextLine;

            pt2->v1.x = pt1->v1.x;
            pt2->v1.y = pt1->v1.y;
            pt2->v2.x = pt1->v2.x;
            pt2->v2.y = pt1->v2.y;
        }
        n->lastLine = pt2;
    }
    return n;
}
