#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logic.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowCloseButtonHint);
    unionForWork = new Union();
    buttonSettings(false);
}

void MainWindow::buttonSettings(bool n){
    ui->draw_btn->setEnabled(n);
    ui->fileLoad_btn->setEnabled(n);
    ui->offset_x_btn->setEnabled(n);
    ui->offset_y_btn->setEnabled(n);
    ui->offset_z_btn->setEnabled(n);
    ui->rotate_x_back_btn->setEnabled(n);
    ui->rotate_y_back_btn->setEnabled(n);
    ui->rotate_z_back_btn->setEnabled(n);
    ui->rotate_x_btn->setEnabled(n);
    ui->rotate_y_btn->setEnabled(n);
    ui->rotate_z_btn->setEnabled(n);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete pix;
    delete unionForWork;
}


void MainWindow::on_fileDialog_btn_clicked(){
    fileName = QFileDialog::getOpenFileName(this, "Choose file", QDir::currentPath(), "CSV Table (*csv)");
    if (this->fileName == ""){
        QMessageBox::critical(this,"Error", "You did not choose file");
        buttonSettings(false);
        if (pix != nullptr)
            delete pix;
        try {
            pix = new QPixmap(450, 450);
        } catch (_exception) {
            pix = nullptr;
            QMessageBox::critical(this,"Error", "Memory error");
            return;
        }
        QPainter paint(pix);
        paint.fillRect(0, 0, 450, 450, QBrush(QColor(Qt::GlobalColor::white)));
        ui->draw_label->setPixmap(*pix);
    } else {
            ui->fileLoad_btn->setEnabled(true);
            ui->fileName_label->setText(fileName);
            ui->doIt_label->setText("Enter Load File");
    }
}

void MainWindow::on_fileLoad_btn_clicked(){  // Считывание файла и сохранение в LoadData в unionForWork
    buttonSettings(true);
    OperationData* request_1 = tryInitOperationData();
    Result* response_1 = tryInitResult();
    OperationData* request_2 = tryInitOperationData();
    Result* response_2 = tryInitResult();
    if (request_1 == nullptr || response_1 == nullptr || request_2 == nullptr || response_2 == nullptr){
        QMessageBox::critical(this,"Error", "Memory error");
        return;
    }
    request_1->operation = Operations::CLEAR;
    response_1 = execute(request_1, unionForWork);
    if (response_1 == nullptr)
        {QMessageBox::critical(this,"Error", "Memory error"); return;}
    delete request_1;
    delete response_1;
    request_2->operation = Operations::LOAD_DATA;
    request_2->fileName = fileName.toStdString();
    response_2 = execute(request_2, unionForWork);
    if (response_2 == nullptr || !response_2->done){
        displayError(response_2->message);
        return;
    }
    delete request_2;
    delete response_2;
    ui->doIt_label->setText("Enter Draw");
}

void MainWindow::on_draw_btn_clicked(){ // Отрисовка на QPix точек
    OperationData* request = tryInitOperationData();
    Result* response = tryInitResult();
    if (request == nullptr || response == nullptr){
        QMessageBox::critical(this,"Error", "Memory error");
        return;
    }
    request->operation = Operations::DRAW;
    response = execute(request,unionForWork);
    if (response == nullptr || !response->done){
        displayError(response->message);
        return;
    }
    ui->doIt_label->setText("");
    drawSurface(unionForWork->lines, unionForWork->lineCount); // Рисуем поверхность
    delete request;
    delete response;
    on_plus_btn_clicked();
}

void MainWindow::on_rotate_x_btn_clicked(){
    handleRotation(Axis::X, 1);
}

void MainWindow::on_rotate_y_btn_clicked(){
    handleRotation(Axis::Y, 1);
}

void MainWindow::on_rotate_z_btn_clicked(){
    handleRotation(Axis::Z, 1);
}

void MainWindow::on_rotate_x_back_btn_clicked(){
    handleRotation(Axis::X, -1);
}

