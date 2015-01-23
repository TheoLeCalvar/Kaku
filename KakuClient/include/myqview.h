#ifndef MYQVIEW_H
#define MYQVIEW_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QPointF>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QGraphicsLineItem>
#include <QGraphicsPolygonItem>
#include <QList>
#include <QDataStream>
#include <QByteArray>
#include <QTime>

#include "Common.h"

class MyQView: public QGraphicsView
{
Q_OBJECT

public:

    typedef enum
    {
        ACTION_TEXTE        = 0x1,
        ACTION_SELECTION    = 0x2,
        ACTION_DRAG         = 0x3,
        ACTION_DELETE       = 0x4,
        ACTION_FIGURE       = 0x10,
        ACTION_LIGNE        = 0x11,
        ACTION_TRIANGLE     = 0x12,
        ACTION_CARRE        = 0x13,
        ACTION_RECTANGLE    = 0x14,
        ACTION_POLYGONE     = 0x15,
        ACTION_CERCLE       = 0x16,
        ACTION_ELLIPSE      = 0x17
    } ActionType;

private:

    // Listes des pointeurs vers les items
    QList<QGraphicsItem *> listItem;

    //pour savoir si on peut dessiner ou pas
    bool _peutDessiner;

    //couleur du contour des figures
    QColor  _borderColor;
    //couleur du fond des figures
    QColor  _mainColor;
    //couleur du text
    QColor  _textColor;

    //style du contour des figure
    Qt::PenStyle    _stylePinceau;

    //taille du contour des figure
    int _taillePinceau;
    //taille du texte
    int _tailleTexte;

    //permet de savoir quel type d'action on effectue
    ActionType _actionActive;

    //previsualisation

    //ligne temporaire lie a la previsualisation
    QGraphicsLineItem * _ligneTmp;
    //carre temporaire lie a la previsualisation
    QGraphicsRectItem * _rectTmp;
    //rectangle temporaire lie a la previsualisation
    QGraphicsRectItem * _rectangleTmp;
    //cercle temporaire lie a la previsualisation
    QGraphicsEllipseItem * _cercleTmp;
    //Ellipse temporaire lie a la previsualisation
    QGraphicsEllipseItem * _helypseTmp;
    //triangle temporaire lie a la previsualisation
    QGraphicsPolygonItem * _triangleTmp;
    //polygone temporaire lie a previsualisation
    QGraphicsPolygonItem * _polyTmp;

    //permet de savoir si on doit supprimer  une ligne temporaire
    bool _ligneASup;
    //permet de savoir si on doit supprimer un carre temporaire
    bool _carreASup;
    //permet de savoir si on doit supprimer un rectangle temporaire
    bool _RectaSup;
    //permet de savoir si on doit supprimer un cercle temporaire
    bool _cercleASup;
    //permet de savoir si on doit supprimer une ellipse temporaire
    bool _helypseASup;
    //permet de savoir si on doit supprimer un triangle temporaire
    bool _triangleASup;
    //permet de savoir si on doit supprimer un polygone temporaire
    bool _polyASup;

    // Les points

    //liste des point avec lequels dessiner
    QList<QPointF>  _points;
    //nombre de points du polygone à dessiner
    int _nombrePointPolygone;
    QPointF _pointLineEdit;

    /*position actuelle de la souris sur la vue, cette position est actualisée en permanance juste au survol lorsque l'on clique
     * et est utile pour la previsualisation des figure avant le dessin final*/
    QPointF _coordoneesSouris;

    // Item en mouvement

    qreal _xMovingItem;
    qreal _yMovingItem;

    //pour savoir si on presse un clic de souris
    bool _clicPressed;
    //pour savoir si nous sommes en train d'effectuer une selection multiple
    bool _estSelectioning;

public:

    // Constructeur et destructeur
    MyQView();
    MyQView(QGraphicsScene *scene, QWidget* parent = 0);
    virtual ~MyQView();

    //getter
    const QColor & getBorderColor() const{return _borderColor;}
    const QColor & getMainColor() const{return _mainColor;}

    bool getTextIsActivated() const {return _actionActive == ACTION_TEXTE;}
    bool getFigureIsActivated() const {return _actionActive & ACTION_FIGURE;}
    bool getSelectionIsactivated() const {return _actionActive == ACTION_SELECTION;}
    bool getDeleteIsActivated() const {return _actionActive ==  ACTION_DELETE;}

