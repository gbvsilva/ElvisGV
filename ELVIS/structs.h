#ifndef STRUCTS_H
#define STRUCTS_H

/**
 * Structs dos objetos desenhados. Em C++, structs são basicamente
 * classes onde seus parâmetros são públicos por padrão.
 */


struct obj; // Protótipo por força maior
/**
 *  Vértice. Guarda coordenadas X e Y.
 */
struct vertex{
    int x;
    int y;
};

/**
 *  Linha. Forma uma polilinha a partir de 
 *  uma lista duplamente encadeada.
 */
struct line{
    obj* top;
    line* nextLine;
    line* previousLine;
    vertex v1, v2;
    line(){
        nextLine = NULL;
        previousLine = NULL;
    }
};

/**
 * Retângulo. Possui os 4 vértices que formam um.
 * Também é utilizado para demarcar o tamanho de um agrupamento.
 */
struct rectangle{
    vertex v1;
    vertex v2;
    vertex v3;
    vertex v4;
};

/**
 * Circunferência. Possui o centro e o raio.
 */
struct circle{
    int radius;
    vertex center;
    circle() {
        radius=10.0;
    }
};

/**
 * Elipse. Centro, e raios X e Y.
 */
struct elipse {
    int rx, ry;
    vertex center;
    elipse() {
        rx=ry=0;
    }
};

/**
 * Guarda uma cor rgb, que varia de 0 a 1.
 * Esse é o escopo utilizado pelo openGL.
 */
struct color{
    float r;
    float g;
    float b;
    color(){
        r = 0;
        g = 0;
        b = 0;
    }
};

/**
 * Objeto genérico. Todos os objetos são apontados por este objeto.
 * A lista com todos os objetos é formado por uma lista duplamente encadeada, onde o primeiro e último objetos
 * são marcados por ponteiros globais (firstObj e lastObj). Com o encadeamento, os ponteiros globais e os ponteiros de marcação
 * (markedObj e markedObj2), é possível ler toda a cadeia de objetos, e se mover a partir de um objeto para ambas as
 * direções com mais facilidade.
 */
struct obj{
    // Fazem com que a lista seja encadeada.
    obj* nextObj; /**< Objeto seguinte */
    obj* previousObj; /**< Objeto seguinte */
    // Definem o subtipo do objeto
    vertex v;
    line* firstLine; /**< Primeira linha da polilinha */
    line* lastLine; /**< Última linha da polilinha */
    circle* c; /**< Ponteiro para a circunferência */
    elipse* elip; /**< Ponteiro para a elipse */
    rectangle* rec; /**< Ponteiro para o retângulo */
    // Cores do objeto
    color* fillColor; /**< Cor do preenchimento. Caso não haja preenchimento, o ponteiro é nulo */
    color* lineColor; /**< Cor da linha */
    // Utilizados apenas se o objeto é um grupo,
    // apontando para o primeiro e último elemento do grupo, respectivamente.
    obj* group; /**< Primeiro objeto do agrupamento */
    obj* endGroup; /**< Último objeto do agrupamento */
    // Marcadores
    bool marked; /**< Utilizad para seleção múltipla */
    bool mainGroup; /**< Marcação do grupo principal */
    // Layer
    int layer; /**< Valor da camada. */
    obj(){
	nextObj = NULL;
	previousObj = NULL;
	firstLine = NULL;
	lastLine = NULL;
	c = NULL;
	elip = NULL;
	rec = NULL;
	fillColor = NULL;
	group = NULL;
	endGroup = NULL;
	lineColor = new color();
	fillColor = NULL;
	marked = false;
	mainGroup = false;
	layer = 0;
    }
};
#endif
