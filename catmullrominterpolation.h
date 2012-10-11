#ifndef CATMULLROMINTERPOLATION_H
#define CATMULLROMINTERPOLATION_H

#include <QVector>
#include <QVector3D>

/**
 *	@class	CatmulRomInterpolation
 *	@brief	class for spline interpolation using Catmul Rom Splines
 */
class CatmulRomInterpolation
{
public:

    /**
     * default constructor
     */
    CatmulRomInterpolation();

    /**
     * Constructor without given time.
     * 1second between Points is assumed
     * @param m_Points list of given Vector3 - time Has to be added
     */
    CatmulRomInterpolation(QVector<QVector3D > & m_Points);

    /**
     *	interpolates the curve for the given t in the interval [0, 1]
     *	even for multi segment curves t stands for the whole curve interval
     *	and is adapted in the concrete interpolation
     *	@param t the parameter
     *	@return	returns the point on the curve
     */
    QVector3D interpolateForT(float t, int index);
    /**
     *	calculates the vertex count for the given subdiv level
     *	@param	level the subdiv level
     *	@return	returns the vertex count
     */
    size_t getVertexCountForSubdivisionLevel(size_t level);

    /**
      * returns the number of control points
      * @return returns the number of points in the vector
      */
    size_t getNumberOfControlPoints();

    /**
      * gets the controlpoint with the given indes
      * @param  index the given index
      * @return returns the vector at index. if index is bigger than the size of the
      *         vector, then the last one is returned
      */
    QVector3D getControlPoint(size_t index);

protected:

    /**
     *	returns the index of the appropriate point for the given t
     *	furthermore it modifies the t to fit in the corresponding range
     *	@param	t the right t
     *	@return	returns the proper index
     */
    size_t getIndexForT(float t);

    QVector<QVector3D>  mPoints;    /**< the spline control points. */
    QVector<float> mWeights;        /**< the weights used for interpolation. */

};


#endif // CATMULLROMINTERPOLATION_H
