#include "mySvg.h"


QList<QGraphicsItem *> SVG::readFromSVG(const QString & path)
{
    QList<QGraphicsItem *> list;

    QDomDocument doc;
    QFile file(path);

    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Fichier inaccessible";
        throw 1;
    }

    if(!doc.setContent(&file))
    {
        qDebug() << "return setContent";
    }
    file.close();

    QDomElement racine = doc.documentElement();

    QDomElement item = racine.firstChildElement(); // Description

    item = item.nextSiblingElement();

    while(!item.isNull())
    {
        list.push_front(readItem(item));
        item = item.nextSiblingElement();
    }

    return list;
}

QGraphicsItem * SVG::readItem(const QDomElement & item)
{
    if(item.tagName() == "rect")
    {
        int x = item.attribute("x").toInt(NULL, 10);
        int y = item.attribute("y").toInt(NULL, 10);
        int width = item.attribute("width").toInt(NULL, 10);
        int height = item.attribute("height").toInt(NULL, 10);
        QColor cBrush(item.attribute("fill"));
        QColor cPen(item.attribute("stroke"));
        QBrush brush(cBrush);
        QPen pen(cPen);
        qreal scale = item.attribute("scale").toFloat();

        pen.setWidth(item.attribute("stroke-width").toInt(NULL,10));
        if(item.attribute("stroke-dasharray") == "2,2")
        pen.setStyle(Qt::DotLine);
        else
        if(item.attribute("stroke-dasharray") == "5,5")
          pen.setStyle(Qt::DashLine);

        QGraphicsRectItem* r = new QGraphicsRectItem(0, 0, width, height);
        r->setPos(x,y);
        r->setFlag(QGraphicsItem::ItemIsMovable, true);
        r->setFlag(QGraphicsItem::ItemIsSelectable,true);
        r->setFlag(QGraphicsItem::ItemSendsGeometryChanges,true);

        r->setPen(pen);
        r->setBrush(brush);
        r->setScale(scale);

        return r;
    }
    else if(item.tagName() == "line")
    {
        int x1 = item.attribute("x1").toInt(NULL, 10);
        int y1 = item.attribute("y1").toInt(NULL, 10);
        int x2 = item.attribute("x2").toInt(NULL, 10);
        int y2 = item.attribute("y2").toInt(NULL, 10);
        QColor cPen(item.attribute("stroke"));
        QPen pen(cPen);
        qreal scale = item.attribute("scale").toFloat();


        pen.setWidth(item.attribute("stroke-width").toInt(NULL,10));
        if(item.attribute("stroke-dasharray") == "2,2")
          pen.setStyle(Qt::DotLine);
        else
          if(item.attribute("stroke-dasharray") == "5,5")
            pen.setStyle(Qt::DashLine);

        QGraphicsLineItem* l = new QGraphicsLineItem(0, 0, x2-x1, y2-y1);
        l->setPos(x1,y1);
        l->setFlag(QGraphicsItem::ItemIsMovable, true);
        l->setFlag(QGraphicsItem::ItemIsSelectable,true);
        l->setFlag(QGraphicsItem::ItemSendsGeometryChanges,true);

        l->setPen(pen);
        l->setScale(scale);

        return l;
    }
    else if(item.tagName() == "text")
    {
        int x = item.attribute("x").toInt(NULL, 10);
        int y = item.attribute("y").toInt(NULL, 10);
        QFont f(item.attribute("font-family"), item.attribute("font-size").toInt(NULL, 10));

        QColor c(item.attribute("stroke"));
        QString text(item.text());
        qreal scale = item.attribute("scale").toFloat();


        QGraphicsTextItem* t = new QGraphicsTextItem(text);
        t->setFlag(QGraphicsItem::ItemIsMovable, true);
        t->setFlag(QGraphicsItem::ItemIsSelectable,true);
        t->setFlag(QGraphicsItem::ItemSendsGeometryChanges,true);
        //t->setTextInteractionFlags(Qt::TextEditorInteraction);

        t->setDefaultTextColor(c);
        t->setFont(f);
        t->setX(x);
        t->setY(y);
        t->setScale(scale);

        return t;
    }
    else if(item.tagName() == "polygon")
    {
        QString points(item.attribute("points"));
        QColor cBrush(item.attribute("fill"));
        QColor cPen(item.attribute("stroke"));
        QBrush brush(cBrush);
        QPen pen(cPen);
        qreal scale = item.attribute("scale").toFloat();


        pen.setWidth(item.attribute("stroke-width").toInt(NULL,10));
        if(item.attribute("stroke-dasharray") == "2,2")
            pen.setStyle(Qt::DotLine);
        else if(item.attribute("stroke-dasharray") == "5,5")
            pen.setStyle(Qt::DashLine);

        QStringList listePositions = points.split(" ");
        QVector<QPointF> v;

        for(int i = 0; i < listePositions.size(); i++)
        {
            QStringList lXY = listePositions[i].split(",");
            QPointF p(lXY[0].toInt(NULL,10), lXY[1].toInt(NULL,10));
            v.push_back(p);
        }

        QPointF p1 = v[0];
        v.remove(0);
        v.push_front(QPointF(0,0));

        for(int i = 1; i < v.size(); i++)
        {
            v[i] = v[i] - p1;
        }

        QGraphicsPolygonItem* p = new QGraphicsPolygonItem(v);
        p->setPos(p1);
        p->setFlag(QGraphicsItem::ItemIsMovable, true);
        p->setFlag(QGraphicsItem::ItemIsSelectable,true);
        p->setFlag(QGraphicsItem::ItemSendsGeometryChanges,true);

        p->setPen(pen);
        p->setBrush(brush);
        p->setScale(scale);

        return p;
    }
    else if(item.tagName() == "ellipse")
    {
        int x = item.attribute("cx").toInt(NULL, 10);
        int y = item.attribute("cy").toInt(NULL, 10);
        int width = item.attribute("rx").toInt(NULL, 10);
        int height = item.attribute("ry").toInt(NULL, 10);
        QColor cBrush(item.attribute("fill"));
        QColor cPen(item.attribute("stroke"));
        QBrush brush(cBrush);
        QPen pen(cPen);

        qreal scale = item.attribute("scale").toFloat();

        pen.setWidth(item.attribute("stroke-width").toInt(NULL,10));
        if(item.attribute("stroke-dasharray") == "2,2")
            pen.setStyle(Qt::DotLine);
        else if(item.attribute("stroke-dasharray") == "5,5")
          pen.setStyle(Qt::DashLine);

        QGraphicsEllipseItem* e = new QGraphicsEllipseItem(0, 0, width*2, height*2);
        e->setPos(x-width,y-height);
        e->setFlag(QGraphicsItem::ItemIsMovable, true);
        e->setFlag(QGraphicsItem::ItemIsSelectable,true);
        e->setFlag(QGraphicsItem::ItemSendsGeometryChanges,true);

        e->setPen(pen);
        e->setBrush(brush);

        e->setScale(scale);

        return e;
    }

  QGraphicsRectItem * re = new QGraphicsRectItem();

  return re;
}