    bool getLigne() const {return _actionActive == ACTION_LIGNE;}
    bool getTriangle() const {return _actionActive == ACTION_TRIANGLE;}
    bool getRectangle() const {return _actionActive == ACTION_RECTANGLE;}
    bool getCarre() const {return _actionActive == ACTION_CARRE;}
    bool getPolygone() const {return _actionActive == ACTION_POLYGONE;}
    bool getEllipse() const {return _actionActive == ACTION_ELLIPSE;}
    bool getCercle() const {return _actionActive == ACTION_CERCLE;}
    int getNombrePointPolygone() const {return _nombrePointPolygone;}

    //fonction de dessin
    //dessine une figure si c'est possible
    //et supprime les figures temporaires liées a
    //la prévisualisation
    void draw();

    //fonction d'aperçu de dessin
    //appelée si on dessine mais que l'on ne peut pas dessiner
    //a chaque appelle supprime les figures temporaire et en créé de nouvelles
    //appelée a chaque changement de position du curseur
    void apercuCreation();

    //suprime les elements temporaires
    //et met les bool liés a leur supression a false
    void supressionElementTmp();

    //Clear la scene et la listItem
    void clear();
    //Set la liste d'item à partir d'une autre liste pour le chargement
    void setListItem(QList<QGraphicsItem *> list);

protected:

    //permet l'interraction avec le clic de souris
    virtual void mousePressEvent(QMouseEvent *e);
    //permet l'interraction avec le declic de souris
    virtual void mouseReleaseEvent(QMouseEvent *e);
    //permet l'interaction avec le deplacement de la souris
    //setMouseTracking(true); permet de pas avoir besoin de cliquer
    //pour avoir cette interaction
    virtual void mouseMoveEvent(QMouseEvent *mouseEvent);
    //permet l'interaction avec la molette de souris
    virtual void wheelEvent(QWheelEvent *ZoomEvent);

public slots:

    //recoit de la mainWindows la couleur de bordure des figures
    //egalement couleur des ligne
    void setBorderColor(QColor c);
    //recoit de la main windows la couleur de fond des figures
    void setMainColor(QColor c);
    //recoit de la main windows la couleur du texte
    void setTextColor(QColor c);
    //recoit de la main windows la taille du contour des figures
    //egalement la taille des lignes
    void setWidthPen(int);
    //recoit de la main windows la taille du text
    void setWidthText(int);
    //recoit de la main window le type de trait
    //egalement la type de ligne
    void setStylePen(int i);

    //passe en mode ajout texte
    //supprime le drag si il y en a un d'actif
    void setTextActivated();
    //passe en selection
    //supprime le drag si il y en a un d'actif
    void setSelectionActivated();
    //passe en mode drag
    //supprime le drag si il y en a un d'actif
    void setDragActivated();
    //passe en mode delete
    //supprime le drag si il y en a un d'actif
    void setDeleteActivated();

    //les slots suivants permettent de passer en ajout figure et
    //supprime le drag si il y en a un d'actif
    void setLigne();//mode ajout ligne
    void setTriangle();//mode ajout triangle
    void setRectangle();//mode ajout rectangle
    void setCarre();//mode ajout carre
    void setPolygone();//mode ajout polygone
    void setEllipse();//mode ajout ellipse
    void setCercle();//mode ajout cercle

    void slotAfficherText(QString);
    void slotNbPoint(int n);

    void slotSuppression();

    //Slot des messages reçus

    //Emis lorsqu'un élément a été ajouté par celui qui controle
    void slotAddedElement(quint16 type, const QByteArray & args);

    //Emis lorsqu'un élément a été modifié
    void slotModedElement(quint16 id, quint16 type, const QByteArray & args);

    //Emis lorsqu'un élément a été supprimé
    void slotDeletedElement(quint16 id);

    //Emis lorsque la scène vient d'être nettoyée
    void slotCleared();

    void slotPeutDessiner(bool dessin);

signals:

    void ouvrirLineEdit(int x, int y);

    //signaux des ajout/changements/suppression des elements
    void addedElement(quint16 type, const QByteArray & args);
    void modedElement(quint16 id, quint16 type, const QByteArray & args);
    void deletedElement(quint16 id);

};

#endif // MYQVIEW_H
