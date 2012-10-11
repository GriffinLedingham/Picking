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

extern bool mFor, mBack, mLeft, mRight, cUp, cDown, cLeft, cRight, cJump, cFall;

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
    connect(timer ,SIGNAL(timeout()),this,SLOT(updateGL()));

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

    //Key input to spin the camera, only if pong mode is disabled
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

    //Pan camera up and down in the Y-Axis
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
    glMatrixMode(GL_MODELVIEW);
    viewMatrix.setToIdentity();
    viewMatrix.rotate(cameraRotVer,1,0,0);
    viewMatrix.rotate(cameraRotHor,0,1,0);
    viewMatrix.translate(cameraX,cameraY,cameraZ);
    viewMatrix.lookAt(QVector3D(0.0f, 2.0f, -3.5f),
                      QVector3D(0.0f, 0.1f, 0.0f),
                      QVector3D(0.0f, 1.0f, 0.0f));
    glLoadMatrixd(viewMatrix.data());


    CatmulRomInterpolation interpolator(splineControlPoints);

    size_t numVertices = interpolator.getVertexCountForSubdivisionLevel(4);

    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_POINTS);

    QVector3D curPoint;
    try
    {
        for(int i = 1; i < splineControlPoints.size()-2; i++)
        {
            for(int j = 0;j<100;j++)
            {
                curPoint = interpolator.interpolateForT((float)j/(float)100, splineControlPoints, i);
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

    glEnd();

}

void BasicOpenGLView::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    projectionMatrix.setToIdentity();
    projectionMatrix.perspective(60.0f, (float)width/(float)height, 0.01f, 50.0f);
    glLoadMatrixd(projectionMatrix.data());

    glMatrixMode(GL_MODELVIEW);
}

void BasicOpenGLView::mousePressEvent(QMouseEvent *event)
{
    // TODO deal with mouse buttons

    qDebug() << "mouse clicked\n";
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

            //  7. calculate the direction vector between the current point and the ray origin
            //  8. calculate the dot product between the former result and the ray direction
            //  9. if ray directions lenght != 1 then normalize the result of 8 according to ray length
            //  10. calculate point on ray with t being the result of 9
            //  11. calculate the distance vector between result of 10 and current spline control point
            //  12. check the length of the distance vector. if smaller than threshold, we have a hit.
    }

    hoverIndex = -1;
}
