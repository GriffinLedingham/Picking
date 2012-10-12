#include "basicopenglview.h"
#include "catmullrominterpolation.h"
#include <QTimer>
#include <OpenGL/glu.h>
#include <QMouseEvent>
#include <cmath>
#include <OpenGL/glu.h>
#include <ctime>

float cameraZ = 0.0f;
float cameraX = 0.0f;
float cameraY = 0.0f;
float cameraRotHor = 0.0f;
float cameraRotVer = 0.0f;

float windowWidth = 550;
float windowHeight = 281;

int hitPoint;
extern bool mFor, mBack, mLeft, mRight, cUp, cDown, cLeft, cRight, cJump, cFall, spaceDown, delDown;
extern int camView;
bool mouseDown;
bool startDrag;

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
    //splineControlPoints.push_back(QVector3D(2.0f, 0.0f, 0));
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
    if(camView == 1)
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
    if(camView == 2)//top
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        viewMatrix.setToIdentity();
        viewMatrix.lookAt(QVector3D(0.0f, 10.0f, 0.0f),
                          QVector3D(0.0f, 0.0f, 0.0f),
                          QVector3D(.0f, 0.0f, 1.0f));
        glLoadMatrixd(viewMatrix.data());
    }
    if(camView == 3)//side
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        viewMatrix.setToIdentity();
        viewMatrix.lookAt(QVector3D(10.0f, 0.0f, 0.0f),
                          QVector3D(0.0f, 0.1f, 0.0f),
                          QVector3D(1.0f, 1.0f, 0.0f));
        glLoadMatrixd(viewMatrix.data());
    }
    if(camView == 4)//front
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

    CatmulRomInterpolation interpolator(splineControlPoints);

    size_t numVertices = interpolator.getVertexCountForSubdivisionLevel(4);

    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_POINTS);
    int subdivisions = 30;

    QVector3D curPoint;
    try
    {
        for(int i = 1; i < splineControlPoints.size()-2; i++)
        {
            for(int j = 0;j<subdivisions;j++)
            {
                curPoint = interpolator.interpolateForT((float)j/(float)subdivisions, i);
                glVertex3d(curPoint.x(), curPoint.y(), curPoint.z());
            }
        }

    }
    catch(...) { }
    glEnd();

    glPointSize(5.0f);
    glBegin(GL_POINTS);
    for(size_t i = 0; i < splineControlPoints.size(); ++i)
    {
        if(i == hoverIndex)
        {
            glColor3f(0.0f, 1.0f, 0.0f);
        }
        else
        {
            glColor3f(0.0f, 0.0f, 0.0f);
        }
        glVertex3d(splineControlPoints[i].x(), splineControlPoints[i].y(), splineControlPoints[i].z());
    }

    glColor3f(0.0f, 0.0f, 0.0f);

    glEnd();
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
        projectionMatrix.ortho(-windowWidth/100, windowWidth/100, -windowHeight/100, windowHeight/100, -10.0, 10.0);
    }
    glLoadMatrixd(projectionMatrix.data());

    glMatrixMode(GL_MODELVIEW);
}

//void BasicOpenGLView::mousePressEvent(QMouseEvent *event)
//{
// TODO deal with mouse buttons
/*float pointX,pointY,pointZ;

    pointX = -(((float)event->x()-(windowWidth/2.0f))/(windowWidth/2.0f))*windowWidth/100;
    pointY = -(((float)event->y()-(windowHeight/2.0f))/(windowHeight/2.0f))*windowHeight/100;

    if(camView == 4)
    {
        splineControlPoints.push_back(QVector3D(pointX, pointY, 0));
    }
    else if(camView==3)
    {
        splineControlPoints.push_back(QVector3D(0, pointY, pointX));
    }*/
//}

