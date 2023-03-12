#include "logic.h"
#include "filehandler.h"
#include "drawhandler.h"
#include "metrichandler.h"
#include <qDebug>

void loadingData (Union* unionForWork, Result* result, string fileName);
void drawing (Union* unionForWork, Result* result);
void rotating (Union* unionForWork, Result* result, OperationData* data);
void offsetting (Union* unionForWork, Result* result, OperationData* data);
void normalizing (Union* unionForWork, Result* result, OperationData* data);
void splitString(string input, string* output);  // Разделить строку по запятым
void setPoints(Point** points, int rows, int cols, Union* unionForWork); // Создание точек из считанных данных

Result* execute(OperationData* data, Union* unionForWork)
{
    Result* result;
    try {
        result = new Result;
    } catch (exception) {
        return nullptr;
    }
    switch (data->operation){
        case Operations::LOAD_DATA:    // Загрузка данных из файла
            loadingData(unionForWork, result, data->fileName);
            unionForWork->lineCount = (unionForWork->matsize - 1) * unionForWork->matsize * 2;
            break;
        case Operations::CLEAR: // Удаление матрицы
            freePointMatrix(unionForWork->points, unionForWork->matsize);
            if (unionForWork->lines != nullptr)
                delete [] unionForWork->lines;
            break;
        case Operations::DRAW:    // Отрисовка точек
            drawing(unionForWork, result);
            break;
        case Operations::ROTATE: // Поворот точек по одной из осей
            rotating(unionForWork, result, data);
            break;
        case Operations::OFFSET:  // Перемещение по поверхности
            offsetting(unionForWork, result, data);
            break;
        case Operations::NORMALIZE: // Процесс нормализации, сокращение кол-ва точек
            normalizing(unionForWork, result, data);
            break;
    }
    return result;
}


void splitString(string input, string* output){ //разделение элементов
    string delimiter = ",";
    int i = 0;
    size_t pos = 0;
    while ((pos = input.find(delimiter)) != string::npos){  //npos - несуществующая позиция
        string str = input.substr(0, pos);
        output[i] = str;
        i++;
        input.erase(0, pos + delimiter.length()); // удаляем элемент
    }
    output[i] = input;
}

void setPoints(Point** points, int rows, int cols, Union* unionForWork){  // Установка точек в 3D пространстве (из данных из файла)
    string arr[unionForWork->matsize];
    for (int i = 0; i < rows; i++){
        splitString((*unionForWork->loadedData)[i], arr);
        for (int j = 0; j < cols; j++){
            Point point;
            setPoint(&point, (double)i, (double)j, stod(arr[j]));
            points[i][j] = point;
        }
    }
}


void loadingData (Union* unionForWork, Result* result, string fileName){
    if (unionForWork->loaded){
        unionForWork->loadedData->clear();
    }
    unionForWork->loadedData = loadData(fileName);
    if (unionForWork->loadedData != nullptr){
        unionForWork->loaded = true;
        unionForWork->matsize = unionForWork->loadedData->size();
        unionForWork->lines = new Line[(unionForWork->matsize - 1) * unionForWork->matsize * 2];
    } else {
        unionForWork->loaded = false;
        result->done = false;
        result->message = "Не удалось загрузить данные";
    }
}

void drawing (Union* unionForWork, Result* result){
    if (unionForWork->loaded){
        if (unionForWork->matrixCreated)  // Очищение поля для отрисовки, если матрица уже создана
            freePointMatrix(unionForWork->points, unionForWork->matsize);
        // Выделяем память под матрицы точек
        unionForWork->points = createPointMatrix(unionForWork->matsize, unionForWork->matsize);
        unionForWork->matrixCreated = true;
        setPoints(unionForWork->points, unionForWork->matsize, unionForWork->matsize, unionForWork); // Заполняем созданную матрицу точками
        normalize(unionForWork->points, unionForWork->matsize, unionForWork->matsize, unionForWork->normalization); // Нормируем значения
        createLines(unionForWork->points, unionForWork->lines, unionForWork->matsize, unionForWork->matsize);  // Создает линии между точками
    } else {
        result->done = false;
        result->message = "Данные не загружены // falled on drawing";
    }
}

void rotating (Union* unionForWork, Result* result, OperationData* data){
    if (unionForWork->loaded){
        rotate(unionForWork->points, unionForWork->matsize, unionForWork->matsize, data->axis, data->rotationAngle); // Поворот
        normalize(unionForWork->points, unionForWork->matsize, unionForWork->matsize, unionForWork->normalization); // Нормируем
        createLines(unionForWork->points, unionForWork->lines, unionForWork->matsize, unionForWork->matsize); // Создает линии между точками
    } else {
        result->done = false;
        result->message = "Данные не загружены // falled on rotating";
    }
}

void offsetting (Union* unionForWork, Result* result, OperationData* data){
    if (unionForWork->loaded){
        offset(unionForWork->points, unionForWork->matsize, unionForWork->matsize, data->axis, data->offsetValue); // Двигаем точки
        createLines(unionForWork->points, unionForWork->lines,unionForWork->matsize, unionForWork->matsize); // Создает линии между точками
    } else {
        result->done = false;
        result->message = "Данные не загружены // falled on ofsetting";
    }
}

void normalizing (Union* unionForWork, Result* result, OperationData* data){
    unionForWork->normalization[0] = data->normalization[0];
    unionForWork->normalization[1] = data->normalization[1];
    if (unionForWork->loaded){
        normalize(unionForWork->points, unionForWork->matsize, unionForWork->matsize, unionForWork->normalization);
        createLines(unionForWork->points, unionForWork->lines, unionForWork->matsize, unionForWork->matsize);
    } else {
        result->done = false;
        result->message = "Значения нормализации установлены. Данные не загружены";
    }
}
