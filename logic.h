#ifndef unionForWork_H
#define unionForWork_H

#include <vector>
#include <string>
#include <QDebug>

#include "drawhandler.h"

using namespace std;

enum Operations{
    LOAD_DATA,
    DRAW,
    ROTATE,
    OFFSET,
    NORMALIZE,
    CLEAR
};

struct OperationData{
    int operation;
    string fileName;
    Axis axis;
    double rotationAngle;
    double offsetValue;
    double normalization[2];
};

struct Result{
    bool done = true;
    string message;
};

struct Union
{
    double angleX = 0;
    double angleY = 0;
    double angleZ = 0;
    int lineCount; // количество линий между точками
    int matsize;  // Размер матрицы
    vector<string> *loadedData;  // Данные из файла посточно
    bool loaded = false; // Загружены ли данные
    bool drawed = false; // Нарисована ли модель
    bool matrixCreated = false; // Создана ли матрица
    Point** points;  // Матрица точек
    Line* lines;  // Линии, соединяющие точки
    double normalization[2] = {50, 400}; // Диапозон нормировки
    string suf;

};

Result* execute(OperationData* request, Union* unionForWork);
#endif // unionForWork_H

