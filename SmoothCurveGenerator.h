#ifndef SMOOTHCURVEGENERATOR_H
#define SMOOTHCURVEGENERATOR_H

#include <QList>
#include <QPainterPath>
#include <QPointF>

class SmoothCurveGenerator1
{
public:
    /**
     * 传入曲线上的点的 list，创建平滑曲线
     *
     * @param points - 曲线上的点
     * @return - 返回使用给定的点创建的 QPainterPath 表示的平滑曲线
     */
    static QPainterPath generateSmoothCurve(const QVector<QPointF> &points);
};

class SmoothCurveGenerator2
{
protected:
    static QPainterPath generateSmoothCurveImp(const QVector<QPointF> &points);

public:
    /**
     * 传入曲线上的点的 list，创建平滑曲线
     * @param points - 曲线上的点
     * @return - 返回使用给定的点创建的 QPainterPath 表示的平滑曲线
     */
    static QPainterPath generateSmoothCurve(const QVector<QPointF> &points);
    static QPainterPath generateSmoothCurve(const QPainterPath &basePath, const QVector<QPointF> &points);

private:
    /**
     * Solves a tridiagonal system for one of coordinates (x or y)
     * of first Bezier control points.
     * @param result - Solution vector.
     * @param rhs - Right hand side vector.
     * @param n - Size of rhs.
     */
    static void calculateFirstControlPoints(double *&result, const double *rhs, int n);

    /**
     * Calculate control points of the smooth curve using the given knots.
     * @param knots - Points of the given curve.
     * @param firstControlPoints - Store the generated first control points.
     * @param secondControlPoints - Store the generated second control points.
     */

    static void calculateControlPoints(const QVector<QPointF> &knots,
                                        QVector<QPointF> *firstControlPoints,
                                        QVector<QPointF> *secondControlPoints);
};

#endif  // SMOOTHCURVEGENERATOR_H
