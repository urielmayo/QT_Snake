#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->render_area->setFixedSize(980,560);
    this->adjustSize();
    ui->puntaje->setText(QString::number(0));
    connect(ui->render_area,SIGNAL(aumentarPuntaje(int)),this,SLOT(updatePoints(int)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    int direccion = 5;
    switch (event->key())
    {
    case Qt::Key_W: direccion = UP;
        break;

    case Qt::Key_A: direccion = LEFT;
        break;

    case Qt::Key_S: direccion = DOWN;
        break;

    case Qt::Key_D: direccion = RIGHT;
        break;
    default: break;
    }
    if(direccion < 4 ){
        ui->render_area->move(direccion);
        ui->render_area->repaint();
    }
}

void MainWindow::updatePoints(int valor)
{
    ui->puntaje->setText(QString::number(valor));
}