void SVG::writeSVG(QGraphicsScene* scene, const QString & path)
{
    if(!path.isEmpty())
    {
        QFile file;
        QTextStream out;

        //qDebug() << "Le chemin n'est pas vide : " << cheminFichier;

        //qDebug() << "Le chemin n'est pas vide : " << chemin;
        file.setFileName(path);
        if(!file.open(QIODevice::WriteOnly))
            return;
        out.setDevice(&file);

        QDomDocument doc;
        QDomElement svg = doc.createElement("svg");

        svg.setAttribute("width",QString::number(scene->width()));
        svg.setAttribute("height",QString::number(scene->height()));
        svg.setAttribute("xmlns","http://www.w3.org/2000/svg");
        //svg.setAttributeNS("xmlns", "xlink", "http://www.w3.org/1999/xlink");
        svg.setAttribute("version", "1.1");

        QDomElement desc = doc.createElement("desc");
        QDomText tDesc = doc.createTextNode("Rendu SVG du dessin collaboratif réalisé avec l'application Kaku.");
        desc.appendChild(tDesc);

        svg.appendChild(desc);

        for(auto item : scene->items())
        {
            svg.appendChild(writeItem(doc,item));
        }
            doc.appendChild(svg);

        //fin sauvegarde
        QDomNode n = doc.createProcessingInstruction("xml","version=\"1.0\"");
        doc.insertBefore(n,doc.firstChild());

        doc.save(out,2);
        file.close();
    }
}

