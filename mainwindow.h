#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    enum{UP = 0,
        RIGHT = 1,
        DOWN = 2,
        LEFT = 3};

private:
    Ui::MainWindow *ui;
    QSize size;
    void keyPressEvent(QKeyEvent *event);
private slots:
    void updatePoints(int);

};
#endif // MAINWINDOW_H
