#include "flock.h"
#include <QVector>

Flock::Flock()
{
    birds = QVector<Bird>();
}

void Flock::animate()
{
    QVector<Bird>::iterator bird;
        for(bird = birds.begin(); bird != birds.end(); bird++)
        {
            (*bird).animate(birds);
        }
}

void Flock::newBird(Bird bird)
{
    birds.push_back(bird);
}
