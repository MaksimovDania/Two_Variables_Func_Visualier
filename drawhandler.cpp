#include "drawhandler.h"
#include "metrichandler.h"
#include <QDebug>


// Выделяем память под матрицу
Point** createPointMatrix(int rows, int cols){ //row - ряд, coln - колонки
    Point** matrix;
    try {
        matrix = new Point* [rows];
        for (int i = 0; i < rows; i++){
            matrix[i] = new Point[cols];
        }
    } catch (_exception) {
        return nullptr;
    }
    return matrix;
}

void freePointMatrix(Point** matrix, int rows){ // Очистить матрицу
    //qDebug() << 11;
    if (matrix != nullptr){
        for (int i = 0; i < rows; i++){
            delete [] matrix[i];
        }
        delete matrix;
    }
}

void setPoint(Point* point, double x, double y, double z){ // Из трех координат сделать точку
    point->x = x;
    point->y = y;
    point->z = z;
}

void normalize(Point** points, int rows, int cols, double* normalization){ // Нормировка
    int size = rows * cols;
    double* valuesX;
    double* valuesY;
    double* valuesZ;
    try {
        valuesX = new double[size]; //массив из x координат
        valuesY = new double[size]; //массив из Y координат
        valuesZ = new double[size]; //массив из Z координат
    }  catch (_exception) {
        valuesX = NULL;
        valuesY = NULL;
        valuesZ = NULL;
    }
    if (valuesX != NULL && valuesY != NULL && valuesZ != NULL){
        getValues(points, valuesX, valuesY, valuesZ, rows, cols); // Достаем текущие точки из матрицы точек

        double minX = findMin(valuesX, size);
        double maxX = findMax(valuesX, size);
        double minY = findMin(valuesY, size);
        double maxY = findMax(valuesY, size);
        double minZ = findMin(valuesZ, size);
        double maxZ = findMax(valuesZ, size);

        double a = *normalization;
        double b = *(normalization + 1);
        // К каждой точке применить формулу // max - min это масштаб,
        for (int i = 0; i < rows; i++){
            for (int j = 0; j < cols; j++){
                // из х вычитаем мин, это расстояние, которое должно быть от низа до точки х, делим на масштаб и умножаем на новый масштаб и прибавляем новое минимальное значени
                points[i][j].x = a + ((points[i][j].x - minX) * (b - a) / (maxX - minX));
                points[i][j].y = a + ((points[i][j].y - minY) * (b - a) / (maxY - minY));
                points[i][j].z = a + ((points[i][j].z - minZ) * (b - a) / (maxZ - minZ));
            }
        }
        delete [] valuesX;
        delete [] valuesY;
        delete [] valuesZ;
    }
}

// Поворот
void rotate(Point** points, int rows, int cols, Axis axis, double angle){ //axis - оси, angle - угол
    // Для каждой точки применяем формулу
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            int a = axis;
            switch (a){
                case Axis::X:
                    rotateX(&points[i][j], angle);
                    break;

                case Axis::Y:
                    rotateY(&points[i][j], angle);
                    break;

                case Axis::Z:
                    rotateZ(&points[i][j], angle);
                    break;
            }
        }
    }
}

void rotateX(Point* point, double degrees){
    double angle = degrees / 180.0 * M_PI; //переводим в радианы
    double tmp = point->y;
    point->y = point->y * cos(angle) - point->z * sin(angle);
    point->z = point->z * cos(angle) + tmp * sin(angle);
}

void rotateY(Point* point, double degrees){
    double angle = degrees / 180.0 * M_PI;
    double tmp = point->x;
    point->x = point->x * cos(angle) + point->z * sin(angle);
    point->z = point->z * cos(angle) - tmp * sin(angle);
}

void rotateZ(Point* point, double degrees){
    double angle = degrees / 180.0 * M_PI;
    double tmp = point->x;
    point->x = point->x * cos(angle) - point->y * sin(angle);
    point->y = point->y * cos(angle) + tmp * sin(angle);
}

// Перемещение
void offset(Point** points, int rows, int cols, Axis axis, double offset){
    // Для каждой точки делаем + координата
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            int a = axis;
            switch (a){
                case Axis::X:
                    points[i][j].x += offset;
                    break;

                case Axis::Y:
                    points[i][j].y += offset;
                    break;

                case Axis::Z:
                    points[i][j].z += offset;
                    break;
            }
        }
    }
}

void getValues(Point** points, double* x, double* y, double* z, int rows, int cols){
    int index = 0;
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            x[index] = points[i][j].x;
            y[index] = points[i][j].y;
            z[index] = points[i][j].z;
            index++;
        }
    }
}

void createLines(Point** points, Line* lines, int rows, int cols){ // Просчет линий, связывающих точек
    int inx = 0;
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols - 1; j++){
            lines[inx].start.x = points[i][j].x;
            lines[inx].start.y = points[i][j].y;
            lines[inx].end.x = points[i][j + 1].x;
            lines[inx].end.y = points[i][j + 1].y;
            inx++;
        }
    }
    for (int i = 0; i < rows - 1; i++){
        for (int j = 0; j < cols; j++){
            lines[inx].start.x = points[i][j].x;
            lines[inx].start.y = points[i][j].y;
            lines[inx].end.x = points[i + 1][j].x;
            lines[inx].end.y = points[i + 1][j].y;
            inx++;
        }
    }
}
