#include "catmullrominterpolation.h"
#include <cmath>

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

QVector3D CatmulRomInterpolation::interpolateForT(float t)
{
    if(mPoints.size() < 2)
    {
        throw std::exception();
    }
    unsigned int index = getIndexForT(t);
    if(index == 0 || index > mPoints.size() - 3)
    {
        throw std::exception();
    }

    QVector3D output;

    // TODO do the catmull rom interpolation here
    return output;
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
    return 0;
}
