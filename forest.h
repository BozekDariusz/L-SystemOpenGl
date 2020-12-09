#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QOpenGLWindow>
#include <QSurfaceFormat>
#include <QOpenGLFunctions>
#include <QtOpenGL>
#include <gl/GLU.h>
#include <math.h>
#include <Windows.h>
#include<cmath>
#include <iostream>
#include <QSlider>
#include "ui_mainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class mainWindow; }
QT_END_NAMESPACE
#define CAMERASPEED	0.03f
typedef struct tVector3					// expanded 3D vector struct
{
    tVector3() {}	// constructor
    tVector3 (float new_x, float new_y, float new_z) // initialize constructor
    {x = new_x; y = new_y; z = new_z;}
    // overload + operator so that we easier can add vectors
    tVector3 operator+(tVector3 vVector) {return tVector3(vVector.x+x, vVector.y+y, vVector.z+z);}
    // overload - operator that we easier can subtract vectors
    tVector3 operator-(tVector3 vVector) {return tVector3(x-vVector.x, y-vVector.y, z-vVector.z);}
    // overload * operator that we easier can multiply by scalars
    tVector3 operator*(float number)	 {return tVector3(x*number, y*number, z*number);}
    // overload / operator that we easier can divide by a scalar
    tVector3 operator/(float number)	 {return tVector3(x/number, y/number, z/number);}
    float x, y, z;						// 3D vector coordinates
}tVector3;


struct treeInfo
{


     treeInfo() {}
     treeInfo(int i) {depth=i;}

   int depth;
   tVector3 twigColour;
   tVector3 leafColour;

};

class treeWindow : public QOpenGLWindow
{
    Q_OBJECT

public:
    treeWindow(QWidget *parent = 0);
    ~treeWindow();
    float ballHeight;
    float postition;
    float velocity;
    float v0;
    float acceleration;
    float restitution;
    int x;
    bool pause;
    bool look;
    int y;
    std::vector<treeInfo> treeInfoCollection;
    treeInfo singleTree;
    bool change;


    void resetpva();
     void Keyboard_Input();


protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);
public slots:


private:
    QOpenGLContext *context;
    QOpenGLFunctions *openGLFunctions;



    float rotation;
    float gravity;





};

class CCamera
{
    public:

        tVector3 mPos;
        tVector3 mView;
        tVector3 mUp;

//NEW//////////////////NEW//////////////////NEW//////////////////NEW////////////////
        // This function let you control the camera with the mouse
        void Mouse_Move(int wndWidth, int wndHeight);

//NEW//////////////////NEW//////////////////NEW//////////////////NEW////////////////
        void Move_Camera(float speed);
        void Rotate_View(float speed);
        void Position_Camera(float pos_x, float pos_y,float pos_z,
                             float view_x, float view_y, float view_z,
                             float up_x,   float up_y,   float up_z);
        void Strafe_Camera(float speed);
};
class mainWindow : public QMainWindow
{
    Q_OBJECT

public:
    mainWindow(QWidget *parent = 0);
    ~mainWindow();

private slots:


private:
Ui::MainWindow *ui;


};


#endif // MAINWINDOW_H







