#ifndef BASICOPENGLVIEW_H
#define BASICOPENGLVIEW_H

#include <QGLWidget>
#include <QMatrix4x4>

/**
  * @class  BasicOpenGLView
  * @brief  our OpenGL view derived from QGLWidget.
  * We have to override several functions for our
  * application-specific OpenGL drawing functionality
  */
class BasicOpenGLView : public QGLWidget
{
    /**
      * macro needed to be inserted in every QT derived class.
      */
    Q_OBJECT
public:

    /**
      * our constructor
      * @param  parent the parent object of the widget.
                pass this one on to the superclass constructor
      */
    explicit BasicOpenGLView(QWidget *parent = 0);






signals:

public slots:

    void animateGL();
    void paintGL();


protected:

    /**
      * function called automatically by QT after the OpenGL context is created
      * in here, do all OpenGL setup stuff, that you can before the actual program
      * execution starts
      */
    void initializeGL();



    /**
      * method called when the widget is resized.
      * we need code in there, to react to the changed window size
      * otherwise the rendering will look skewed and ugly
      */
    void resizeGL(int width, int height);

    /**
      * function called when a mouse button is pressed and the cursor is hovering
      * over our widget
      * use the QMouseEvent classmembers to access the mouse information
      */




private:

    QMatrix4x4  projectionMatrix;       /**< the projectionMatrix to describe our camera projection parameters.*/
    QMatrix4x4  viewMatrix;             /**< the view matrix to describe the location and orientation of our camera. */




    int64_t hoverIndex;                          /**< the index of the overed spline control point. */

};

#endif // BASICOPENGLVIEW_H
