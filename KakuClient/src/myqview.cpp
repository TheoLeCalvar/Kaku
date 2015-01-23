#include "myqview.h"

#include <cmath>
#include <QDebug>
#include <QScrollBar>

MyQView::MyQView():
    _peutDessiner(false),
    _borderColor(Qt::black), _mainColor(Qt::black),_textColor(Qt::black),
    _stylePinceau(Qt::SolidLine),_taillePinceau(1),_tailleTexte(12),
    _actionActive(ACTION_TEXTE),
    _ligneASup(false),_carreASup(false), _RectaSup(false),_cercleASup(false),
    _helypseASup(false), _triangleASup(false),  _polyASup(false),
    _nombrePointPolygone(4),
    _coordoneesSouris(QPoint(0,0)),
    _clicPressed(false),
    _estSelectioning(false)
{
    //pour pas avoir besoin de cliquer pour recuperer les coordoner de la souris
    setMouseTracking(true);
}

MyQView::MyQView(QGraphicsScene *scene, QWidget *parent):
    QGraphicsView(scene, parent),
    _peutDessiner(false),
    _borderColor(Qt::black), _mainColor(Qt::black),_textColor(Qt::black),
    _stylePinceau(Qt::SolidLine),_taillePinceau(1),_tailleTexte(12),
    _actionActive(ACTION_TEXTE),
    _ligneASup(false),_carreASup(false), _RectaSup(false),_cercleASup(false),
    _helypseASup(false), _triangleASup(false),  _polyASup(false),
    _nombrePointPolygone(4),
    _coordoneesSouris(QPoint(0,0)),
    _clicPressed(false),
    _estSelectioning(false)
{
    //pour pas avoir besoin de cliquer pour recuperer les coordoner de la souris
    setMouseTracking(true);
    // setScene(scene);
}

MyQView::~MyQView(){}

/* ****************************************** */
/*              Dessin                        */
/* ****************************************** */
void MyQView::draw()
{
    QPen pen(_borderColor);
    pen.setWidth(_taillePinceau);
    pen.setStyle(_stylePinceau);
    QBrush brush(_mainColor);

    switch(_actionActive)
    {
       // qDebug() << "Avant le add " << scene()->items();
       // qDebug() << "----------------------------------------------------------";

        case ACTION_LIGNE:
        {
            QPointF x1, x2;
            x1 = _points.takeFirst();
            x2 = _points.takeFirst();
            QGraphicsLineItem* line = scene()->addLine(0, 0, x2.x()-x1.x(), x2.y()-x1.y(), pen);
            line->setPos(x1.x(),x1.y());
            line->setFlag(QGraphicsItem::ItemIsMovable, true);
            line->setFlag(QGraphicsItem::ItemIsSelectable,true);
            line->setFlag(QGraphicsItem::ItemSendsGeometryChanges,true);

            QByteArray data;
            QDataStream stream(&data, QIODevice::WriteOnly);
            stream.setVersion(QDataStream::Qt_5_0);

            stream  << x1.x()
                    << x1.y()
                    << x2.x()
                    << x2.y()
                    << line->scale()
                    << pen;


            listItem << line;
            emit addedElement(LIGNE,data);

            //fin previsualisation
            if(_ligneASup) scene()->removeItem(_ligneTmp);//on suprime l'apercut pour pas avoir un double
                _ligneASup=false;

            break;
        }

        case ACTION_TRIANGLE:
        {
            QPolygonF p;
            QPointF p1;

            p1 = _points.takeFirst();
            p << QPointF(0,0);
            p << _points.takeFirst() - p1;
            p << _points.takeFirst() - p1;

            QGraphicsPolygonItem * poly = scene()->addPolygon(p, pen, brush);
            poly->setPos(p1);
            poly->setFlag(QGraphicsItem::ItemIsMovable, true);
            poly->setFlag(QGraphicsItem::ItemIsSelectable,true);
            poly->setFlag(QGraphicsItem::ItemSendsGeometryChanges,true);

            QByteArray data;
            QDataStream stream(&data, QIODevice::WriteOnly);
            stream.setVersion(QDataStream::Qt_5_0);

            stream  << p1.x()
                    << p1.y()
                    << poly->polygon().toPolygon()
                    << poly->scale()
                    << pen
                    << brush;

            listItem << poly;
            emit addedElement(POLYGONE,data);

            //fin previsualisation
            if(_triangleASup)scene()->removeItem(_triangleTmp);//on suprime l'apercut pour pas avoir un double
                _triangleASup=false;

            break;
        }

        case ACTION_CARRE:
        {
            QPointF x1, x2;
            x1 = _points.takeFirst();
            x2 = _points.takeFirst();
            qreal   x = std::min(x1.x(), x2.x()),
                    y = std::min(x1.y(), x2.y()),
                    witdh = fabs(x1.x() - x2.x());
            QGraphicsRectItem * rect = scene()->addRect(0, 0, witdh, witdh, pen, brush);
            rect->setPos(x,y);
            rect->setFlag(QGraphicsItem::ItemIsMovable, true);
            rect->setFlag(QGraphicsItem::ItemIsSelectable,true);
            rect->setFlag(QGraphicsItem::ItemSendsGeometryChanges,true);

            QByteArray data;
            QDataStream stream(&data, QIODevice::WriteOnly);
            stream.setVersion(QDataStream::Qt_5_0);

            stream  << x
                    << y
                    << witdh
                    << witdh
                    << rect->scale()
                    << pen
                    << brush;

            listItem << rect;
            emit addedElement(RECTANGLE,data);

            //fin previsualisation
            if(_carreASup)scene()->removeItem(_rectTmp);//on suprime l'apercut pour pas avoir un double
                _carreASup=false;

            break;
        }

        case ACTION_RECTANGLE:
        {
            QPointF x1, x2;
            x1 = _points.takeFirst();
            x2 = _points.takeFirst();
            qreal   x = std::min(x1.x(), x2.x()),
                    y = std::min(x1.y(), x2.y()),
                    witdh = fabs(x1.x() - x2.x()),
                    height = fabs(x1.y() - x2.y());
            QGraphicsRectItem * rect = scene()->addRect(0, 0, witdh, height, pen, brush);
            rect->setPos(x,y);
            rect->setFlag(QGraphicsItem::ItemIsMovable, true);
            rect->setFlag(QGraphicsItem::ItemIsSelectable,true);
            rect->setFlag(QGraphicsItem::ItemSendsGeometryChanges,true);

            QByteArray data;
            QDataStream stream(&data, QIODevice::WriteOnly);
            stream.setVersion(QDataStream::Qt_5_0);

            stream  << x
                    << y
                    << witdh
                    << height
                    << rect->scale()
                    << pen
                    << brush;

            listItem << rect;

            emit addedElement(RECTANGLE,data);

            //fin previsualisation
            if(_RectaSup)scene()->removeItem(_rectangleTmp);//on suprime l'apercut pour pas avoir un double
                _RectaSup=false;

            break;
        }

        case ACTION_POLYGONE:
        {
            QPolygonF p;
            QPointF p1;

            p1 = _points.takeFirst();
            p << QPointF(0,0);

            for(auto point: _points)
                p << (point - p1);

            QGraphicsPolygonItem * poly = scene()->addPolygon(p, pen, brush);
            poly->setPos(p1);
            poly->setFlag(QGraphicsItem::ItemIsMovable, true);
            poly->setFlag(QGraphicsItem::ItemIsSelectable,true);
            poly->setFlag(QGraphicsItem::ItemSendsGeometryChanges,true);

            QByteArray data;
            QDataStream stream(&data, QIODevice::WriteOnly);
            stream.setVersion(QDataStream::Qt_5_0);

            stream  << p1.x()
                     << p1.y()
                     << poly->polygon().toPolygon()
                     << poly->scale()
                     << pen
                     << brush;

            listItem << poly;
            emit addedElement(POLYGONE,data);

             //fin previsualisation
            if(_polyASup)scene()->removeItem(_polyTmp);//on suprime l'apercut pour pas avoir un double
                _polyASup=false;

             break;
        }

        case ACTION_CERCLE:
        {
            QPointF x1, x2;
            x1 = _points.takeFirst();
            x2 = _points.takeFirst();
            qreal   x = std::min(x1.x(), x2.x()),
                    y = std::min(x1.y(), x2.y()),
                    witdh = fabs(x1.x() - x2.x());

            QGraphicsEllipseItem * cercle = scene()->addEllipse(0, 0, witdh, witdh, pen, brush);
            cercle->setPos(x,y);
            cercle->setFlag(QGraphicsItem::ItemIsMovable, true);
            cercle->setFlag(QGraphicsItem::ItemIsSelectable,true);
            cercle->setFlag(QGraphicsItem::ItemSendsGeometryChanges,true);

            QByteArray data;
            QDataStream stream(&data, QIODevice::WriteOnly);
            stream.setVersion(QDataStream::Qt_5_0);

            stream  << x
                    << y
                    << witdh
                    << witdh
                    << cercle->scale()
                    << pen
                    << brush;

            listItem << cercle;
            emit addedElement(ELLIPSE,data);

            //fin previsualisation
            if(_cercleASup)scene()->removeItem(_cercleTmp);//on suprime l'apercut pour pas avoir un double
                _cercleASup=false;

            break;
        }

        case ACTION_ELLIPSE:
        {
            QPointF x1, x2;
            x1 = _points.takeFirst();
            x2 = _points.takeFirst();
            qreal   x = std::min(x1.x(), x2.x()),
                    y = std::min(x1.y(), x2.y()),
                    witdh = fabs(x1.x() - x2.x()),
                    height = fabs(x1.y() - x2.y());

            QGraphicsEllipseItem * helypse = scene()->addEllipse(0, 0, witdh, height, pen, brush);
            helypse->setPos(x,y);
            helypse->setFlag(QGraphicsItem::ItemIsMovable, true);
            helypse->setFlag(QGraphicsItem::ItemIsSelectable,true);
            helypse->setFlag(QGraphicsItem::ItemSendsGeometryChanges,true);

            QByteArray data;
            QDataStream stream(&data, QIODevice::WriteOnly);
            stream.setVersion(QDataStream::Qt_5_0);

            stream  << x
                    << y
                    << witdh
                    << height
                    << helypse->scale()
                    << pen
                    << brush;

            listItem << helypse;
            emit addedElement(ELLIPSE,data);

            //fin previsualisation
            if(_helypseASup)scene()->removeItem( _helypseTmp);//on suprime l'apercut pour pas avoir un double
                _helypseASup=false;

            break;
        }

        default:
            break;
    }

    _points.clear();
    //qDebug() << "Apres le add " << scene()->items();
    //qDebug() << "----------------------------------------------------------";
}

