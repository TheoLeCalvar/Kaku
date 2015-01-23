#ifndef MYSVG_H
#define MYSVG_H

#include <QString>
#include <QList>
#include <QGraphicsItem>
#include <QDomNode>
#include <QDomDocument>
#include <QDomElement>
#include <QDomAttr>
#include <QDomCDATASection>
#include <QFile>
#include <QTextStream>
#include <QBrush>
#include <QPen>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QGraphicsLineItem>
#include <QDebug>
#include <algorithm>
#include <QStringList>

namespace SVG
{
        /**
        * @brief Sauvegarde la scène sous format svg
        * @param scene Scene à sauvegarder
        * @param path Chemin du fichier une fois sauvegardé
        */
        void writeSVG(QGraphicsScene* scene, const QString & path);
        /**
         * @brief Renvoie l'item rentrée en paramètre sous forme d'élément XML
         *
         * @param doc document XML dans lequel sera ajouté l'item
         * @param item item a transformé en élément XML
         *
         * @return élément XML correspondant à l'item
         */
        QDomElement writeItem(QDomDocument & doc, QGraphicsItem * item);
        /**
         * @brief Charge un fichier XML et renvoie la liste des items à ajouter à la scène
         *
         * @param path Chemin du fichier xml
         * @return liste d'items qui seront ajoutés à la liste
         */
        QList<QGraphicsItem *> readFromSVG(const QString & path);
        /**
         * @brief Renvoie l'item correspondant à l'élement XML donné en paramètre
         *
         * @param item Element XML a transformé en QGraphicsItem
         * @return item a ajouté à la scene
         */
        QGraphicsItem * readItem(const QDomElement & item);
}

#endif // MYSVG_H
