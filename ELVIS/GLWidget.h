#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>

class GLWidget : public QGLWidget
{
    Q_OBJECT
public slots:
    void mouseClick();
    void mouseMovement();
    void drawPolyline();
    void drawCircle();
    void drawRectangle();
    void drawEllipse();
    void editCopy();
    void editTranslation();
signals:
    void mouseClicked();
    void mouseMoved();
public:
    explicit GLWidget(QWidget *parent = 0);

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void clearMarkers();
    void clearMouse();
    void delSelected();
    void drawSquareMarker(float x, float y, int size);
    void drawSelSquareMarker(float x, float y, int size, int color);
    void createGroup();
    void undoGroup();
    void changeLayer();
    void objDebug();
    void increaseLayer(int incValue);
    void decreaseLayer(int decValue);
    void save();
    void load();
    void saveSVG();
};
#endif // GLWIDGET_H