/* ****************************************** */
/*             apercut creation               */
/* ****************************************** */

void MyQView::apercuCreation()
{
    QPen pen(_borderColor);
    pen.setStyle(_stylePinceau);
    pen.setWidth(_taillePinceau);
    QBrush brush(_mainColor);

    //on delete l'element ajouter temporaire si il y en a
    supressionElementTmp();

    //pour dire que avant on etait en previsualisation

    switch(_actionActive)
    {
        case ACTION_LIGNE:
        {
            QPointF x1, x2;
            x1 = _points.first();
            x2 =  _coordoneesSouris;
            _ligneTmp = scene()->addLine(x1.x(), x1.y(), x2.x(), x2.y(), pen);
            _ligneASup=true;
            break;
        }

        case ACTION_CARRE:
        {
            QPointF x1, x2;
            x1 = _points.first();
            x2 = _coordoneesSouris;
            float   x = std::min(x1.x(), x2.x()),
                    y = std::min(x1.y(), x2.y()),
                    witdh = fabs(x1.x() - x2.x());
            _rectTmp = scene()->addRect(x, y, witdh, witdh, pen, brush);
            _carreASup=true;
            break;
        }

        case ACTION_RECTANGLE:
        {
            QPointF x1, x2;
            x1 = _points.first();
            x2 = _coordoneesSouris;
            float   x = std::min(x1.x(), x2.x()),
                    y = std::min(x1.y(), x2.y()),
                    witdh = fabs(x1.x() - x2.x()),
                    height = fabs(x1.y() - x2.y());
            _rectangleTmp = scene()->addRect(x, y, witdh, height, pen, brush);
            _RectaSup=true;
            break;
        }

        case ACTION_CERCLE:
        {
            QPointF x1, x2;
            x1 = _points.first();
            x2 = _coordoneesSouris;
            float   x = std::min(x1.x(), x2.x()),
                    y = std::min(x1.y(), x2.y()),
                    witdh = fabs(x1.x() - x2.x());

            _cercleTmp = scene()->addEllipse(x, y, witdh, witdh, pen, brush);
            _cercleASup=true;

            break;
        }

        case ACTION_ELLIPSE:
        {
            QPointF x1, x2;
            x1 = _points.first();
            x2 = _coordoneesSouris;
            float   x = std::min(x1.x(), x2.x()),
                    y = std::min(x1.y(), x2.y()),
                    witdh = fabs(x1.x() - x2.x()),
                    height = fabs(x1.y() - x2.y());

            _helypseTmp = scene()->addEllipse(x, y, witdh, height, pen, brush);
            _helypseASup=true;

            break;
        }

        case ACTION_TRIANGLE:
        {
            if(_points.size()==1)
            {
                QPointF x1, x2;
                x1 = _points.first();
                x2 =  _coordoneesSouris;
               _ligneTmp = scene()->addLine(x1.x(), x1.y(), x2.x(), x2.y(), pen);
               _ligneASup=true;
               break;
            }

            if(_points.size()==2)
            {
                QPolygonF p;

                p << _points.first();
                p << _points.at(1);
                p << _coordoneesSouris;

                _triangleTmp= scene()->addPolygon(p, pen, brush);
                _triangleASup=true;

                break;
            }
        }

        case ACTION_POLYGONE:
        {
            if(_points.size()==1)
            {
                QPointF x1, x2;
                x1 = _points.first();
                x2 =  _coordoneesSouris;
                _ligneTmp = scene()->addLine(x1.x(), x1.y(), x2.x(), x2.y(), pen);
                _ligneASup=true;
                break;
            }

            if(_points.size()==2)
            {
                QPolygonF p;

                p << _points.first();
                p << _points.at(1);
                p << _coordoneesSouris;

                _triangleTmp= scene()->addPolygon(p, pen, brush);
                _triangleASup=true;

                break;
            }

            if(_points.size()==3)
            {
                QPolygonF p;

                p << _points.first();
                p << _points.at(1);
                p << _points.at(2);
                p << _coordoneesSouris;

                _polyTmp= scene()->addPolygon(p, pen, brush);
                _polyASup=true;

                break;
            }

            if(_points.size()==4)
            {
                QPolygonF p;

                p << _points.first();
                p << _points.at(1);
                p << _points.at(2);
                p << _points.at(3);
                p << _coordoneesSouris;

                _polyTmp= scene()->addPolygon(p, pen, brush);
                _polyASup=true;

                break;
            }

            if(_points.size()==5)
            {
                QPolygonF p;

                p << _points.first();
                p << _points.at(1);
                p << _points.at(2);
                p << _points.at(3);
                p << _points.at(4);
                p << _coordoneesSouris;

                _polyTmp= scene()->addPolygon(p, pen, brush);
                _polyASup=true;

                break;
            }

            if(_points.size()==6)
            {
                QPolygonF p;

                p << _points.first();
                p << _points.at(1);
                p << _points.at(2);
                p << _points.at(3);
                p << _points.at(4);
                p << _points.at(5);
                p << _coordoneesSouris;

                _polyTmp= scene()->addPolygon(p, pen, brush);
                _polyASup=true;

                break;
            }

            if(_points.size()==7)
            {
                QPolygonF p;

                p << _points.first();
                p << _points.at(1);
                p << _points.at(2);
                p << _points.at(3);
                p << _points.at(4);
                p << _points.at(5);
                p << _points.at(6);
                p << _coordoneesSouris;

                _polyTmp= scene()->addPolygon(p, pen, brush);
                _polyASup=true;

                break;
            }

            if(_points.size()==8)
            {
                QPolygonF p;

                p << _points.first();
                p << _points.at(1);
                p << _points.at(2);
                p << _points.at(3);
                p << _points.at(4);
                p << _points.at(5);
                p << _points.at(6);
                p << _points.at(7);
                p << _coordoneesSouris;

                _polyTmp= scene()->addPolygon(p, pen, brush);
                _polyASup=true;

                break;
            }

            if(_points.size()==9)
            {
                QPolygonF p;

                p << _points.first();
                p << _points.at(1);
                p << _points.at(2);
                p << _points.at(3);
                p << _points.at(4);
                p << _points.at(5);
                p << _points.at(6);
                p << _points.at(7);
                p << _points.at(8);
                p << _coordoneesSouris;

                _polyTmp= scene()->addPolygon(p, pen, brush);
                _polyASup=true;

                break;
            }
        }
        default:
            break;
    }
}