void MainWindow::on_rotate_y_back_btn_clicked(){
    handleRotation(Axis::Y, -1);
}

void MainWindow::on_rotate_z_back_btn_clicked(){
    handleRotation(Axis::Z, -1);
}

void MainWindow::on_offset_x_btn_clicked(){
    hadleOffset(Axis::X);
}

void MainWindow::on_offset_y_btn_clicked(){
    hadleOffset(Axis::Y);
}

void MainWindow::on_offset_z_btn_clicked(){
    hadleOffset(Axis::Z);
}

void MainWindow::on_normalization_btn_clicked(){
        unionForWork->normalization[0] = ui->normalization_a->toPlainText().toDouble();
        unionForWork->normalization[1] = ui->normalization_b->toPlainText().toDouble();
        normalize();
}

void MainWindow::normalize (){
    if ((unionForWork->normalization[1] < 0) || (unionForWork->normalization[0]) > 450){
        ui->pred->setText("Картина за пределами холста");
    } else if ((unionForWork->normalization[0] <= 0) || ((unionForWork->normalization[1]) >= 450)){
        ui->pred->setText("Картина выходит за пределы холста");
    } else
        ui->pred->setText("");
    OperationData* request = tryInitOperationData();
    Result* response = tryInitResult();
    if (request == nullptr || response == nullptr){
        QMessageBox::critical(this,"Error", "Memory error");
        return;
    }
    request->operation = Operations::DRAW;
    response = execute(request,unionForWork);
    if (response == nullptr || !response->done){
        displayError(response->message);
        return;
    }
    ui->doIt_label->setText("");
    drawSurface(unionForWork->lines, unionForWork->lineCount); // Рисуем поверхность
    delete request;
    delete response;
}


void MainWindow::drawSurface(Line* lines, int count){ // Рисовать поверхность
    delete pix;
    try {
        pix = new QPixmap(450, 450);
    }  catch (_exception) {
        pix = nullptr;
        QMessageBox::critical(this,"Error", "Memory error");
        return;
    }
    QPainter paint(pix);
    paint.fillRect(0, 0, 450, 450, QBrush(QColor(Qt::GlobalColor::white)));
    QColor color;
    QBrush br(color);
    int sizeOfPoint = 8;
    for (int i = 0; i < unionForWork->matsize; i++){
        for(int j = 0; j < unionForWork->matsize; j++){
            Point curPoint = unionForWork->points[i][j];
            color = getMyColor(curPoint.z);
            br.setColor(color);
            paint.setBrush(br);
            paint.drawEllipse(curPoint.x - sizeOfPoint/2.0, curPoint.y - sizeOfPoint/2.0, sizeOfPoint, sizeOfPoint);
        }
    }
    QPen pen(Qt::darkGray);
    pen.setWidth(1);
    paint.setPen(pen);
    for (int i = 0; i < count; i++) {
        drawLine(&paint, &(lines[i])); // Рисовать каждую линию
    }
    ui->draw_label->setPixmap(*pix);
}

QColor MainWindow::getMyColor(int param){
    int minim = 100;
    int current = max(0, min(255 - minim + param * 2, 255));
    QColor color = QColor(100, current, 255);
    return color;
}

void MainWindow::drawLine(QPainter* paint, Line* line){ // Рисовать линию
    paint->drawLine(line->start.x, line->start.y, line->end.x, line->end.y);
}

void MainWindow::displayError(string message){
    QMessageBox::about(this, "Ошибка", QString::fromStdString(message));
    buttonSettings(false);
    ui->doIt_label->setText("Enter Choose File");
}

void MainWindow::handleRotation(Axis axis, int direction){ // Повернуть
    normalize();
    OperationData* request = tryInitOperationData();
    if (request == nullptr){
        QMessageBox::critical(this,"Error", "Memory error");
        return;
    }
    request->operation = Operations::ROTATE;
    request->axis = axis;
    int a = request->axis;
    switch (a){
        case Axis::X:
            unionForWork->angleX += ui->rotation_x->toPlainText().toDouble() * direction;
            break;
        case Axis::Y:
            unionForWork->angleY += ui->rotation_y->toPlainText().toDouble() * direction;
            break;
        case Axis::Z:
            unionForWork->angleZ += ui->rotation_z->toPlainText().toDouble() * direction;
            break;
    }
    drawWithRotation(request);
    delete request;
}

