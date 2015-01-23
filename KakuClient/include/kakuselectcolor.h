#ifndef KAKUSELECTCOLOR_H
#define KAKUSELECTCOLOR_H

#include <QImage>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QPainter>
#include <QSizePolicy>
#include <QColor>
#include <QColorDialog>

//cette classe permet d'avoir un rectangle de couleur
//qui change en fonction des couleurs qu'on lui affecte (signalColor)
//nous avons donc crée nous meme ce widget pour correspondre
//a la couleur du text ou du fond des figures

class KakuSelectColor : public QWidget
{
Q_OBJECT

private:
    //image de notre widget
    QImage  _buf;
    //couleur de l'image de fond
    QColor  _color;
    //pour savoir si on est sur notre widget ou pas
    bool    _hover;

public:

    KakuSelectColor(QWidget *parent = 0);
    ~KakuSelectColor();

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

#endif // KAKUSELECTCOLOR_H