/* ****************************************** */
/*              Event                         */
/* ****************************************** */
void MyQView::mousePressEvent(QMouseEvent *e)
{
    _clicPressed = true;
    
    if(_actionActive & ACTION_FIGURE)
    {
        if(_peutDessiner)
        {
            int hvalue = horizontalScrollBar()->value();
            int vvalue = verticalScrollBar()->value();

            _points << (e->pos() + QPointF(hvalue, vvalue));

            switch(_actionActive)
            {

                case ACTION_CERCLE:
                case ACTION_ELLIPSE:
                case ACTION_LIGNE:
                case ACTION_CARRE:
                case ACTION_RECTANGLE:
                    if(_points.size() >= 2)
                        draw();
                    break;

                case ACTION_TRIANGLE:
                    if(_points.size() >= 3)
                        draw();
                    break;

                case ACTION_POLYGONE:
                    if(_points.size() >= _nombrePointPolygone)
                        draw();
                    break;

                default: 
                    break;
            }
        }
    }
    else if(_actionActive == ACTION_SELECTION)
    {
        if(_peutDessiner)
        {
            int hvalue = horizontalScrollBar()->value();
            int vvalue = verticalScrollBar()->value();

            _points << (e->pos() + QPointF(hvalue, vvalue));

            if((scene()->itemAt(_points.takeFirst(),QTransform())) == NULL)
            {
                //qDebug() << "Début sélection";
                _estSelectioning = true;
                setDragMode(QGraphicsView::RubberBandDrag);
                scene()->clearSelection();
            }
            QGraphicsView::mousePressEvent(e);

           // qDebug() << "clic simple mode select";
        }
    }

    else if(_actionActive == ACTION_TEXTE)
    {
        if(_peutDessiner)
        {
            int hvalue = horizontalScrollBar()->value();
            int vvalue = verticalScrollBar()->value();

            _points << (e->pos() + QPointF(hvalue, vvalue));

            if(! _points.isEmpty())
            {
                _pointLineEdit = _points.takeFirst();
                emit ouvrirLineEdit(_pointLineEdit.x(),_pointLineEdit.y());
            }
        }
    }
    else if(_actionActive == ACTION_DELETE)
    {
        if(_peutDessiner)
        {
            QGraphicsItem * item = scene()->itemAt(e->pos(),QTransform());

            int pos = listItem.indexOf(item);
            emit deletedElement(pos);
            scene()->removeItem(item);
            listItem.removeOne(item);
            delete item;
        }
    }
    else if(_actionActive == ACTION_DRAG)
    {
        int hvalue = horizontalScrollBar()->value();
        int vvalue = verticalScrollBar()->value();

        _points << (e->pos() + QPointF(hvalue, vvalue));

        if(scene()->itemAt(_points.takeFirst(),QTransform()) == NULL)
        {
            setDragMode(QGraphicsView::ScrollHandDrag);
            QGraphicsView::mousePressEvent(e);
            scene()->clearSelection();
        }
    }
    else
    {
        if(_peutDessiner)
            QGraphicsView::mousePressEvent(e);
    }
}

void MyQView::mouseReleaseEvent(QMouseEvent *e)
{
    _estSelectioning = false;
    _clicPressed = false;

    if(_actionActive == ACTION_DRAG)
    {
        setDragMode(QGraphicsView::ScrollHandDrag);
        QGraphicsView::mouseReleaseEvent(e);
        scene()->clearSelection();
    }

    if(_actionActive== ACTION_SELECTION)
    {
        setDragMode(QGraphicsView::RubberBandDrag);
        QGraphicsView::mouseReleaseEvent(e);
    }
}

void MyQView::mouseMoveEvent(QMouseEvent *mouseEvent)
{
    static QTime lastEnvoie;

    if(lastEnvoie.isNull()) lastEnvoie.start();

    qreal hvalue = horizontalScrollBar()->value();
    qreal vvalue = verticalScrollBar()->value();

    _coordoneesSouris =(mouseEvent->pos()+QPointF(hvalue, vvalue));

    switch(_actionActive)
    {
        case ACTION_LIGNE:
        case ACTION_CARRE:
        case ACTION_RECTANGLE:
        case ACTION_CERCLE:
        case ACTION_ELLIPSE:
            if(_points.size() == 1)
                apercuCreation();
            break;

        case ACTION_TRIANGLE:
            if((_points.size() == 2)||(_points.size() == 1))
                apercuCreation();

        case ACTION_DRAG:
             QGraphicsView::mouseMoveEvent(mouseEvent);
             scene()->clearSelection();
         break;

        case ACTION_POLYGONE:
            if((_points.size() >= 1)&&(_points.size() <= 9)) 
                apercuCreation();

        case ACTION_SELECTION:
        {
            QGraphicsView::mouseMoveEvent(mouseEvent);
            if(!_estSelectioning && _clicPressed)
            {
                if(lastEnvoie.elapsed() > 33)
                {
                    lastEnvoie.restart();

                    if(scene()->selectedItems().size() > 0)
                    {
                        for(QGraphicsItem * item : scene()->selectedItems())
                        {
                            if(item->type() == QGraphicsRectItem::Type)
                            {
                                QByteArray data;
                                QDataStream stream(&data, QIODevice::WriteOnly);
                                stream.setVersion(QDataStream::Qt_5_0);

                                stream  << item->x()
                                        << item->y()
                                        << qgraphicsitem_cast<QGraphicsRectItem *>(item)->rect().width()
                                        << qgraphicsitem_cast<QGraphicsRectItem *>(item)->rect().height()
                                        << item->scale()
                                        << qgraphicsitem_cast<QGraphicsRectItem *>(item)->pen()
                                        << qgraphicsitem_cast<QGraphicsRectItem *>(item)->brush();

                                emit modedElement(listItem.indexOf(item),RECTANGLE,data);
                            }

                            else if(item->type() == QGraphicsEllipseItem::Type)
                            {
                                QByteArray data;
                                QDataStream stream(&data, QIODevice::WriteOnly);
                                stream.setVersion(QDataStream::Qt_5_0);

                                stream  << item->x()
                                        << item->y()
                                        << qgraphicsitem_cast<QGraphicsEllipseItem *>(item)->rect().width()
                                        << qgraphicsitem_cast<QGraphicsEllipseItem *>(item)->rect().height()
                                        << item->scale()
                                        << qgraphicsitem_cast<QGraphicsEllipseItem *>(item)->pen()
                                        << qgraphicsitem_cast<QGraphicsEllipseItem *>(item)->brush();

                                emit modedElement(listItem.indexOf(item),ELLIPSE,data);
                            }

                            else if(item->type() == QGraphicsPolygonItem::Type)
                            {
                                QByteArray data;
                                QDataStream stream(&data, QIODevice::WriteOnly);
                                stream.setVersion(QDataStream::Qt_5_0);

                                stream  << item->x()
                                        << item->y()
                                        << qgraphicsitem_cast<QGraphicsPolygonItem *>(item)->polygon().toPolygon()
                                        << item->scale()
                                        << qgraphicsitem_cast<QGraphicsPolygonItem *>(item)->pen()
                                        << qgraphicsitem_cast<QGraphicsPolygonItem *>(item)->brush();

                                emit modedElement(listItem.indexOf(item),POLYGONE,data);

                            }
                            else if(item->type() == QGraphicsLineItem::Type)
                            {
                                QByteArray data;
                                QDataStream stream(&data, QIODevice::WriteOnly);
                                stream.setVersion(QDataStream::Qt_5_0);

                                stream  << item->x()
                                        << item->y()
                                        << qgraphicsitem_cast<QGraphicsLineItem *>(item)->line().x2()+item->x()
                                        << qgraphicsitem_cast<QGraphicsLineItem *>(item)->line().y2()+item->y()
                                        << item->scale()
                                        << qgraphicsitem_cast<QGraphicsLineItem *>(item)->pen();

                                emit modedElement(listItem.indexOf(item),LIGNE,data);
                            }
                            else if(item->type() == QGraphicsTextItem::Type)
                            {
                                QByteArray data;
                                QDataStream stream(&data, QIODevice::WriteOnly);
                                stream.setVersion(QDataStream::Qt_5_0);

                                stream  << item->x()
                                        << item->y()
                                        << item->scale()
                                        << qgraphicsitem_cast<QGraphicsTextItem *>(item)->toPlainText()
                                        << qgraphicsitem_cast<QGraphicsTextItem *>(item)->font()
                                        << qgraphicsitem_cast<QGraphicsTextItem *>(item)->defaultTextColor();
                                /*
                                qDebug() << "Liste avant le moded de texte : " << scene()->items(); 
                                qDebug() << "Indice : " << listItem.indexOf(item);
                                qDebug() << "Pointeur : " << item; 
                                */

                                emit modedElement(listItem.indexOf(item),TEXTE,data);
                            }
                        }
                    }
                }
            }
            break;
        }

        default:
            break;
    }
}

