#include "basicopenglview.h"
#include "catmullrominterpolation.h"
#include <QTimer>
#include <OpenGL/glu.h>
#include <QMouseEvent>
#include <cmath>
#include <ctime>

float cameraZ = 0.0f;
float cameraX = 0.0f;
float cameraY = 0.0f;
float cameraRotHor = 0.0f;
float cameraRotVer = 0.0f;
float lastArc = 0;
float windowWidth = 550;
float windowHeight = 281;

extern bool mFor, mBack, mLeft, mRight, cUp, cDown, cLeft, cRight, cJump, cFall, spaceDown, delDown;
extern int camView;
bool mouseDown, startDrag;

int hitPoint;
int pointCount = 1;
int subdivCount = 0;
int ballX,ballY,ballZ;

BasicOpenGLView::BasicOpenGLView(QWidget *parent) :
    QGLWidget(QGLFormat(QGL::SampleBuffers), parent),
    projectionMatrix(),
    viewMatrix(),
    splineControlPoints(),
    hoverIndex(-1)
{
    splineControlPoints.push_back(QVector3D(-2.0f, -2.0f, 0));
    splineControlPoints.push_back(QVector3D(-1.8f, 1.0f, 0));
    splineControlPoints.push_back(QVector3D(-1.0f, 0.0f, 0));
    splineControlPoints.push_back(QVector3D(0.0f, 2.0f, 0));
    splineControlPoints.push_back(QVector3D(1.0f, 0.0f, 0));
    splineControlPoints.push_back(QVector3D(1.8f, 1.0f, 0));
    splineControlPoints.push_back(QVector3D(2.0f, -2.0f, 0));
}

