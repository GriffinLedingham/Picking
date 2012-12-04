#include "basicopenglview.h"
#include <QTimer>
#include <OpenGL/glu.h>
#include <QMouseEvent>
#include <cmath>
#include <ctime>
#include "flock.h"
#include "bird.h"

float cameraZ = -10.0f;
float cameraX = 0.0f;
float cameraY = -3.0f;
float cameraRotHor = 0.0f;
float cameraRotVer = 10.0f;
float windowWidth = 550;
float windowHeight = 281;

extern bool mFor, mBack, mLeft, mRight, cUp, cDown, cLeft, cRight, cJump, cFall, spaceDown, delDown;

int ballX,ballY,ballZ;
Flock flock;

BasicOpenGLView::BasicOpenGLView(QWidget *parent) :
    QGLWidget(QGLFormat(QGL::SampleBuffers), parent),
    projectionMatrix(),
    viewMatrix()
{


}

void BasicOpenGLView::initializeGL()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    flock = Flock();

    for (int i = 0; i < 50; i++) {
        Bird *tempBird = new Bird(0,0,0,i);
        flock.newBird(*tempBird);
    }



    //Timer which updates the GL render.
    QTimer *timer = new QTimer();
    timer->start(17); //time in ms, here 17 ms
    connect(timer ,SIGNAL(timeout()),this,SLOT(paintGL()));

    //Timer which updates the scene objects.
    QTimer *animate = new QTimer();
    animate->start(1000); //time in ms, here 1000 ms
    connect(timer ,SIGNAL(timeout()),this,SLOT(animateGL()));
}

void BasicOpenGLView::animateGL()
{

    //Controls used to move camera, based on key input
    if(mFor == true)
    {
        cameraX = cameraX - sin((cameraRotHor*3.141592)/180);
        cameraZ = cameraZ + cos((cameraRotHor*3.141592)/180);
    }
    if(mBack == true)
    {
        cameraX = cameraX + sin((cameraRotHor*3.141592)/180)*.5;
        cameraZ = cameraZ - cos((cameraRotHor*3.141592)/180)*.5;
    }
    if(mLeft == true)
    {
        cameraX = cameraX - sin(((cameraRotHor-90)*3.141592)/180);
        cameraZ = cameraZ + cos(((cameraRotHor-90)*3.141592)/180);
    }
    if(mRight == true)
    {
        cameraX = cameraX - sin(((cameraRotHor+90)*3.141592)/180);
        cameraZ = cameraZ + cos(((cameraRotHor+90)*3.141592)/180);
    }
    if(cLeft == true)
    {
        cameraRotHor -= 3;
    }
    if(cRight == true)
    {
        cameraRotHor += 3;
    }
    if(cUp == true)
    {
        cameraRotVer -= 3;
    }
    if(cDown == true)
    {
        cameraRotVer += 3;
    }
    if(cJump == true)
    {
        cameraY -= 1;
    }
    if(cFall == true)
    {
        cameraY += 1;
    }

}

void BasicOpenGLView::paintGL()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);


    //glEnable(GL_DEPTH_TEST);
    //resizeGL(windowWidth,windowHeight);

    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    viewMatrix.setToIdentity();
    viewMatrix.rotate(cameraRotVer,1,0,0);
    viewMatrix.rotate(cameraRotHor,0,1,0);

    viewMatrix.translate(cameraX,cameraY,cameraZ);
    viewMatrix.lookAt(QVector3D(0.0,3.0,-50.0),
                      QVector3D(0.0,4.0,0.0),
                      QVector3D(0.0,1.0,0.0));
    glLoadMatrixd(viewMatrix.data());
    //glEnable(GL_DEPTH_TEST);

    flock.animate();
}

void BasicOpenGLView::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    windowWidth = width;
    windowHeight = height;

    glMatrixMode(GL_PROJECTION);
    projectionMatrix.setToIdentity();
    projectionMatrix.perspective(60.0f, (float)windowWidth/(float)windowHeight, 0.01f, 200.0f);

    glLoadMatrixd(projectionMatrix.data());

    glMatrixMode(GL_MODELVIEW);
}


