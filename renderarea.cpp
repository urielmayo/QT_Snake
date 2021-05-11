#include "renderarea.h"
#include "mainwindow.h"
#include <QPainter>
#include <QVector>
#include <QTime>
#include <QPen>
#include <QRandomGenerator>
#include <QDebug>
#include <QMessageBox>

RenderArea::RenderArea(QWidget *parent)
    : QWidget(parent)
    , pInicial(400,300)
    , cuerpo(3)
    , lastDirection(MainWindow::RIGHT)
    , points(0)

{
    setInitialPosition();
    fruit.setSize(QSize(10,10));
    locateFruit();
    updateTimerId = startTimer(200);
    fruitTimerId = startTimer(1);
    connect(this,SIGNAL(s_fruitEaten()),this,SLOT(locateFruit()));
    connect(this,SIGNAL(signalGameOver()),this,SLOT(resetRenderArea()));
}
void RenderArea::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setBrush(Qt::black);
    painter.drawRect(this->rect());
    if(!isGameOver){
        painter.setBrush(Qt::red);
        painter.drawRects(cuerpo);
        painter.setBrush(Qt::green);
        painter.drawRect(fruit);
    }


}

void RenderArea::setInitialPosition()
{
    cuerpo[0].setRect(pInicial.x(),pInicial.y(),10,10);
    for(int i = 1; i < 3; i++){
        cuerpo[i].setTopLeft(QPoint(cuerpo[i-1].topLeft().x()-10,cuerpo[i-1].topLeft().y()));
        cuerpo[i].setBottomRight(QPoint(cuerpo[i-1].bottomRight().x()-10,cuerpo[i-1].bottomRight().y()));
    }
}

void RenderArea::locateFruit()
{
    int topLeftX, topLeftY;
    bool canBeLocated = true;
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    topLeftX = qrand() % (R_LIMIT-10);
    if((topLeftX % 10) != 0){
        int rest = 10 - (topLeftX % 10);
        topLeftX+= rest;
    }
    topLeftY = qrand() % (D_LIMIT-10);
    if((topLeftY%10) != 0){
        int rest = 10 - (topLeftY % 10);
        topLeftY+= rest;
    }
    QPoint fruitTopLeft(topLeftX,topLeftY);
    for(int i = 0; i < cuerpo.size() && canBeLocated == true; i++)
    {
        if(fruitTopLeft == cuerpo[i].topLeft()){
            canBeLocated = false;
        }
    }
    if(canBeLocated)
    {
        fruit.moveTo(fruitTopLeft);
    }

}

void RenderArea::move(int direccion)
{
    if(checkDirections(direccion) == true)
    {
        for(int i = cuerpo.size()-1; i > 0; i--){
            cuerpo[i].moveTo(cuerpo[i-1].topLeft());
        }
        lastPosition.setX(cuerpo[cuerpo.size()-1].x());
        lastPosition.setY(cuerpo[cuerpo.size()-1].y());
        switch (direccion)
        {

        case MainWindow::UP:
            if(cuerpo[0].top() == U_LIMIT){
                cuerpo[0].translate(0,550);
                qDebug() <<"BOTTOM";
            }
            else{
                cuerpo[0].translate(0,-10);
            }
            lastDirection = MainWindow::UP;
            break;

        case MainWindow::RIGHT:
            if(cuerpo[0].right() >= R_LIMIT){
                cuerpo[0].translate(- R_LIMIT + 10,0);
            }
            else{
                cuerpo[0].translate(10,0);
            }
            lastDirection = MainWindow::RIGHT;
            break;

        case MainWindow::DOWN:
            if(cuerpo[0].bottom() >= D_LIMIT){
                qDebug() <<"BOTTOM";
                cuerpo[0].translate(0,-D_LIMIT+ 10);
            }
            else{
                cuerpo[0].translate(0,10);
            }
            lastDirection = MainWindow::DOWN;
            break;

        case MainWindow::LEFT:
            if(cuerpo[0].left() == L_LIMIT){
                cuerpo[0].translate(R_LIMIT- 10,0);
            }
            else{
                cuerpo[0].translate(-10,0);
            }
            lastDirection = MainWindow::LEFT;
            break;
        default: break;
        }
    }
}

void RenderArea::free()
{
    killTimer(updateTimerId);
    killTimer(fruitTimerId);
    while(!cuerpo.isEmpty())
    {
        cuerpo.pop_back();
    }
}

bool RenderArea::checkDirections(int direccion)
{
    bool res;
    switch(direccion)
    {
    case MainWindow::UP: res = (lastDirection != MainWindow::DOWN);
        break;
    case MainWindow::DOWN: res = (lastDirection != MainWindow::UP);
        break;
    case MainWindow::RIGHT: res = (lastDirection != MainWindow::LEFT);
        break;
    case MainWindow::LEFT: res = (lastDirection != MainWindow::RIGHT);
        break;
    default: res = true;
        break;
    }
    return res;
}

void RenderArea::checkGameOver()
{
    for(int i = 1; i < cuerpo.size(); i++){
        if(cuerpo[0].topLeft() == cuerpo[i].topLeft()){
            isGameOver = true;
            emit signalGameOver();
            break;
        }
    }
}

bool RenderArea::fruitEaten()
{
    return (cuerpo[0].topLeft() == fruit.topLeft());
}

void RenderArea::resizeSnake()
{
    QRect resizeBody;
    resizeBody.setRect(lastPosition.x(),lastPosition.y(),10,10);
    cuerpo.append(resizeBody);
}

void RenderArea::timerEvent(QTimerEvent * event)
{
    if(event->timerId() == this->fruitTimerId){
        if(fruitEaten()){
            emit s_fruitEaten();
            emit aumentarPuntaje(++points);
            resizeSnake();
            repaint();
        }
        checkGameOver();
    }

    if(event->timerId() == this->updateTimerId){
        update();
    }
}

void RenderArea::update()
{
    move(lastDirection);
    repaint();
}

void RenderArea::resetRenderArea()
{
    QMessageBox::information(this,"GAMEOVER","PERDISTE PUTITO");
    this->free();
}