void BasicOpenGLView::initializeGL()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

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
    float arc = 0;
    int subdivisions = 50;

    QVector3D curPoint,curPoint2;
    GLUquadricObj *quadric;
    CatmulRomInterpolation interpolator(splineControlPoints);
    size_t numVertices = interpolator.getVertexCountForSubdivisionLevel(4);
    QVector<float> ArcLengthArray = interpolator.interpolateForArc();

    glEnable(GL_DEPTH_TEST);
    resizeGL(windowWidth,windowHeight);

    if(camView == 0)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        viewMatrix.setToIdentity();
        viewMatrix.rotate(cameraRotVer,1,0,0);
        viewMatrix.rotate(cameraRotHor,0,1,0);
        viewMatrix.translate(cameraX,cameraY,cameraZ);
        viewMatrix.lookAt(QVector3D(0.0f, 0.0f, -3.5f),
                          QVector3D(0.0f, 0.1f, 0.0f),
                          QVector3D(0.0f, 1.0f, 0.0f));
        glLoadMatrixd(viewMatrix.data());
    }
    else if(camView == 2)//top
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        viewMatrix.setToIdentity();
        viewMatrix.lookAt(QVector3D(0.0f, 10.0f, 0.0f),
                          QVector3D(0.0f, 0.0f, 0.0f),
                          QVector3D(.0f, 0.0f, 1.0f));
        glLoadMatrixd(viewMatrix.data());
    }
    else if(camView == 3)//side
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        viewMatrix.setToIdentity();
        viewMatrix.lookAt(QVector3D(10.0f, 0.0f, 0.0f),
                          QVector3D(0.0f, 0.1f, 0.0f),
                          QVector3D(1.0f, 1.0f, 0.0f));
        glLoadMatrixd(viewMatrix.data());
    }
    else if(camView == 4)//front
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        viewMatrix.setToIdentity();
        viewMatrix.translate(0.0f,0.0f,-5.0f);
        viewMatrix.lookAt(QVector3D(0.0f, 0.0f, -3.5f),
                          QVector3D(0.0f, 0.0f, 0.0f),
                          QVector3D(0.0f, 1.0f, 0.0f));
        glLoadMatrixd(viewMatrix.data());
    }
    glEnable(GL_DEPTH_TEST);

    for(int i = 0;i<ArcLengthArray.size();i++)
    {
        arc += ArcLengthArray[i];
    }

    if(lastArc != arc)
    {
        lastArc = arc;
        qDebug()<<"\n";
        interpolator.printArcTable(ArcLengthArray);
    }

    glColor3f(1.0f, 0.0f, 0.0f);
    glPointSize(2.0f);
    glBegin(GL_POINTS);

    for(int i = 1; i < splineControlPoints.size()-2; i++)
    {
        for(int j = 0;j<50;j++)
        {
            curPoint2 = interpolator.interpolateForT((float)j/(float)50, i);
            glColor3f(0.0f, 0.0f, 0.0f);
            glVertex3d(curPoint2.x(), curPoint2.y(), curPoint2.z());
        }
    }
    glEnd();

    glColor3f(1.0f, 0.0f, 0.0f);
    curPoint = interpolator.interpolateForT((float)subdivCount/(float)subdivisions, pointCount);
    if(camView == 1)//playback
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        viewMatrix.setToIdentity();
        //viewMatrix.rotate(cameraRotVer,1,0,0);
        //viewMatrix.rotate(cameraRotHor,0,1,0);
        //viewMatrix.translate(curPoint.x(), curPoint.y(), curPoint.z());
        viewMatrix.lookAt(QVector3D(curPoint.x(), curPoint.y(), curPoint.z()),
                          QVector3D(0.0f,0.0f, 1.0f),
                          QVector3D(0.0f, 1.0f, 0.0f));

        glLoadMatrixd(viewMatrix.data());
    }

    if(camView != 1)
    {
        glPushMatrix();
        glTranslatef(curPoint.x(), curPoint.y(), curPoint.z());
        quadric = gluNewQuadric();
        gluQuadricDrawStyle(quadric, GLU_FILL );
        gluSphere(quadric,.1,1000,1000);
        gluDeleteQuadric(quadric);
        glPopMatrix();
    }

    subdivCount++;
    if(subdivCount==subdivisions-1)
    {
        if(pointCount == splineControlPoints.size()-3)
        {
            pointCount = 1;
        }
        else
        {
            pointCount++;
        }
        subdivCount = 0;
    }

    glPointSize(10.0f);
    glBegin(GL_POINTS);
    for(int i = 0; i < splineControlPoints.size(); ++i)
    {
        if(i == hoverIndex)
        {
            glColor3f(0.0f, 1.0f, 0.0f);
        }
        else
        {
            glColor3f(0.0f, 1.0f, 0.0f);
        }
        if(camView != 1){
            glVertex3d(splineControlPoints[i].x(), splineControlPoints[i].y(), splineControlPoints[i].z());}
    }

    glColor3f(0.0f, 0.0f, 0.0f);
    glEnd();

    glPushMatrix();
    glTranslatef(0, 0, 1);
    quadric = gluNewQuadric();
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    gluQuadricDrawStyle(quadric, GLU_FILL );
    gluSphere(quadric,.5,100,10);
    gluDeleteQuadric(quadric);
    glPopMatrix();
}

void BasicOpenGLView::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    windowWidth = width;
    windowHeight = height;

    glMatrixMode(GL_PROJECTION);
    projectionMatrix.setToIdentity();
    if(camView == 0 || camView == 1)
    {
        projectionMatrix.perspective(60.0f, (float)windowWidth/(float)windowHeight, 0.01f, 50.0f);
    }
    else
    {
        projectionMatrix.ortho(-windowWidth/100, windowWidth/100, -windowHeight/100, windowHeight/100, -30.0, 30.0);
    }
    glLoadMatrixd(projectionMatrix.data());

    glMatrixMode(GL_MODELVIEW);
}

