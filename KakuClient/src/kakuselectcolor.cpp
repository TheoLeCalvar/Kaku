#include "kakuselectcolor.h"

KakuSelectColor::KakuSelectColor(QWidget *parent):QWidget(parent),_buf(30, 20, QImage::Format_RGBX8888)
{
    setFixedSize(30,20);
    _hover = false;
    _color = Qt::black;
    emit signalColor(Qt::black);
}

KakuSelectColor::~KakuSelectColor(){}

void KakuSelectColor::enterEvent(QEvent *)
{
    _hover = true;
    update();
}

void KakuSelectColor::leaveEvent(QEvent *)
{
    _hover = false;
    update();
}

void KakuSelectColor::mouseReleaseEvent(QMouseEvent *)
{
    QColor color = QColorDialog::getColor(_color, this);
    if(color.isValid())
    {
        setcolor(color);
        update();
        emit signalColor(color);
    }
}


void KakuSelectColor::paintEvent(QPaintEvent * e)
{
    draw();

    QPainter p(this);
    p.drawImage(e->rect(), _buf);
}

void KakuSelectColor::draw()
{
    //dessin du rectangle
    QColor border(_hover ? Qt::gray : Qt::black);
    _buf.fill(Qt::white);
    QPainter p(&_buf);

    p.setPen(border);
    p.drawRect(2,2,26,16);

    //couleur a l'interieur du rectangle
    p.setPen(_color);

    for (int i =3 ; i<=17 ;i++)
    {
        p.drawLine(3,i,27,i);
    }
}

void KakuSelectColor::setcolor(QColor couleur)
{
    _color = couleur;
}
