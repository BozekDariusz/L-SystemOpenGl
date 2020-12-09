#include "forest.h"
#include "ui_forest.h"



#include<QDebug>
int i=0;
void drawLeaf(treeInfo tree){
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE,1.0);
    glBegin(GL_TRIANGLES);




    glColor3f(tree.twigColour.x,tree.twigColour.y,tree.twigColour.z);//twig
    glNormal3f(0.0,1.0,0.0);
    glVertex3f(0.0,0.0,0.0);
    glVertex3f(0.01,0.0,0.1);
    glVertex3f(-0.01,0.0,0.1);



    glColor3f(tree.leafColour.x,tree.leafColour.y,tree.leafColour.z);//leaf
    glNormal3f(0.0,1.0,0.0);
    glVertex3f(0.35,0.0,0.1);
    glVertex3f(0.0,0.0,0.5);
    glVertex3f(-0.35,0.0,0.1);

   glVertex3f(0.0,0.0,0.1);
   glVertex3f(0.35,0.0,0.40);
   glVertex3f(-0.35,0.0,0.40);
    glEnd();
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE,0.0);
}

void drawTaperedCylinder(int depth,float bottom,float top, float lenght ){
    GLUquadric *quad;
       quad = gluNewQuadric();
    gluCylinder( quad, top,bottom,lenght,3,1);


}
void drawTree(treeInfo tree,int depth){


    if(depth<0)
            return;
    if(depth==0){
        drawLeaf(tree);
        return;

    }
    else
        glColor3f(tree.twigColour.x,tree.twigColour.y,tree.twigColour.z);//brown
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    drawTaperedCylinder(depth,0.05*(depth-1),0.05*depth,depth);
    glRotatef(23.0, 0.0, 0.0, 1.0);
    glTranslatef(0.0, 0.0, depth);
    drawTree(tree,depth-1);
    glRotatef(-32.5, 0.0, 1.0, 0.0);
    drawTree(tree,depth-2);

    glRotatef(35.0, 0.0, 1.0, 0.0);
    glRotatef(120.0, 0.0, 0.0, 1.0);
    glRotatef(-35.0, 0.0, 1.0, 0.0);
    drawTree(tree,depth-2);

    glRotatef(35.0, 0.0, 1.0, 0.0);
    glRotatef(120.0, 0.0, 0.0, 1.0);
    glRotatef(-35.0, 0.0, 1.0, 0.0);
    drawTree(tree,depth-2);


    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

}


mainWindow::mainWindow(QWidget *parent) : QMainWindow(parent)
 , ui(new Ui::MainWindow)
{

     treeWindow *renderWindow = new  treeWindow();
     QWidget *container =QWidget::createWindowContainer(renderWindow);
     this->layout()->addWidget(container);
     ui->setupUi(this);


}

mainWindow::~mainWindow()
{
delete ui;
}

treeWindow::treeWindow(QWidget *parent)
{



    setSurfaceType(QWindow::OpenGLSurface);

    QSurfaceFormat format;
    format.setProfile(QSurfaceFormat::CompatibilityProfile);
    //format.setVersion(4,6);
    setFormat(format);

    context = new QOpenGLContext;
    context->setFormat(format);
    context->create();
    context->makeCurrent(this);


        x=0;//pause check
            y=0;//restart check
            look=true;
            pause = false;
}
CCamera objCamera;

treeWindow::~treeWindow()
{
}

void treeWindow::initializeGL()
{
    for (int i=0;i<1000;i++){//seed for every tree
        treeInfo tree;
        tree.depth=rand() % 3 + 2;//becaouse of not multithreading using more depth will slow down everything
      int twig =rand() % 3 + 1;
      int leaf =rand() % 3 + 1;
      switch (twig) {
      case 1 :
          tree.twigColour=(tVector3(0.5f, 0.35f, 0.05f));//brow
           break;
       case 2 :
          tree.twigColour=(tVector3(0.4f, 0.4f, 0.4f));//darkgrey
           break;
      case 3:
         tree.twigColour=(tVector3(0.0f, 0.5f, 0.0f));//darkgreen
          break;
      }
      switch (leaf) {
      case 1 :
          tree.leafColour=(tVector3(0.0f, 1.0f, 0.0f));//green
           break;
       case 2 :
          tree.leafColour=(tVector3(0.5f, 0.0f, 0.5f));//darkmagenta
           break;
      case 3:
         tree.leafColour=(tVector3(0.0f, 1.0f, 1.0f));//cyan
          break;
      }
        treeInfoCollection.push_back(tree);
    }
    glEnable(GL_DEPTH_TEST);
    resizeGL(this->width(), this->height());
}