QDomElement SVG::writeItem(QDomDocument & doc, QGraphicsItem * item)
{
  /*Types des item avec int pour faire le switch :
        *
        * - PathItem : 2
        * - RectItem : 3
        * - EllipseItem : 4
        * - PolygonItem : 5
        * - LineItem : 6
        * - PixmapItem : 7
        * - TextItem : 8
        * - SimpleTextItem : 9

  */
        int typeItem = item->type();

        switch(typeItem)
        {
            case 3 :
            {
                QDomElement rectangle = doc.createElement("rect");
                if(item->type() == QGraphicsRectItem::Type)
                {
                    QGraphicsRectItem* r = qgraphicsitem_cast<QGraphicsRectItem *>(item);
                    rectangle.setAttribute("x",(int)r->x());
                    rectangle.setAttribute("y",(int)r->y());
                    rectangle.setAttribute("width", r->rect().width());
                    rectangle.setAttribute("height",r->rect().height());
                    // rectangle.setAttribute("zValue",r->zValue());
                    //rectangle.setAttribute("fill-opacity",r->opacity());
                    rectangle.setAttribute("stroke",r->pen().color().name());
                    rectangle.setAttribute("stroke-width",r->pen().width());
                    if(r->pen().style() == Qt::DashLine)
                    rectangle.setAttribute("stroke-dasharray","5,5");
                    else
                    if(r->pen().style() == Qt::DotLine)
                      rectangle.setAttribute("stroke-dasharray","2,2");
                    else
                      rectangle.setAttribute("stroke-dasharray",0);
                    rectangle.setAttribute("fill",r->brush().color().name());
                    rectangle.setAttribute("scale",QString::number(r->scale()));

                    return rectangle;
                }

              break;
            }

            case 4 :
            {
                QDomElement ellipse = doc.createElement("ellipse");
                if(item->type() == QGraphicsEllipseItem::Type)
                {
                    QGraphicsEllipseItem* e = qgraphicsitem_cast<QGraphicsEllipseItem *>(item);
                    ellipse.setAttribute("cx", (int)(e->x()+e->rect().width()/2));
                    ellipse.setAttribute("cy", (int)(e->y()+e->rect().height()/2));
                    ellipse.setAttribute("rx", (int)e->rect().width()/2);
                    ellipse.setAttribute("ry", (int)e->rect().height()/2);
                    // ellipse.setAttribute("zValue",e->zValue());
                    // ellipse.setAttribute("opacity",e->opacity());
                    ellipse.setAttribute("stroke",e->pen().color().name());
                    ellipse.setAttribute("stroke-width",e->pen().width());
                    if(e->pen().style() == Qt::DashLine)
                     ellipse.setAttribute("stroke-dasharray","5,5");
                    else
                     if(e->pen().style() == Qt::DotLine)
                       ellipse.setAttribute("stroke-dasharray","2,2");
                     else
                       ellipse.setAttribute("stroke-dasharray",0);
                    ellipse.setAttribute("fill",e->brush().color().name());
                    ellipse.setAttribute("scale",QString::number(e->scale()));

                    return ellipse;
                }

            break;
          }

        case 5 :
        {
            QDomElement polygon = doc.createElement("polygon");
            if(item->type() == QGraphicsPolygonItem::Type)
            {
                QGraphicsPolygonItem* p = qgraphicsitem_cast<QGraphicsPolygonItem *>(item);
                QString points("");

                points.append(QString::number(int(p->x())));
                points.append(",");
                points.append(QString::number(int(p->y())));
                points.append(", ");

                for(int i = 1; i < p->polygon().size(); i++)
                {
                points.append(QString::number((int)(p->polygon()[i].x() + p->x() )));
                points.append(",");
                points.append(QString::number((int)(p->polygon()[i].y() + p->y() )));
                points.append(", ");
                }
                //points.remove(2,1);
                points.remove(points.size()-1,1);

                polygon.setAttribute("points",points);

                //TODO : trouver le moyen de récuperer les points d'un polygon :)
                // polygon.setAttribute("zValue",p->zValue());
                // polygon.setAttribute("opacity",p->opacity());
                polygon.setAttribute("stroke",p->pen().color().name());
                polygon.setAttribute("stroke-width",p->pen().width());
                if(p->pen().style() == Qt::DashLine)
                polygon.setAttribute("stroke-dasharray","5,5");
                else
                if(p->pen().style() == Qt::DotLine)
                  polygon.setAttribute("stroke-dasharray","2,2");
                else
                  polygon.setAttribute("stroke-dasharray",0);
                polygon.setAttribute("fill",p->brush().color().name());
                polygon.setAttribute("scale",QString::number(p->scale()));

                return polygon;
            }

            break;
        }
        case 6 :
        {
            QDomElement line = doc.createElement("line");
            if(item->type() == QGraphicsLineItem::Type)
            {
                QGraphicsLineItem* l = qgraphicsitem_cast<QGraphicsLineItem *>(item);
                line.setAttribute("x1", (int)(l->line().x1() + l->x() ));
                line.setAttribute("y1", (int)(l->line().y1() + l->y() ));
                line.setAttribute("x2", (int)(l->line().x2() + l->x() ));
                line.setAttribute("y2", (int)(l->line().y2() + l->y() ));
                // polygon.setAttribute("zValue",p->zValue());
                // polygon.setAttribute("opacity",p->opacity());
                line.setAttribute("stroke",l->pen().color().name());
                line.setAttribute("stroke-width",l->pen().width());
                if(l->pen().style() == Qt::DashLine)
                line.setAttribute("stroke-dasharray","5,5");
                else
                if(l->pen().style() == Qt::DotLine)
                  line.setAttribute("stroke-dasharray","2,2");
                else
                  line.setAttribute("stroke-dasharray",0);
                line.setAttribute("scale",QString::number(l->scale()));

                return line;
            }

            break;
        }
        case 8 :
        {
            QDomElement text = doc.createElement("text");
            if(item->type() == QGraphicsTextItem::Type)
            {
                QGraphicsTextItem* p = qgraphicsitem_cast<QGraphicsTextItem *>(item);
                text.setAttribute("x", (int)p->x());
                text.setAttribute("y", (int)p->y());
                text.setAttribute("font-size", p->font().pointSize());
                QDomText t = doc.createTextNode(p->toPlainText());
                text.appendChild(t);
                text.setAttribute("stroke",p->defaultTextColor().name());
                text.setAttribute("fill",p->defaultTextColor().name());
                text.setAttribute("font-family",p->font().family());
                text.setAttribute("scale",QString::number(p->scale()));


                //QDomElement
                //polygon.appendChild();

                return text;
            }

            break;
        }
           default :
              qDebug() << "item not availible";
        }
        QDomElement elementVide;
        return elementVide;
}