void BasicOpenGLView::mousePressEvent(QMouseEvent *event)
{
    const float threshold = 0.1f;
    float pointX,pointY,pointZ;

    CatmulRomInterpolation interpolator(splineControlPoints);

    mouseDown = true;
    pointX = -(((float)event->x()-(windowWidth/2.0f))/(windowWidth/2.0f))*windowWidth/100;
    pointY = -(((float)event->y()-(windowHeight/2.0f))/(windowHeight/2.0f))*windowHeight/100;
    if(delDown && splineControlPoints.size() > 4)
    {
        for(int i = 0; i < splineControlPoints.size(); ++i)
        {
            QVector3D curPoint = splineControlPoints[i];

            if(camView==4)
            {

                if(pointX < curPoint.x()+threshold && pointX > curPoint.x()-threshold && pointY < curPoint.y()+threshold && pointY > curPoint.y()-threshold)
                {
                    if((splineControlPoints.size()-1) >= 4)
                    {
                        pointCount = 1;
                        qDebug() << "delete";
                        splineControlPoints.remove(i);
                    }
                }
            }
            else if(camView==3)
            {

                if(pointX < curPoint.z()+threshold && pointX > curPoint.z()-threshold && pointY < curPoint.y()+threshold && pointY > curPoint.y()-threshold)
                {
                    if((splineControlPoints.size()-1) >= 4)
                    {
                        pointCount = 1;
                        qDebug() << "delete";
                        splineControlPoints.remove(i);
                    }
                }
            }
            else if(camView==2)
            {

                if(pointY < curPoint.z()+threshold && pointY > curPoint.z()-threshold && pointX < curPoint.x()+threshold && pointX > curPoint.x()-threshold)
                {
                    if((splineControlPoints.size()-1) >= 4)
                    {
                        pointCount = 1;
                        qDebug() << "delete";
                        splineControlPoints.remove(i);
                    }
                }
            }
        }
    }
    else if(spaceDown)
    {
        pointX = -(((float)event->x()-(windowWidth/2.0f))/(windowWidth/2.0f))*windowWidth/100;
        pointY = -(((float)event->y()-(windowHeight/2.0f))/(windowHeight/2.0f))*windowHeight/100;

        if(camView == 4)
        {
            splineControlPoints.push_back(QVector3D(pointX, pointY, 0));
        }
        else if(camView==3)
        {
            splineControlPoints.push_back(QVector3D(0, pointY, pointX));
        }
        else if(camView==2)
        {
            splineControlPoints.push_back(QVector3D(pointX, 0, pointY));
        }
    }
}

void BasicOpenGLView::mouseReleaseEvent(QMouseEvent *event)
{
    mouseDown = false;
    startDrag = false;
}

void BasicOpenGLView::mouseMoveEvent(QMouseEvent *event)
{
    const float threshold = 0.1f;
    float curDistance = std::numeric_limits<float>::max();

    for(size_t i = 0; i < splineControlPoints.size(); ++i)
    {
        QVector3D curPoint = splineControlPoints[i];

        float pointX,pointY,pointZ;

        pointX = -(((float)event->x()-(windowWidth/2.0f))/(windowWidth/2.0f))*windowWidth/100;
        pointY = -(((float)event->y()-(windowHeight/2.0f))/(windowHeight/2.0f))*windowHeight/100;

        if(camView==4)
        {
            if(pointX < curPoint.x()+threshold && pointX > curPoint.x()-threshold && pointY < curPoint.y()+threshold && pointY > curPoint.y()-threshold)
            {
                if(mouseDown)
                {
                    startDrag=true;
                    hitPoint = i;
                }
            }
            else if(startDrag && hitPoint == i)
            {   pointCount = 1;
                splineControlPoints[i].setX((qreal)pointX);
                splineControlPoints[i].setY((qreal)pointY);
                paintGL();
            }
        }
        if(camView==2)
        {
            if(pointX < curPoint.x()+threshold && pointX > curPoint.x()-threshold && pointZ < curPoint.z()+threshold && pointZ > curPoint.z()-threshold)
            {
                if(mouseDown)
                {
                    startDrag=true;
                    hitPoint = i;
                }
            }
            else if(startDrag && hitPoint == i)
            {
                pointCount = 1;
                splineControlPoints[i].setX((qreal)pointX);
                splineControlPoints[i].setZ((qreal)pointY);
                paintGL();
            }
        }
        if(camView==3)
        {
            if(pointY < curPoint.y()+threshold && pointY > curPoint.y()-threshold && pointZ < curPoint.z()+threshold && pointZ > curPoint.z()-threshold)
            {
                if(mouseDown)
                {
                    startDrag=true;
                    hitPoint = i;
                }
            }
            else if(startDrag && hitPoint == i)
            {
                pointCount = 1;
                splineControlPoints[i].setZ((qreal)pointX);
                splineControlPoints[i].setY((qreal)pointY);
                paintGL();
            }
        }
    }
    hoverIndex = -1;
}
