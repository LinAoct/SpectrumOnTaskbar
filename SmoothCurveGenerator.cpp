#include "SmoothCurveGenerator.h"


QPainterPath SmoothCurveGenerator1::generateSmoothCurve(const QVector<QPointF> &points)
{
    if (points.size() == 0)
    {
        return QPainterPath();
    }

    QPainterPath path(points[0]);

    for(int i = 0; i < points.size() - 1; ++i)
    {
        // 控制点的 x 坐标为 sp 与 ep 的 x 坐标和的一半
        // 第一个控制点 c1 的 y 坐标为起始点 sp 的 y 坐标
        // 第二个控制点 c2 的 y 坐标为结束点 ep 的 y 坐标
        QPointF sp = points[i];
        QPointF ep = points[i + 1];
        QPointF c1 = QPointF((sp.x() + ep.x()) / 2, sp.y());
        QPointF c2 = QPointF((sp.x() + ep.x()) / 2, ep.y());
        path.cubicTo(c1, c2, ep);
    }

    return path;
}


QPainterPath SmoothCurveGenerator2::generateSmoothCurveImp(const QVector<QPointF> &points)
{
    QPainterPath path;
    int len = points.size();

    if (len < 2)
    {
        return path;
    }

    QVector<QPointF> firstControlPoints;
    QVector<QPointF> secondControlPoints;
    calculateControlPoints(points, &firstControlPoints, &secondControlPoints);

    path.moveTo(points[0].x(), points[0].y());

    // Using bezier curve to generate a smooth curve.
    for(int i = 0; i < len - 1; ++i)
    {
        path.cubicTo(firstControlPoints[i], secondControlPoints[i], points[i + 1]);
    }

    return path;
}

QPainterPath SmoothCurveGenerator2::generateSmoothCurve(const QVector<QPointF> &points)
{
    QPainterPath result;

    int segmentStart = 0;
    int i = 0;
    int pointSize = points.size();
    while(i < pointSize)
    {
        if (qIsNaN(points.at(i).y()) || qIsNaN(points.at(i).x()) || qIsInf(points.at(i).y()))
        {
            // QVector<QPointF> lineData = QVector<QPointF>(points.constBegin() + segmentStart, points.constBegin() + i - segmentStart);
            QVector<QPointF> lineData = points.mid(0, i - segmentStart);

            result.addPath(generateSmoothCurveImp(lineData));
            segmentStart = i + 1;
        }
        ++i;
    }
    // QVector<QPointF> lineData(QVector<QPointF>(points.constBegin() + segmentStart, points.constEnd()));
    QVector<QPointF> lineData = points.mid(segmentStart, points.count() - segmentStart);
    result.addPath(generateSmoothCurveImp(lineData));
    return result;
}

QPainterPath SmoothCurveGenerator2::generateSmoothCurve(const QPainterPath &basePath, const QVector<QPointF> &points)
{
    if (points.isEmpty())
        return basePath;

    QPainterPath path = basePath;
    int len = points.size();
    if (len == 1)
    {
        path.lineTo(points.at(0));
        return path;
    }

    QVector<QPointF> firstControlPoints;
    QVector<QPointF> secondControlPoints;
    calculateControlPoints(points, &firstControlPoints, &secondControlPoints);

    path.lineTo(points.at(0));
    for(int i = 0; i < len - 1; ++i)
        path.cubicTo(firstControlPoints[i], secondControlPoints[i], points[i + 1]);

    return path;
}

void SmoothCurveGenerator2::calculateFirstControlPoints(double *&result, const double *rhs, int n)
{
    result = new double[n];
    double *tmp = new double[n];
    double b = 2.0;
    result[0] = rhs[0] / b;

    // Decomposition and forward substitution.
    for(int i = 1; i < n; i++)
    {
        tmp[i] = 1 / b;
        b = (i < n - 1 ? 4.0 : 3.5) - tmp[i];
        result[i] = (rhs[i] - result[i - 1]) / b;
    }

    for(int i = 1; i < n; i++)
    {
        result[n - i - 1] -= tmp[n - i] * result[n - i];  // Backsubstitution.
    }

    delete[] tmp;
}

void SmoothCurveGenerator2::calculateControlPoints(const QVector<QPointF> &knots, QVector<QPointF> *firstControlPoints, QVector<QPointF> *secondControlPoints)
{
    int n = knots.size() - 1;

    firstControlPoints->reserve(n);
    secondControlPoints->reserve(n);

    for(int i = 0; i < n; ++i)
    {
        firstControlPoints->append(QPointF());
        secondControlPoints->append(QPointF());
    }

    if (n == 1)
    {
        // Special case: Bezier curve should be a straight line.
        // P1 = (2P0 + P3) / 3
        (*firstControlPoints)[0].rx() = (2 * knots[0].x() + knots[1].x()) / 3;
        (*firstControlPoints)[0].ry() = (2 * knots[0].y() + knots[1].y()) / 3;

        // P2 = 2P1 – P0
        (*secondControlPoints)[0].rx() = 2 * (*firstControlPoints)[0].x() - knots[0].x();
        (*secondControlPoints)[0].ry() = 2 * (*firstControlPoints)[0].y() - knots[0].y();

        return;
    }

    // Calculate first Bezier control points
    double *xs = nullptr;
    double *ys = nullptr;
    double *rhsx = new double[n];  // Right hand side vector
    double *rhsy = new double[n];  // Right hand side vector

    // Set right hand side values
    for(int i = 1; i < n - 1; ++i)
    {
        rhsx[i] = 4 * knots[i].x() + 2 * knots[i + 1].x();
        rhsy[i] = 4 * knots[i].y() + 2 * knots[i + 1].y();
    }
    rhsx[0] = knots[0].x() + 2 * knots[1].x();
    rhsx[n - 1] = (8 * knots[n - 1].x() + knots[n].x()) / 2.0;
    rhsy[0] = knots[0].y() + 2 * knots[1].y();
    rhsy[n - 1] = (8 * knots[n - 1].y() + knots[n].y()) / 2.0;

    // Calculate first control points coordinates
    calculateFirstControlPoints(xs, rhsx, n);
    calculateFirstControlPoints(ys, rhsy, n);

    // Fill output control points.
    for(int i = 0; i < n; ++i)
    {
        (*firstControlPoints)[i].rx() = xs[i];
        (*firstControlPoints)[i].ry() = ys[i];

        if (i < n - 1)
        {
            (*secondControlPoints)[i].rx() = 2 * knots[i + 1].x() - xs[i + 1];
            (*secondControlPoints)[i].ry() = 2 * knots[i + 1].y() - ys[i + 1];
        }
        else
        {
            (*secondControlPoints)[i].rx() = (knots[n].x() + xs[n - 1]) / 2;
            (*secondControlPoints)[i].ry() = (knots[n].y() + ys[n - 1]) / 2;
        }
    }

    delete xs;
    delete ys;
    delete[] rhsx;
    delete[] rhsy;
}
