#ifndef KAKUSELECTCOLORBORDER_H
#define KAKUSELECTCOLORBORDER_H

#include <QImage>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QPainter>
#include <QSizePolicy>
#include <QColor>
#include <QColorDialog>

//cette classe permet d'avoir une ligne de couleur
//qui change en fonction des couleurs qu'on lui affecte (signalColor)
//nous avont donc créé nous meme ce widget pour corespondre
//a la couleur des lignes ou du contour des figures

class KakuSelectColorBorder : public QWidget
{
Q_OBJECT

private:

    QImage  _buf;
    QColor  _color;
    bool    _hover;

public:

    KakuSelectColorBorder(QWidget *parent = 0);
    ~KakuSelectColorBorder();

    void setcolor(QColor);
    QColor getcolor(){return _color;}

protected:

    virtual void paintEvent(QPaintEvent * e);
    virtual void enterEvent(QEvent *e);
    virtual void leaveEvent(QEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent * e);

private:

    void draw();

signals:

    void signalColor(QColor c);
};

#endif // KAKUSELECTCOLORBORDER_H