void BasicOpenGLView::mousePressEvent(QMouseEvent *event)
{
    //qDebug() << "mouse down";
    mouseDown = true;
    const float threshold = 0.1f;

    CatmulRomInterpolation interpolator(splineControlPoints);

    float pointX,pointY,pointZ;

    pointX = -(((float)event->x()-(windowWidth/2.0f))/(windowWidth/2.0f))*windowWidth/100;
    pointY = -(((float)event->y()-(windowHeight/2.0f))/(windowHeight/2.0f))*windowHeight/100;
    if(delDown)
    {
        for(size_t i = 0; i < splineControlPoints.size(); ++i)
        {
            QVector3D curPoint = splineControlPoints[i];


            if(camView==4)
            {

                if(pointX < curPoint.x()+threshold && pointX > curPoint.x()-threshold && pointY < curPoint.y()+threshold && pointY > curPoint.y()-threshold)
                {
                    qDebug() << "delete";
                    splineControlPoints.remove(i);
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
    }
}

void BasicOpenGLView::mouseReleaseEvent(QMouseEvent *event)
{
    mouseDown = false;
    startDrag = false;
}

void BasicOpenGLView::cameraSpline()
{
    glBegin(GL_POINTS);
    int subdivisions = 20;

    CatmulRomInterpolation interpolator(splineControlPoints);

    QVector3D curPoint;
    try
    {
        if(camView == 1)
        {
            for(int i = 1; i < splineControlPoints.size()-2; i++)
            {
                for(int j = 0;j<subdivisions;j++)
                {
                    curPoint = interpolator.interpolateForT((float)j/(float)subdivisions, i);
                    //glVertex3d(curPoint.x(), curPoint.y(), curPoint.z());
                    cameraX = curPoint.x();
                    cameraY = curPoint.y();
                    cameraZ = curPoint.z();
                    sleep(2);
                    paintGL();
                }
            }
        }
    }
    catch(...) { }
    glEnd();
}

void BasicOpenGLView::mouseMoveEvent(QMouseEvent *event)
{
    //  TODO
    //  1. convert Window coordiates into OpenGL View coordinates in range [-1, 1]
    //  2. build ray start and end in postprojected space (z coordinates 0 and 1)
    //  3. create inverse viewProjection matrix
    //  4. use inverse view-projection matrix to transform the ray start and end into world space
    //  5. build ray in world space (direction vector has to be normalized)

    const float threshold = 0.1f;
    float curDistance = std::numeric_limits<float>::max();

    //  6. loop over splineControlPoints and calculate the distance of every control point to the ray
    for(size_t i = 0; i < splineControlPoints.size(); ++i)
    {
        QVector3D curPoint = splineControlPoints[i];

        float pointX,pointY,pointZ;

        pointX = -(((float)event->x()-(windowWidth/2.0f))/(windowWidth/2.0f))*windowWidth/100;
        pointY = -(((float)event->y()-(windowHeight/2.0f))/(windowHeight/2.0f))*windowHeight/100;

        //qDebug() << pointX << "," << pointY;

        if(camView==4)
        {
            if(pointX < curPoint.x()+threshold && pointX > curPoint.x()-threshold && pointY < curPoint.y()+threshold && pointY > curPoint.y()-threshold)
            {
                //qDebug() << "hit:" << i;
                //curPoint.setX((qreal)pointX);
                //curPoint.setY((qreal)pointY);
                if(mouseDown)
                {
                    startDrag=true;
                    hitPoint = i;
                }
            }
            else if(startDrag && hitPoint == i)
            {
                splineControlPoints[i].setX((qreal)pointX);
                splineControlPoints[i].setY((qreal)pointY);
                // = curPoint;
                paintGL();
            }
        }
        //  7. calculate the direction vector between the current point and the ray origin
        //  8. calculate the dot product between the former result and the ray direction
        //  9. if ray directions lenght != 1 then normalize the result of 8 according to ray length
        //  10. calculate point on ray with t being the result of 9
        //  11. calculate the distance vector between result of 10 and current spline control point
        //  12. check the length of the distance vector. if smaller than threshold, we have a hit.
    }

    hoverIndex = -1;
}