void MyQView::wheelEvent(QWheelEvent *zoom)
{
    if(((_actionActive== ACTION_SELECTION)|| (_actionActive== ACTION_DRAG)) && scene()->selectedItems().size()>0)
    {
        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

        for(int i = 0; i < scene()->selectedItems().size(); i++)
        {
            QGraphicsItem * item = scene()->selectedItems()[i];

            if(zoom->delta() > 0)
            {
                if((item->scale()+0.1)> 0.1 )
                    qgraphicsitem_cast<QGraphicsItem *>(item)->setScale((item->scale()+0.1));
            }
            else
            {
                if((item->scale()-0.1)> 0.1 )
                    qgraphicsitem_cast<QGraphicsItem *>(item)->setScale((item->scale()-0.1));
            }

            if(item->type() == QGraphicsRectItem::Type)
            {
                QByteArray data;
                QDataStream stream(&data, QIODevice::WriteOnly);
                stream.setVersion(QDataStream::Qt_5_0);

                stream  << item->x()
                        << item->y()
                        << qgraphicsitem_cast<QGraphicsRectItem *>(item)->rect().width()
                        << qgraphicsitem_cast<QGraphicsRectItem *>(item)->rect().height()
                        << item->scale()
                        << qgraphicsitem_cast<QGraphicsRectItem *>(item)->pen()
                        << qgraphicsitem_cast<QGraphicsRectItem *>(item)->brush()
                        << qgraphicsitem_cast<QGraphicsRectItem *>(item)->scale();

                emit modedElement(listItem.indexOf(item),RECTANGLE,data);
            }
            else if(item->type() == QGraphicsEllipseItem::Type)
            {
                QByteArray data;
                QDataStream stream(&data, QIODevice::WriteOnly);
                stream.setVersion(QDataStream::Qt_5_0);

                stream  << item->x()
                        << item->y()
                        << qgraphicsitem_cast<QGraphicsEllipseItem *>(item)->rect().width()
                        << qgraphicsitem_cast<QGraphicsEllipseItem *>(item)->rect().height()
                        << item->scale()
                        << qgraphicsitem_cast<QGraphicsEllipseItem *>(item)->pen()
                        << qgraphicsitem_cast<QGraphicsEllipseItem *>(item)->brush();

                emit modedElement(listItem.indexOf(item),ELLIPSE,data);
            }
            else if(item->type() == QGraphicsPolygonItem::Type)
            {
                QByteArray data;
                QDataStream stream(&data, QIODevice::WriteOnly);
                stream.setVersion(QDataStream::Qt_5_0);

                stream  << item->x()
                        << item->y()
                        << qgraphicsitem_cast<QGraphicsPolygonItem *>(item)->polygon().toPolygon()
                        << item->scale()
                        << qgraphicsitem_cast<QGraphicsPolygonItem *>(item)->pen()
                        << qgraphicsitem_cast<QGraphicsPolygonItem *>(item)->brush();

                emit modedElement(listItem.indexOf(item),POLYGONE,data);

            }
            else if(item->type() == QGraphicsLineItem::Type)
            {
                QByteArray data;
                QDataStream stream(&data, QIODevice::WriteOnly);
                stream.setVersion(QDataStream::Qt_5_0);

                stream  << item->x()
                        << item->y()
                        << qgraphicsitem_cast<QGraphicsLineItem *>(item)->line().x2()+item->x()
                        << qgraphicsitem_cast<QGraphicsLineItem *>(item)->line().y2()+item->y()
                        << item->scale()
                        << qgraphicsitem_cast<QGraphicsLineItem *>(item)->pen();

                emit modedElement(listItem.indexOf(item),LIGNE,data);
            }
            else if(item->type() == QGraphicsTextItem::Type)
            {
                QByteArray data;
                QDataStream stream(&data, QIODevice::WriteOnly);
                stream.setVersion(QDataStream::Qt_5_0);

                stream  << item->x()
                        << item->y()
                        << item->scale()
                        << qgraphicsitem_cast<QGraphicsTextItem *>(item)->toPlainText()
                        << qgraphicsitem_cast<QGraphicsTextItem *>(item)->font()
                        << qgraphicsitem_cast<QGraphicsTextItem *>(item)->defaultTextColor();


                emit modedElement(listItem.indexOf(item),TEXTE,data);
            }
        }
    }
}



void MyQView::supressionElementTmp()
{
    //on delete l'element ajouter temporaire si il y en a
    if(_ligneASup)
    {
        scene()->removeItem(_ligneTmp);
        _ligneASup=false;
    }

    if(_carreASup)
    {
        scene()->removeItem(_rectTmp);
        _carreASup=false;
    }

    if(_RectaSup)
    {
        scene()->removeItem(_rectangleTmp);
        _RectaSup=false;
    }

    if(_helypseASup)
    {
        scene()->removeItem(_helypseTmp);
        _helypseASup=false;
    }

    if(_cercleASup)
    {
        scene()->removeItem(_cercleTmp);
        _cercleASup=false;
    }

    if(_triangleASup)
    {
        scene()->removeItem(_triangleTmp);
        _triangleASup=false;
    }

    if(_polyASup)
    {
        scene()->removeItem(_polyTmp);
        _polyASup=false;
    }
}

void MyQView::clear()
{
    scene()->clear();
    listItem.clear();

    //qDebug() << "on a nettoyer la scene et la listItem";
}