void treeWindow::resizeGL(int w, int h)
{

    //set viewport
objCamera.Position_Camera(0, 2.5f, 5,	0, 2.5f, 0,   0, 1, 0);
    glViewport(0, 0, w, h);
    qreal aspectRatio = qreal(w)/qreal(h);

    //initialize projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(75, aspectRatio, 0.1, 400000000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Draw_Grid()
{

    for(float i = -500; i <= 500; i += 5)
    {
        glBegin(GL_LINES);
        glColor3ub(150, 190, 150);
        glVertex3f(-500, 0, i);
        glVertex3f(500, 0, i);
        glVertex3f(i, 0,-500);
        glVertex3f(i, 0, 500);
        glEnd();
    }
}
void treeWindow::paintGL()
{

    Keyboard_Input();
        if(look){
        objCamera.Mouse_Move(640,480);
        }
    glClearColor(0.39f, 0.58f, 0.93f , 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     Draw_Grid();

    glLoadIdentity();

    gluLookAt(objCamera.mPos.x,  objCamera.mPos.y,  objCamera.mPos.z,
              objCamera.mView.x, objCamera.mView.y, objCamera.mView.z,
              objCamera.mUp.x,   objCamera.mUp.y,   objCamera.mUp.z);
    glPushMatrix();



glRotatef(	270, 1, 0,0);
for(int i=0;i<100;i++){
 glPushMatrix();
    for(int j=0;j<10;j++){

    drawTree(treeInfoCollection[i*j+j+i],treeInfoCollection[i*j+j+i].depth);//not sure if this index is right but it works
    glTranslated(10,0,0);
    }
     glPopMatrix();
    glTranslated(0,10,0);
}
   glPopMatrix();

    glFlush();


    this->update();

}



void treeWindow::resizeEvent(QResizeEvent *event)
{
    resizeGL(this->width(), this->height());

    this->update();
}

void treeWindow::paintEvent(QPaintEvent *event)
{


    paintGL();

}



///////////////////////////////////camera stuff below that line


void CCamera::Position_Camera(float pos_x,  float pos_y,  float pos_z,
                              float view_x, float view_y, float view_z,
                              float up_x,   float up_y,   float up_z)
{
    objCamera.mPos	= tVector3(pos_x,  pos_y,  pos_z ); // set position
    objCamera.mView	= tVector3(view_x, view_y, view_z); // set view
    objCamera.mUp		= tVector3(up_x,   up_y,   up_z  ); // set the up vector
}

void CCamera::Rotate_View(float speed)
{
    tVector3 vVector = mView - mPos;	// Get the view vector

    mView.z = (float)(mPos.z + sin(speed)*vVector.x + cos(speed)*vVector.z);
    mView.x = (float)(mPos.x + cos(speed)*vVector.x - sin(speed)*vVector.z);
}

void CCamera::Mouse_Move(int wndWidth, int wndHeight)
{
    POINT mousePos;
    int mid_x = wndWidth  >> 1;
    int mid_y = wndHeight >> 1;
    float angle_y  = 0.0f;
    float angle_z  = 0.0f;

    GetCursorPos(&mousePos);	// Get the 2D mouse cursor (x,y) position

    if( (mousePos.x == mid_x) && (mousePos.y == mid_y) ) return;

    SetCursorPos(mid_x, mid_y);	// Set the mouse cursor in the center of the window

    // Get the direction from the mouse cursor, set a resonable maneuvering speed
    angle_y = (float)( (mid_x - mousePos.x) ) / 1000;
    angle_z = (float)( (mid_y - mousePos.y) ) / 1000;

    // The higher the value is the faster the camera looks around.
    objCamera.mView.y += angle_z*2 ;

    if((mView.y - mPos.y) > 8)  mView.y = mPos.y + 8;
    if((mView.y - mPos.y) <-8)  mView.y = mPos.y - 8;
    Rotate_View(-angle_y); // Rotate
}
void CCamera::Move_Camera(float speed)
{
    tVector3 vVector = mView - mPos;	// Get the view vector

    // forward positive cameraspeed and backward negative -cameraspeed.
    mPos.x  = mPos.x  + vVector.x * speed;
    mPos.z  = mPos.z  + vVector.z * speed;
    mView.x = mView.x + vVector.x * speed;
    mView.z = mView.z + vVector.z * speed;
}
void CCamera::Strafe_Camera(float speed)
{
    tVector3 vVector = mView - mPos;	// Get the view vector
    tVector3 vOrthoVector;              // Orthogonal vector for the view vector

    vOrthoVector.x = -vVector.z;
    vOrthoVector.z =  vVector.x;

    // left positive cameraspeed and right negative -cameraspeed.
    mPos.x  = mPos.x  + vOrthoVector.x * speed;
    mPos.z  = mPos.z  + vOrthoVector.z * speed;
    mView.x = mView.x + vOrthoVector.x * speed;
    mView.z = mView.z + vOrthoVector.z * speed;
}







void treeWindow::Keyboard_Input()//for camera movement
{

    if((GetKeyState(VK_UP) & 0x80) || (GetKeyState('W') & 0x80))
    {
        objCamera.Move_Camera( CAMERASPEED);
    }

    if((GetKeyState(VK_DOWN) & 0x80) || (GetKeyState('S') & 0x80))
    {
        objCamera.Move_Camera(-CAMERASPEED);
    }

    if((GetKeyState(VK_LEFT) & 0x80) || (GetKeyState('A') & 0x80))
    {
        objCamera.Strafe_Camera(-CAMERASPEED);
    }

    if((GetKeyState(VK_RIGHT) & 0x80) || (GetKeyState('D') & 0x80))
    {
        objCamera.Strafe_Camera( CAMERASPEED);
    }
    if(GetKeyState(0x50)& 0x80 && (x == 0)){
        pause=!pause;
        x++;
    }
    else if(!(GetKeyState(0x50)& 0x80)){
        x=0;

    }



    if(GetKeyState(0x43)& 0x80 && (y == 0)){
        look=!look;
        y++;
    }
    else if(!(GetKeyState(0x43)& 0x80)){
        y=0;

    }

}



















