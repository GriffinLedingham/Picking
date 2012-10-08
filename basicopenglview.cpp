#include "basicopenglview.h"

#include "catmullrominterpolation.h"

#include <QMouseEvent>

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
//    splineControlPoints.push_back(QVector3D(2.0f, 0.0f, 0));
}

void BasicOpenGLView::initializeGL()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);


}

void BasicOpenGLView::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    viewMatrix.setToIdentity();
    viewMatrix.lookAt(QVector3D(0.0f, 2.0f, -3.5f),
                      QVector3D(0.0f, 0.1f, 0.0f),
                      QVector3D(0.0f, 1.0f, 0.0f));
    glLoadMatrixd(viewMatrix.data());



    CatmulRomInterpolation interpolator(splineControlPoints);

    size_t numVertices = interpolator.getVertexCountForSubdivisionLevel(4);

    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_POINTS);

    for(size_t i = 0; i < numVertices; ++i)
    {
        float interpolatedParameter = (float) i / numVertices;
        try
        {
            QVector3D curPoint = interpolator.interpolateForT(interpolatedParameter);

            glVertex3d(curPoint.x(), curPoint.y(), curPoint.z());
        }
        catch(...) { }
    }
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