void MyQView::setListItem(QList<QGraphicsItem *> list)
{
    listItem = list;

    //qDebug() << "Liste après chargement : " << listItem;
}

/* ****************************************** */
/*              Slots                         */
/* ****************************************** */

void MyQView::setBorderColor(QColor c)
{
    _borderColor = c;

    QPen pen(c);

    for(int i = 0; i < scene()->selectedItems().size(); i++)
    {
        QGraphicsItem * item = scene()->selectedItems()[i];

        if(item->type() == QGraphicsRectItem::Type)
        {
            pen.setWidth(qgraphicsitem_cast<QGraphicsRectItem *>(item)->pen().width());
            pen.setStyle(qgraphicsitem_cast<QGraphicsRectItem *>(item)->pen().style());
            qgraphicsitem_cast<QGraphicsRectItem *>(item)->setPen(pen);

            QByteArray data;
            QDataStream stream(&data, QIODevice::WriteOnly);
            stream.setVersion(QDataStream::Qt_5_0);

            stream  << item->x()
                    << item->y()
                    << qgraphicsitem_cast<QGraphicsRectItem *>(item)->rect().width()
                    << qgraphicsitem_cast<QGraphicsRectItem *>(item)->rect().height()
                    << item->scale()
                    << qgraphicsitem_cast<QGraphicsRectItem *>(item)->pen()
                    << qgraphicsitem_cast<QGraphicsRectItem *>(item)->brush()
                    << qgraphicsitem_cast<QGraphicsRectItem *>(item)->scale();

            emit modedElement(listItem.indexOf(item),RECTANGLE,data);
        }

        else if(item->type() == QGraphicsEllipseItem::Type)
        {
            pen.setWidth(qgraphicsitem_cast<QGraphicsEllipseItem *>(item)->pen().width());
            pen.setStyle(qgraphicsitem_cast<QGraphicsEllipseItem *>(item)->pen().style());
            qgraphicsitem_cast<QGraphicsEllipseItem *>(item)->setPen(pen);

            QByteArray data;
            QDataStream stream(&data, QIODevice::WriteOnly);
            stream.setVersion(QDataStream::Qt_5_0);

            stream  << item->x()
                    << item->y()
                    << qgraphicsitem_cast<QGraphicsEllipseItem *>(item)->rect().width()
                    << qgraphicsitem_cast<QGraphicsEllipseItem *>(item)->rect().height()
                    << item->scale()
                    << qgraphicsitem_cast<QGraphicsEllipseItem *>(item)->pen()
                    << qgraphicsitem_cast<QGraphicsEllipseItem *>(item)->brush();

            emit modedElement(listItem.indexOf(item),ELLIPSE,data);
        }

        else if(item->type() == QGraphicsPolygonItem::Type)
        {
            pen.setWidth(qgraphicsitem_cast<QGraphicsPolygonItem *>(item)->pen().width());
            pen.setStyle(qgraphicsitem_cast<QGraphicsPolygonItem *>(item)->pen().style());
            qgraphicsitem_cast<QGraphicsPolygonItem *>(item)->setPen(pen);

            QByteArray data;
            QDataStream stream(&data, QIODevice::WriteOnly);
            stream.setVersion(QDataStream::Qt_5_0);

            stream  << item->x()
                    << item->y()
                    << qgraphicsitem_cast<QGraphicsPolygonItem *>(item)->polygon().toPolygon()
                    << item->scale()
                    << qgraphicsitem_cast<QGraphicsPolygonItem *>(item)->pen()
                    << qgraphicsitem_cast<QGraphicsPolygonItem *>(item)->brush();

            emit modedElement(listItem.indexOf(item),POLYGONE,data);

        }
        else if(item->type() == QGraphicsLineItem::Type)
        {
            pen.setWidth(qgraphicsitem_cast<QGraphicsLineItem *>(item)->pen().width());
            pen.setStyle(qgraphicsitem_cast<QGraphicsLineItem *>(item)->pen().style());
            qgraphicsitem_cast<QGraphicsLineItem *>(item)->setPen(pen);

            QByteArray data;
            QDataStream stream(&data, QIODevice::WriteOnly);
            stream.setVersion(QDataStream::Qt_5_0);

            stream  << item->x()
                    << item->y()
                    << qgraphicsitem_cast<QGraphicsLineItem *>(item)->line().x2()+item->x()
                    << qgraphicsitem_cast<QGraphicsLineItem *>(item)->line().y2()+item->y()
                    << item->scale()
                    << qgraphicsitem_cast<QGraphicsLineItem *>(item)->pen();

            emit modedElement(listItem.indexOf(item),LIGNE,data);
        }
    }
}

void MyQView::setMainColor(QColor c)
{
    _mainColor = c;

    QBrush brush(c);

    for(int i = 0; i < scene()->selectedItems().size(); i++)
    {
        QGraphicsItem * item = scene()->selectedItems()[i];

        if(item->type() == QGraphicsRectItem::Type)
        {
            qgraphicsitem_cast<QGraphicsRectItem *>(item)->setBrush(brush);
            QByteArray data;
            QDataStream stream(&data, QIODevice::WriteOnly);
            stream.setVersion(QDataStream::Qt_5_0);

            stream  << item->x()
                    << item->y()
                    << qgraphicsitem_cast<QGraphicsRectItem *>(item)->rect().width()
                    << qgraphicsitem_cast<QGraphicsRectItem *>(item)->rect().height()
                    << item->scale()
                    << qgraphicsitem_cast<QGraphicsRectItem *>(item)->pen()
                    << qgraphicsitem_cast<QGraphicsRectItem *>(item)->brush()
                    << qgraphicsitem_cast<QGraphicsRectItem *>(item)->scale();

            emit modedElement(listItem.indexOf(item),RECTANGLE,data);

        }
        else if(item->type() == QGraphicsEllipseItem::Type)
        {
            qgraphicsitem_cast<QGraphicsEllipseItem *>(item)->setBrush(brush);
            QByteArray data;
            QDataStream stream(&data, QIODevice::WriteOnly);
            stream.setVersion(QDataStream::Qt_5_0);

            stream  << item->x()
                    << item->y()
                    << qgraphicsitem_cast<QGraphicsEllipseItem *>(item)->rect().width()
                    << qgraphicsitem_cast<QGraphicsEllipseItem *>(item)->rect().height()
                    << item->scale()
                    << qgraphicsitem_cast<QGraphicsEllipseItem *>(item)->pen()
                    << qgraphicsitem_cast<QGraphicsEllipseItem *>(item)->brush();

            emit modedElement(listItem.indexOf(item),ELLIPSE,data);

        }
        else if(item->type() == QGraphicsPolygonItem::Type)
        {
            qgraphicsitem_cast<QGraphicsPolygonItem *>(item)->setBrush(brush);
            QByteArray data;
            QDataStream stream(&data, QIODevice::WriteOnly);
            stream.setVersion(QDataStream::Qt_5_0);

            stream  << item->x()
                    << item->y()
                    << qgraphicsitem_cast<QGraphicsPolygonItem *>(item)->polygon().toPolygon()
                    << item->scale()
                    << qgraphicsitem_cast<QGraphicsPolygonItem *>(item)->pen()
                    << qgraphicsitem_cast<QGraphicsPolygonItem *>(item)->brush();

            emit modedElement(listItem.indexOf(item),POLYGONE,data);
        }
    }
}

void MyQView::setTextColor(QColor c)
{
    _textColor = c;

    QBrush brush(c);

    for(int i = 0; i < scene()->selectedItems().size(); i++)
    {
        QGraphicsItem * item = scene()->selectedItems()[i];

        if(item->type() == QGraphicsTextItem::Type)
        {
            qgraphicsitem_cast<QGraphicsTextItem *>(item)->setDefaultTextColor(c);


            QByteArray data;
            QDataStream stream(&data, QIODevice::WriteOnly);
            stream.setVersion(QDataStream::Qt_5_0);

            stream  << item->x()
                    << item->y()
                    << item->scale()
                    << qgraphicsitem_cast<QGraphicsTextItem *>(item)->toPlainText()
                    << qgraphicsitem_cast<QGraphicsTextItem *>(item)->font()
                    << qgraphicsitem_cast<QGraphicsTextItem *>(item)->defaultTextColor();


            emit modedElement(listItem.indexOf(item),TEXTE,data);
       }
    }
}


