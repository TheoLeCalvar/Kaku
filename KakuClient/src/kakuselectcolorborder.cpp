#include "kakuselectcolorborder.h"

KakuSelectColorBorder::KakuSelectColorBorder(QWidget *parent):QWidget(parent),_buf(30, 20, QImage::Format_RGBX8888)
{
    setFixedSize(30,20);
    _hover = false;
    _color = Qt::black;
    emit signalColor(Qt::black);
}

KakuSelectColorBorder::~KakuSelectColorBorder(){}

void KakuSelectColorBorder::enterEvent(QEvent *)
{
    _hover = true;
    update();
}

void KakuSelectColorBorder::leaveEvent(QEvent *)
{
    _hover = false;
    update();
}

void KakuSelectColorBorder::mouseReleaseEvent(QMouseEvent *)
{
    QColor color = QColorDialog::getColor(_color, this);
    if(color.isValid())
    {
        setcolor(color);
        emit signalColor(color);
        update();
    }
}

void KakuSelectColorBorder::paintEvent(QPaintEvent * e)
{
    draw();

    QPainter p(this);
    p.drawImage(e->rect(), _buf);
}

void KakuSelectColorBorder::draw()
{
    //dessin du rectangle
    _buf.fill(Qt::white);
    QPainter p(&_buf);

    p.setPen(_color);

    for (int i =9 ; i<=10 ;i++)
    {
        p.drawLine(3,i,27,i);
    }
}

void KakuSelectColorBorder::setcolor(QColor couleur)
{
    _color = couleur;
}
