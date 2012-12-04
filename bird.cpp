#include "bird.h"
#include <math.h>
#include <time.h>
#include <QVector3D>
#include <QVector>
#include "flock.h"
#include <math.h>
#include <float.h>
#include <OpenGL/glu.h>
#include <QDebug>

Bird::Bird()
{

}

Bird::Bird(float x, float y, float z, int i)
{
    this->acceleration = QVector3D(0,0,0);
    srand ( time(NULL) );
    float velocityX,velocityY,velocityZ;
    velocityX = -1+2*((float)rand())/(float)RAND_MAX;
    velocityY = -1+2*((float)rand())/(float)RAND_MAX;
    velocityZ = -1+2*((float)rand())/(float)RAND_MAX;


    this->velocity = QVector3D(velocityX,velocityY,velocityZ);

    this->position = QVector3D(x,y,z);

    r = 2.0f;
    maxvelocity = 2;
    maxforce = .03f;
    //qDebug("%d", i);
}

void Bird::animate(QVector<Bird> birds) //run
{
    flock(birds);
    update();
    draw();
}

void Bird::applyForce(QVector3D force)
{
    acceleration += force;
}

void Bird::flock(QVector<Bird> birds)
{
    QVector3D separation = separate(birds);
    QVector3D alignment = align(birds);
    QVector3D cohesion = cohes(birds);

    separation *= 2.5;
    alignment *= 1.0f;
    cohesion *= 1.0f;

    applyForce(separation);
    applyForce(alignment);
    applyForce(cohesion);
}

void Bird::update()
{
    velocity+=acceleration;
    velocity = limit(velocity,maxvelocity);
    position+=velocity;
    acceleration = QVector3D(0,0,0);
}

QVector3D Bird::limit(QVector3D vec, float limit)
{
    QVector3D temp;
    if(vec.length() < limit)
        //if(vec.x() < limit && vec.y() < limit && vec.z() < limit )
    {
        return vec;
    }
    else
    {
        /*if(vec.x() > limit )
        {
            vec.setX(limit);
        }
        if(vec.y() > limit)
        {
            vec.setY(limit);
        }
        if(vec.z() > limit)
        {
            vec.setZ(limit);
        }*/
        temp = vec.normalized();
        //temp*= limit;
        return temp;
    }
}



QVector3D Bird::seek(QVector3D target)
{
    //might be backwards
    QVector3D desired = target - position;
    desired.normalize();
    desired*= maxvelocity;
    //might be backwards
    QVector3D steer = desired - velocity;
    limit(steer,maxforce);

    return steer;
}

void Bird::draw() //render
{
    //float angle = getAngle(velocity) + 1.57079633;
    //QQuaternion thisQuat = QQuaternion(position.x(),position.y(),position.z(),0);
    //QQuaternion zeroQuat = (0,0,0,0);
    //QQuaternion = QQuaternion::slerp(thisQuat,zeroQuat);
    glPushMatrix();
    glTranslatef(position.x(),position.y(),position.z());
    glColor3f(1.0,0,1.0);
    //glRotatef();
    //qDebug("drawing1");
    GLUquadricObj *quadric;
    quadric = gluNewQuadric();
    gluQuadricDrawStyle(quadric, GLU_FILL );
    gluSphere(quadric,1,100,100);
    gluDeleteQuadric(quadric);
    glPopMatrix();
    //qDebug("drawing2");

}

QVector3D Bird::separate(QVector<Bird> birds)
{
    float desiredSeparation = 25.0f;
    QVector3D steer = QVector3D(0,0,0);
    int count = 0;

    QVector<Bird>::iterator bird;
    for(bird = birds.begin(); bird != birds.end(); bird++)
    {
        float d = (position-(*bird).position).length();//QVector3D::distanceToLine(this->position,(*bird).position);
        //QVector3D::
        if(d>0 && d < desiredSeparation)
        {
            QVector3D diff = position - (*bird).position;
            diff.normalize();
            diff /= d;
            steer += diff;
            count++;
        }
    }
    if(count > 0)
    {
        steer /= (float)count;
    }
    if(steer.length() > 0)
    {
        steer.normalize();
        steer *= maxvelocity;
        steer -= velocity;
    }

    steer = limit(steer, maxforce);


    return steer;
}

QVector3D Bird::align(QVector<Bird> birds)
{
    float neighbordist = 50;
    QVector3D sum = QVector3D(0,0,0);
    int count = 0;
    QVector<Bird>::iterator bird;
    for(bird = birds.begin(); bird != birds.end(); bird++)
    {
        float d = (position - (*bird).position).length();
        if ((d > 0) && (d < neighbordist)) {
            sum+= (*bird).velocity;
            count++;
        }
    }
    if (count > 0)
    {
        sum /= (float)count;
        sum.normalize();
        sum *= maxvelocity;
        QVector3D steer = sum-velocity;
        limit(steer,maxforce);
        return steer;
    }
    else
    {
        return QVector3D(0,0,0);
    }
}

QVector3D Bird::cohes(QVector<Bird> birds)
{
    float neighbordist = 50;
    QVector3D sum = QVector3D(0,0,0);
    int count = 0;
    QVector<Bird>::iterator bird;
    for(bird = birds.begin(); bird != birds.end(); bird++)
    {
        float d = (position - (*bird).position).length();
        if ((d > 0) && (d < neighbordist)) {
            sum+= (*bird).position;
            count++;
        }
    }

    if (count > 0)
    {
        sum /=count;
        return seek(sum);  // Steer towards the location
    }
    else
    {
        return QVector3D(0,0,0);
    }

}