void MyQView::setWidthPen(int n)
{
    _taillePinceau = n;

    QPen pen;

    for(int i = 0; i < scene()->selectedItems().size(); i++)
    {
        QGraphicsItem * item = scene()->selectedItems()[i];

        if(item->type() == QGraphicsRectItem::Type)
        {
            pen.setColor(qgraphicsitem_cast<QGraphicsRectItem *>(item)->pen().color());
            pen.setWidth(n);
            pen.setStyle(qgraphicsitem_cast<QGraphicsRectItem *>(item)->pen().style());
            qgraphicsitem_cast<QGraphicsRectItem *>(item)->setPen(pen);

            QByteArray data;
            QDataStream stream(&data, QIODevice::WriteOnly);
            stream.setVersion(QDataStream::Qt_5_0);

            stream  << item->x()
                    << item->y()
                    << qgraphicsitem_cast<QGraphicsRectItem *>(item)->rect().width()
                    << qgraphicsitem_cast<QGraphicsRectItem *>(item)->rect().height()
                    << item->scale()
                    << qgraphicsitem_cast<QGraphicsRectItem *>(item)->pen()
                    << qgraphicsitem_cast<QGraphicsRectItem *>(item)->brush();

            emit modedElement(listItem.indexOf(item),RECTANGLE,data);
        }

        else if(item->type() == QGraphicsEllipseItem::Type)
        {
            pen.setColor(qgraphicsitem_cast<QGraphicsEllipseItem *>(item)->pen().color());
            pen.setWidth(n);
            pen.setStyle(qgraphicsitem_cast<QGraphicsEllipseItem *>(item)->pen().style());
            qgraphicsitem_cast<QGraphicsEllipseItem *>(item)->setPen(pen);

            QByteArray data;
            QDataStream stream(&data, QIODevice::WriteOnly);
            stream.setVersion(QDataStream::Qt_5_0);

            stream  << item->x()
                    << item->y()
                    << qgraphicsitem_cast<QGraphicsEllipseItem *>(item)->rect().width()
                    << qgraphicsitem_cast<QGraphicsEllipseItem *>(item)->rect().height()
                    << item->scale()
                    << qgraphicsitem_cast<QGraphicsEllipseItem *>(item)->pen()
                    << qgraphicsitem_cast<QGraphicsEllipseItem *>(item)->brush();

            emit modedElement(listItem.indexOf(item),ELLIPSE,data);
        }

        else if(item->type() == QGraphicsPolygonItem::Type)
        {
            pen.setColor(qgraphicsitem_cast<QGraphicsPolygonItem *>(item)->pen().color());
            pen.setWidth(n);
            pen.setStyle(qgraphicsitem_cast<QGraphicsPolygonItem *>(item)->pen().style());
            qgraphicsitem_cast<QGraphicsPolygonItem *>(item)->setPen(pen);

            QByteArray data;
            QDataStream stream(&data, QIODevice::WriteOnly);
            stream.setVersion(QDataStream::Qt_5_0);

            stream  << item->x()
                    << item->y()
                    << qgraphicsitem_cast<QGraphicsPolygonItem *>(item)->polygon().toPolygon()
                    << item->scale()
                    << qgraphicsitem_cast<QGraphicsPolygonItem *>(item)->pen()
                    << qgraphicsitem_cast<QGraphicsPolygonItem *>(item)->brush();

            emit modedElement(listItem.indexOf(item),POLYGONE,data);
        }
        else if(item->type() == QGraphicsLineItem::Type)
        {
            pen.setColor(qgraphicsitem_cast<QGraphicsLineItem *>(item)->pen().color());
            pen.setWidth(n);
            pen.setStyle(qgraphicsitem_cast<QGraphicsLineItem *>(item)->pen().style());
            qgraphicsitem_cast<QGraphicsLineItem *>(item)->setPen(pen);

            QByteArray data;
            QDataStream stream(&data, QIODevice::WriteOnly);
            stream.setVersion(QDataStream::Qt_5_0);

            stream  << item->x()
                    << item->y()
                    << qgraphicsitem_cast<QGraphicsLineItem *>(item)->line().x2()+item->x()
                    << qgraphicsitem_cast<QGraphicsLineItem *>(item)->line().y2()+item->y()
                    << item->scale()
                    << qgraphicsitem_cast<QGraphicsLineItem *>(item)->pen();

            emit modedElement(listItem.indexOf(item),LIGNE,data);
        }
    }
}

void MyQView::setWidthText(int n)
{
    _tailleTexte = n;

    QFont myfont("Ubuntu");
    myfont.setPointSize(n);

    for(int i = 0; i < scene()->selectedItems().size(); i++)
    {
        QGraphicsItem * item = scene()->selectedItems()[i];

        if(item->type() == QGraphicsTextItem::Type)
        {
            qgraphicsitem_cast<QGraphicsTextItem *>(item)->setFont(myfont);

            QByteArray data;
            QDataStream stream(&data, QIODevice::WriteOnly);
            stream.setVersion(QDataStream::Qt_5_0);

            stream  << item->x()
                    << item->y()
                    << item->scale()
                    << qgraphicsitem_cast<QGraphicsTextItem *>(item)->toPlainText()
                    << qgraphicsitem_cast<QGraphicsTextItem *>(item)->font()
                    << qgraphicsitem_cast<QGraphicsTextItem *>(item)->defaultTextColor();


            emit modedElement(listItem.indexOf(item),TEXTE,data);
        }
    }
}

