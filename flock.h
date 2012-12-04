#ifndef FLOCK_H
#define FLOCK_H

#include <Qvector>
#include "bird.h"

class Flock
{
public:
    Flock();
    void animate();
    void newBird(Bird);
private:
    QVector<Bird> birds;

};

#endif // FLOCK_H
