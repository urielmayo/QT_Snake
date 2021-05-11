#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>
#include <QKeyEvent>
#include <QTimer>
#include <QVector>
#include <QPainter>
#define U_LIMIT 0
#define L_LIMIT 0
#define R_LIMIT 980
#define D_LIMIT 560

class RenderArea : public QWidget
{
    Q_OBJECT

public:
    explicit RenderArea(QWidget *parent = nullptr);
    void move(int direccion);
    void free();
    void getPoints();

protected:
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *);

private: //attributtes
    const QPoint pInicial;
    QPoint actual, lastPosition;
    QRect cuadrado, fruit;
    QVector <QRect> cuerpo;
    bool inicio = true;
    int lastDirection;
    int updateTimerId, fruitTimerId;
    int points;
    bool isGameOver = false;

private slots:
    void locateFruit();
    void update();
    void resetRenderArea();

private: //methods
    bool checkDirections(int direction);
    void checkGameOver();
    bool fruitEaten();
    void resizeSnake();
    void setInitialPosition();

signals:
    void aumentarPuntaje(int);
    void s_fruitEaten();
    void signalGameOver();
};

#endif // RENDERAREA_H