void MyQView::setStylePen(int i)
{
    if(i == 0)
        _stylePinceau = Qt::SolidLine;
    else if(i == 1)
        _stylePinceau = Qt::DashLine;
    else if (i == 2)
        _stylePinceau = Qt::DotLine;

    QPen pen;

    for(int i = 0; i < scene()->selectedItems().size(); i++)
    {
        QGraphicsItem * item = scene()->selectedItems()[i];

        if(item->type() == QGraphicsRectItem::Type)
        {
            pen.setColor(qgraphicsitem_cast<QGraphicsRectItem *>(item)->pen().color());
            pen.setWidth(qgraphicsitem_cast<QGraphicsRectItem *>(item)->pen().width());
            pen.setStyle(_stylePinceau);
            qgraphicsitem_cast<QGraphicsRectItem *>(item)->setPen(pen);

            QByteArray data;
            QDataStream stream(&data, QIODevice::WriteOnly);
            stream.setVersion(QDataStream::Qt_5_0);

            stream  << item->x()
                    << item->y()
                    << qgraphicsitem_cast<QGraphicsRectItem *>(item)->rect().width()
                    << qgraphicsitem_cast<QGraphicsRectItem *>(item)->rect().height()
                    << item->scale()
                    << qgraphicsitem_cast<QGraphicsRectItem *>(item)->pen()
                    << qgraphicsitem_cast<QGraphicsRectItem *>(item)->brush()
                    << qgraphicsitem_cast<QGraphicsRectItem *>(item)->scale();

            emit modedElement(listItem.indexOf(item),RECTANGLE,data);
        }

        else if(item->type() == QGraphicsEllipseItem::Type)
        {
            pen.setColor(qgraphicsitem_cast<QGraphicsEllipseItem *>(item)->pen().color());
            pen.setWidth(qgraphicsitem_cast<QGraphicsEllipseItem *>(item)->pen().width());
            pen.setStyle(_stylePinceau);
            qgraphicsitem_cast<QGraphicsEllipseItem *>(item)->setPen(pen);

            QByteArray data;
            QDataStream stream(&data, QIODevice::WriteOnly);
            stream.setVersion(QDataStream::Qt_5_0);

            stream  << item->x()
                    << item->y()
                    << qgraphicsitem_cast<QGraphicsEllipseItem *>(item)->rect().width()
                    << qgraphicsitem_cast<QGraphicsEllipseItem *>(item)->rect().height()
                    << item->scale()
                    << qgraphicsitem_cast<QGraphicsEllipseItem *>(item)->pen()
                    << qgraphicsitem_cast<QGraphicsEllipseItem *>(item)->brush();

            emit modedElement(listItem.indexOf(item),ELLIPSE,data);

        }

        else if(item->type() == QGraphicsPolygonItem::Type)
        {
            pen.setColor(qgraphicsitem_cast<QGraphicsPolygonItem *>(item)->pen().color());
            pen.setWidth(qgraphicsitem_cast<QGraphicsPolygonItem *>(item)->pen().width());
            pen.setStyle(_stylePinceau);
            qgraphicsitem_cast<QGraphicsPolygonItem *>(item)->setPen(pen);

            QByteArray data;
            QDataStream stream(&data, QIODevice::WriteOnly);
            stream.setVersion(QDataStream::Qt_5_0);

            stream  << item->x()
                    << item->y()
                    << qgraphicsitem_cast<QGraphicsPolygonItem *>(item)->polygon().toPolygon()
                    << item->scale()
                    << qgraphicsitem_cast<QGraphicsPolygonItem *>(item)->pen()
                    << qgraphicsitem_cast<QGraphicsPolygonItem *>(item)->brush();

            emit modedElement(listItem.indexOf(item),POLYGONE,data);

        }
        else if(item->type() == QGraphicsLineItem::Type)
        {
            pen.setColor(qgraphicsitem_cast<QGraphicsLineItem *>(item)->pen().color());
            pen.setWidth(qgraphicsitem_cast<QGraphicsLineItem *>(item)->pen().width());
            pen.setStyle(_stylePinceau);
            qgraphicsitem_cast<QGraphicsLineItem *>(item)->setPen(pen);

            QByteArray data;
            QDataStream stream(&data, QIODevice::WriteOnly);
            stream.setVersion(QDataStream::Qt_5_0);

            stream  << item->x()
                    << item->y()
                    << qgraphicsitem_cast<QGraphicsLineItem *>(item)->line().x2()+item->x()
                    << qgraphicsitem_cast<QGraphicsLineItem *>(item)->line().y2()+item->y()
                    << item->scale()
                    << qgraphicsitem_cast<QGraphicsLineItem *>(item)->pen();

            emit modedElement(listItem.indexOf(item),LIGNE,data);

        }
    }
}

void MyQView::setTextActivated()
{
    _actionActive = ACTION_TEXTE;

    setDragMode(QGraphicsView::NoDrag);
    //on delete l'element ajouter temporaire si il y en a
    supressionElementTmp();
}

void MyQView::setSelectionActivated()
{
    _actionActive = ACTION_SELECTION;

    setDragMode(QGraphicsView::NoDrag);
    //on delete l'element ajouter temporaire si il y en a
    supressionElementTmp();
}

void MyQView::setDragActivated()
{
    _actionActive = ACTION_DRAG;

    //on delete l'element ajouter temporaire si il y en a
    supressionElementTmp();
}

void MyQView::setDeleteActivated()
{
    _actionActive = ACTION_DELETE;

    setDragMode(QGraphicsView::NoDrag);

    //on delete l'element ajouter temporaire si il y en a
    supressionElementTmp();
}

void MyQView::setLigne()
{
    _points.clear();//clear la liste de point du mode utiliser avant

    _actionActive = ACTION_LIGNE;

    setDragMode(QGraphicsView::NoDrag);

    //on delete l'element ajouter temporaire si il y en a
    supressionElementTmp();
}

void MyQView::setTriangle()
{
    _points.clear();//clear la liste de point du mode utiliser avant

    _actionActive = ACTION_TRIANGLE;

    setDragMode(QGraphicsView::NoDrag);

    //on delete l'element ajouter temporaire si il y en a
    supressionElementTmp();
}

void MyQView::setRectangle()
{
    _points.clear();//clear la liste de point du mode utiliser avant

    _actionActive = ACTION_RECTANGLE;

    setDragMode(QGraphicsView::NoDrag);

    //on delete l'element ajouter temporaire si il y en a
    supressionElementTmp();
}

void MyQView::setCarre()
{
    _points.clear();//clear la liste de point du mode utiliser avant

    _actionActive = ACTION_CARRE;

    setDragMode(QGraphicsView::NoDrag);

    //on delete l'element ajouter temporaire si il y en a
    supressionElementTmp();
}

void MyQView::setPolygone()
{
    _points.clear();//clear la liste de point du mode utiliser avant

    _actionActive = ACTION_POLYGONE;

    setDragMode(QGraphicsView::NoDrag);

    //on delete l'element ajouter temporaire si il y en a
    supressionElementTmp();
}

void MyQView::setEllipse()
{
    _points.clear();//clear la liste de point du mode utiliser avant

    _actionActive = ACTION_ELLIPSE;

    setDragMode(QGraphicsView::NoDrag);

    //on delete l'element ajouter temporaire si il y en a
    supressionElementTmp();
}

void MyQView::setCercle()
{
    _points.clear();//clear la liste de point du mode utiliser avant

    _actionActive = ACTION_CERCLE;

    setDragMode(QGraphicsView::NoDrag);

    //on delete l'element ajouter temporaire si il y en a
    supressionElementTmp();
}

void MyQView::slotAfficherText(QString m)
{
    if(! m.isEmpty())
    {
        QFont myfont("Courrier");
        myfont.setPointSize(_tailleTexte);
        QGraphicsTextItem* textItem = new QGraphicsTextItem(m);

        textItem->setFlag(QGraphicsItem::ItemIsMovable, true);
        textItem->setFlag(QGraphicsItem::ItemIsSelectable,true);
        textItem->setFlag(QGraphicsItem::ItemIsFocusable,true);

        textItem->setFlag(QGraphicsItem::ItemSendsGeometryChanges,true);

        textItem->setFont(myfont);
        textItem->setX(_pointLineEdit.x() - textItem->boundingRect().width()/2);
        textItem->setY(_pointLineEdit.y() - textItem->boundingRect().height()/2);
        textItem->setDefaultTextColor(_textColor);

        QByteArray data;
        QDataStream stream(&data, QIODevice::WriteOnly);
        stream.setVersion(QDataStream::Qt_5_0);

        stream  << textItem->x()
                << textItem->y()
                << textItem->scale()
                << textItem->toPlainText()
                << textItem->font()
                << textItem->defaultTextColor();

        scene()->addItem(textItem);

        //qDebug() << "Ma liste après ajout d'un texte : " << scene()->items();

        listItem << textItem;
        emit addedElement(TEXTE, data);
    }
}

void MyQView::slotNbPoint(int n)
{
   _nombrePointPolygone = n;
}

void MyQView::slotSuppression()
{
    for(int i = 0; i < scene()->selectedItems().size(); i++)
    {
        QGraphicsItem * item = scene()->selectedItems()[i];
        int pos = listItem.indexOf(item);

        scene()->removeItem(item);

        emit deletedElement(pos);
        listItem.removeOne(item);
        delete item;

        i--;
    }
}

void MyQView::slotPeutDessiner(bool dessin)
{
    _peutDessiner = dessin;

    if(!_peutDessiner)
        scene()->clearSelection();
}

void MyQView::slotCleared()
{
    scene()->clear();
    listItem.clear();
}

