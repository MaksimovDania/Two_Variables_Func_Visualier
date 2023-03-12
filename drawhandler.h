#ifndef DRAWHANDLER_H
#define DRAWHANDLER_H

#include <math.h>
//#define PI 3.14159265;

struct Point{
    double x;
    double y;
    double z;
};

struct Point2D{
    double x;
    double y;
};

struct Line{
    Point2D start;
    Point2D end;
};

enum Axis{
    X,
    Y,
    Z
};

void normalize(Point** points, int rows, int cols, double* normalization);
void rotate(Point** points, int rows, int cols, Axis axis, double angle);
void offset(Point** points, int rows, int cols, Axis axis, double offset);
Point** createPointMatrix(int rows, int cols);
void freePointMatrix(Point** matrix, int rows);
void setPoint(Point* point, double x, double y, double z);
void getValues(Point** points, double* x, double* y, double* z, int rows, int cols);
void createLines(Point** points, Line* lines, int rows, int cols);
// Повороты по каждой из осей
void rotateX(Point* point, double angle);
void rotateY(Point* point, double angle);
void rotateZ(Point* point, double angle);

#endif // DRAWHANDLER_H