void MainWindow::drawWithRotation (OperationData* request){
    Result* response = tryInitResult();
    if (response == nullptr){
        QMessageBox::critical(this,"Error", "Memory error");
        return;
    }
    request->axis = X;
    request->rotationAngle = unionForWork->angleX;
    response = execute(request,unionForWork);
    if (response == nullptr || !response->done){
        displayError(response->message);
    } else {
        request->axis = Y;
        request->rotationAngle = unionForWork->angleY;
        response = execute(request,unionForWork);
        if (response == nullptr || !response->done){
            displayError(response->message);
        } else {
            request->axis = Z;
            request->rotationAngle = unionForWork->angleZ;
            response = execute(request,unionForWork);
            if (response == nullptr || !response->done){
                displayError(response->message);
            } else
                drawSurface(unionForWork->lines, unionForWork->lineCount);
        }
    }
    delete response;
}

void MainWindow::hadleOffset(Axis axis){ // Переместить
    OperationData* request = tryInitOperationData();
    if (request == nullptr){
        QMessageBox::critical(this,"Error", "Memory error");
        return;
    }
    request->operation = Operations::OFFSET;
    request->axis = axis;
    switch (axis){
        case Axis::X:
            request->offsetValue = ui->offset_x->toPlainText().toDouble();
            break;
        case Axis::Y:
            request->offsetValue = ui->offset_y->toPlainText().toDouble();
            break;
        case Axis::Z:
            request->offsetValue = ui->offset_z->toPlainText().toDouble();
            break;
    }
    drawingWithOffsetting(request);
    delete request;
}

void MainWindow::drawingWithOffsetting (OperationData* request){
    Result* response = tryInitResult();
    if (response == nullptr){
        QMessageBox::critical(this,"Error", "Memory error");
        return;
    }
    response = execute(request, unionForWork);
    if (response == nullptr) {
        QMessageBox::critical(this,"Error", "Memory error");
        return;
    }
    if (!response->done){
        displayError(response->message);
        return;
    }
    drawSurface(unionForWork->lines, unionForWork->lineCount);

    delete response;
}

void MainWindow::on_minus_btn_clicked() {
    unionForWork->normalization[0] +=  ui->step->text().toInt();
    unionForWork->normalization[1] -=  ui->step->text().toInt();
    do_scaling ();
}

void MainWindow::on_plus_btn_clicked() {
    unionForWork->normalization[0] -=  ui->step->text().toInt();
    unionForWork->normalization[1] +=  ui->step->text().toInt();
    do_scaling ();
}

void MainWindow::do_scaling (){ // изменение масштаба
    if ((unionForWork->normalization[1] < 0) || (unionForWork->normalization[0]) > 450){
        ui->pred->setText("Картина за пределами холста");
    } else if ((unionForWork->normalization[0] <= 0) || ((unionForWork->normalization[1]) >= 450)){
        ui->pred->setText("Картина выходит за пределы холста");
    } else
        ui->pred->setText("");
    OperationData* request = tryInitOperationData();
    Result* response = tryInitResult();
    if (request == nullptr || response == nullptr){
        QMessageBox::critical(this,"Error", "Memory error");
        return;
    }
    request->operation = Operations::NORMALIZE;
    request->normalization[0] = unionForWork->normalization[0];
    request->normalization[1] = unionForWork->normalization[1];
    response = execute(request,unionForWork);
    if (response == nullptr) {
        QMessageBox::critical(this,"Error", "Memory error");
        return;
    }
    if (!response->done){
        displayError(response->message);
        return;
    }
    drawSurface(unionForWork->lines, unionForWork->lineCount);
    delete request;
    delete response;
}