void MyQView::slotAddedElement(quint16 type, const QByteArray &args)
{
    //qDebug() << "Avant l'added " << listItem;
    //qDebug() << "----------------------------------------------------------";


    QDataStream in(args);
    in.setVersion(QDataStream::Qt_5_0);

    switch(type)
    {
        case LIGNE:
        {
            qreal x1, y1, x2, y2, scale;
            QPen pen;

            in >> x1 >> y1 >> x2 >> y2 >> scale >> pen;

            QGraphicsLineItem * item = scene()->addLine(0, 0, x2-x1, y2-y1);
            listItem << item;

            item->setPos(x1,y1);
            item->setScale(scale);
            item->setPen(pen);

            item->setFlag(QGraphicsItem::ItemIsMovable, true);
            item->setFlag(QGraphicsItem::ItemIsSelectable,true);
            item->setFlag(QGraphicsItem::ItemSendsGeometryChanges,true);

            break;
        }

        case TEXTE:
        {
            qreal x, y, scale;
            QFont font;
            QString text;
            QColor color;

            in >> x >> y >> scale >> text >> font >> color;
            font.setFamily("Courrier");

            QGraphicsTextItem * item = scene()->addText(text);
            listItem << item;
            item->setX(x);
            item->setY(y);
            item->setScale(scale);
            item->setDefaultTextColor(color);
            item->setFont(font);

            item->setFlag(QGraphicsItem::ItemIsMovable, true);
            item->setFlag(QGraphicsItem::ItemIsSelectable,true);
            item->setFlag(QGraphicsItem::ItemIsFocusable,true);

            item->setFlag(QGraphicsItem::ItemSendsGeometryChanges,true);
            break;
        }

        case CARRE:
        case RECTANGLE:
        {
            //qDebug() << " Et c'est un rectangle";
            qreal x, y, w, h, scale;
            QPen pen;
            QBrush brush;

            in >> x >> y >> w >> h >> scale >> pen >> brush;
            //qDebug() << "x = " << x << " y = " << y << " width = " << w << "height = " << h << " Pen = " << pen << " Brush = " << brush;

            QGraphicsRectItem * item = scene()->addRect(0, 0, w, h);
            listItem << item;
            item->setPos(x,y);
            item->setPen(pen);
            item->setBrush(brush);
            item->setScale(scale);


            item->setFlag(QGraphicsItem::ItemIsMovable, true);
            item->setFlag(QGraphicsItem::ItemIsSelectable,true);
            item->setFlag(QGraphicsItem::ItemSendsGeometryChanges,true);

            break;
        }

        case TRIANGLE:
        case POLYGONE:
        {
            qreal x, y, scale;
            QPolygon poly;
            QPen pen;
            QBrush brush;

            in >> x >> y  >> poly >> scale >> pen >> brush;

            QGraphicsPolygonItem * item = scene()->addPolygon(poly);
            listItem << item;
            item->setPos(x,y);
            item->setScale(scale);
            item->setPen(pen);
            item->setBrush(brush);

            //qDebug() << "Added : " << "x = " << x << " y = " << y << " Polygon = " << poly;


            item->setFlag(QGraphicsItem::ItemIsMovable, true);
            item->setFlag(QGraphicsItem::ItemIsSelectable,true);
            item->setFlag(QGraphicsItem::ItemSendsGeometryChanges,true);

            break;
        }


        case CERCLE:
        case ELLIPSE:
        {
            qreal x, y, w, h, scale;
            QPen pen;
            QBrush brush;

            in >> x >> y >> w >> h >> scale >> pen >> brush;
            //qDebug() << "x = " << x << " y = " << y << " width = " << w << "height = " << h << " Pen = " << pen << " Brush = " << brush;

            QGraphicsEllipseItem * item = scene()->addEllipse(0, 0, w, h);
            listItem << item;
            item->setPos(x,y);
            item->setPen(pen);
            item->setBrush(brush);
            item->setScale(scale);

            item->setFlag(QGraphicsItem::ItemIsMovable, true);
            item->setFlag(QGraphicsItem::ItemIsSelectable,true);
            item->setFlag(QGraphicsItem::ItemSendsGeometryChanges,true);

            break;
        }

        default:
        {
            qWarning() << "Forme inconnue (" << type << ")";
            break;
        }
    }
    //qDebug() << "item added " << listItem[listItem.size()-1];
    //qDebug() << "----------------------------------------------------------";
}


void MyQView::slotDeletedElement(quint16 id)
{
    QGraphicsItem * item = listItem.at(id);

    scene()->removeItem(item);
    listItem.removeAt(id);
    delete item;
}

void MyQView::slotModedElement(quint16 id, quint16 type, const QByteArray & args)
{
    //qDebug() << "Avant le modded " << scene()->items();
    //qDebug() << "----------------------------------------------------------";

    //qDebug() << "Taille de la listItem : " << listItem.size();
    QGraphicsItem * item = listItem.at(id);
    //qDebug() << "type de l'item : " << item->type() << " et le type envoyé : " << type;


    if(!item)
    {
        qWarning() << "Item existe pas";
    }
    else
    {
        QDataStream in(args);
        in.setVersion(QDataStream::Qt_5_0);

        switch(type)
        {
            case LIGNE:
            {
                QGraphicsLineItem * lineItem = (QGraphicsLineItem*)item;

                qreal x1, y1, x2, y2, scale;
                QPen pen;

                in >> x1 >> y1 >> x2 >> y2 >> scale >> pen;

                lineItem->setLine(0, 0, x2 - x1, y2 - y1);
                lineItem->setPos(x1, y1);
                lineItem->setPen(pen);
                lineItem->setScale(scale);

                break;
            }
            case TEXTE:
            {
                QGraphicsTextItem * textItem = (QGraphicsTextItem*)item;

                qreal x, y, scale;
                QString text;
                QFont font;
                QColor color;

                in >> x >> y >> scale >> text >> font >> color;

                textItem->setX(x);
                textItem->setY(y);
                textItem->setScale(scale);
                textItem->setDefaultTextColor(color);
                textItem->setFont(font);
                textItem->setPlainText(text);
                break;
            }
            case CARRE:
            case RECTANGLE:
            {
                QGraphicsRectItem * rectItem = (QGraphicsRectItem*)item;

                qreal x, y, w, h, scale;
                QPen pen;
                QBrush brush;

                in >> x >> y >> w >> h >> scale >> pen >> brush;

                rectItem->setRect(0, 0, w, h);
                rectItem->setPos(x, y);
                rectItem->setPen(pen);
                rectItem->setBrush(brush);
                rectItem->setScale(scale);
                break;


                //qDebug()  << "Modded Rect " << "x = " << x << " y = " << y << " w  = " << w ;

            }
            case TRIANGLE:
            case POLYGONE:
            {
               QGraphicsPolygonItem * polygonItem = (QGraphicsPolygonItem*)item;

               qreal x, y, scale;
               QPolygon points;
               QPen pen;
               QBrush brush;

               in >> x >> y >> points >> scale >> pen >> brush;

               //qDebug()  << "Modded Poly " << "x = " << x << " y = " << y << " Polygon = " << points;


                polygonItem->setPolygon(points);
                polygonItem->setPos(x, y);
                polygonItem->setPen(pen);
                polygonItem->setBrush(brush);
                polygonItem->setScale(scale);
                break;
            }
            case CERCLE:
            case ELLIPSE:
            {
             QGraphicsEllipseItem * ellipseItem = (QGraphicsEllipseItem*)item;

                qreal x, y, w, h, scale;
                QPen pen;
                QBrush brush;

                in >> x >> y >> w >> h >> scale >> pen >> brush;

                ellipseItem->setRect(0, 0, w, h);
                ellipseItem->setPos(x, y);
                ellipseItem->setPen(pen);
                ellipseItem->setBrush(brush);
                ellipseItem->setScale(scale);
                break;
            }
            default:
                qWarning() << "Forme inconnue (" << type << ")";
        }

    }
    //qDebug() << "Apres le modded " << scene()->items();
    //qDebug() << "----------------------------------------------------------";
}

