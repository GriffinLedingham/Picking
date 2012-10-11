#include "catmullrominterpolation.h"
#include <cmath>
#include <qDebug>


CatmulRomInterpolation::CatmulRomInterpolation(QVector<QVector3D> & m_Points) : mPoints(m_Points), mWeights()
{

    float segmentPercent = 1.0f / (m_Points.size() - 3);
    float curValue = -segmentPercent;

    for (size_t i = 0; i < m_Points.size(); ++i)
    {
        mWeights.push_back(curValue);
        curValue += segmentPercent;
    }
}

QVector3D CatmulRomInterpolation::interpolateForT(float t, int index)
{
    QVector3D& p0 = mPoints[index-1];
    QVector3D& p1 = mPoints[index];
    QVector3D& p2 = mPoints[index+1];
    QVector3D& p3 = mPoints[index+2];

    return p0 * (((-t + 2) * t - 1) * t * 0.5f) + p1 * ((((3 * t - 5) * t) * t + 2) * 0.5f) + p2 * (((-3 * t + 4) * t + 1) * t * 0.5f) + p3 * (((t - 1) * t * t) * 0.5f);
}

size_t CatmulRomInterpolation::getNumberOfControlPoints()
{
    return mPoints.size();
}

QVector3D CatmulRomInterpolation::getControlPoint(size_t index)
{
    if(index >= mPoints.size())
    {
        index = mPoints.size() - 1;
    }
    return mPoints[index];
}

size_t CatmulRomInterpolation::getVertexCountForSubdivisionLevel(size_t level)
{
    return (mPoints.size() - 3) * pow(2, level) + 1;
}

size_t CatmulRomInterpolation::getIndexForT(float t)
{
    // TODO change this code here to find the index with the closest weight to t
    //return 0;
}
