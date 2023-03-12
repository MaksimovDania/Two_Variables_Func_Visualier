#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "logic.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>
#include <exception.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void buttonSettings(bool n);

private slots:
    void on_fileDialog_btn_clicked();
    void on_fileLoad_btn_clicked();
    void on_draw_btn_clicked();
    void on_rotate_x_btn_clicked();
    void on_rotate_y_btn_clicked();
    void on_rotate_z_btn_clicked();
    void on_offset_x_btn_clicked();
    void on_offset_y_btn_clicked();
    void on_offset_z_btn_clicked();
    void on_rotate_x_back_btn_clicked();
    void on_rotate_y_back_btn_clicked();
    void on_rotate_z_back_btn_clicked();
    void on_normalization_btn_clicked();
    void on_pushButton_clicked();

    //void on_plus_or_minus_btn_clicked(int i);
    void on_plus_btn_clicked();
    void on_minus_btn_clicked();
    void do_scaling ();
    QColor getMyColor(int param);
    void normalize();
private:
    Ui::MainWindow *ui;
    QPixmap *pix = nullptr; // Холст для рисования
    QString fileName;
    Union* unionForWork;
    void displayError(std::string message);

    void drawSurface(Line* lines, int count); // Рисовать поверхность
    void drawLine(QPainter* paint, Line* line); // Рисовать линию

    void handleRotation(Axis axis, int direction = 1); // Повернуть
    void hadleOffset(Axis axis); // Переместить
    void catchSignal();
    void drawWithRotation (OperationData* request);
    bool checkingFormat(string check);
    void drawingWithOffsetting (OperationData* request);

};

#endif // MAINWINDOW_H
