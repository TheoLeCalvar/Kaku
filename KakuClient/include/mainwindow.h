#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QMap>
#include <QAction>
#include <QComboBox>
#include <QIcon>
#include <QToolBar>
#include <QString>
#include <QPen>
#include <QLineEdit>
#include <QFileDialog>
#include <QScrollBar>
#include <QMessageBox>
#include <QSpinBox>
#include <QKeySequence>
#include <QApplication>

#include "myqview.h"
#include "mychatbox.h"
#include "mydemandemain.h"
#include "kakuselectcolor.h"
#include "kakuselectcolorborder.h"
#include "mySvg.h"
#include "dpointpolygone.h"
#include "ClientSocket.h"
#include "dialogConnexion.h"

//fenetre pricipal de notre programe
class MainWindow: public QWidget
{
Q_OBJECT

private:

    //Client
    ClientSocket *      _socket;
    DialogConnexion *   _dialogConnexion;

    //Menu
    QMenuBar *  _mBar;
    QMenu *     _mFichier;
    QMenu *     _mEdition;
    QMenu *     _mOption;
    QAction *   _aNouveau;
    QAction *   _aSauvegarde;
    QAction *   _aChargement;
    QAction *   _aQuitter;
    QAction *   _aboutQT;
    QAction *   _aSuppr;

    //Toolbar principale

    QToolBar *  _tBarMenu;

    //style de ligne
    QComboBox * _cTypePinceau;
    QSpinBox *  _eTaillePinceau;

    //taille texte
    QSpinBox *  _eTailleTexte;

    //couleur figures & texte
    KakuSelectColor *       _kCouleur;
    KakuSelectColor *       _kCouleurTexte;
    KakuSelectColorBorder * _kCouleurbord;

    QAction *   _actionText;
    QAction *   _actionfigure;
    QAction *   _actionGomme;
    QAction *   _actionSelection;
    QAction *   _actionDrag;

    //Toolbar des figures

    QToolBar *  _toolbarfigure;

    QMap<QString, QAction *>    _figures;


    // Autre
    MyDemandeMain *     _mListeMain;
    MyChatBox *         _cChatBox;
    MyQView *           _view;
    QGraphicsScene *    _scene;

    DPointPolygone *    _dNbPoint;

    // Layout

    QVBoxLayout *   _vLayoutPrincipal;
    QHBoxLayout *   _hLayoutPrincipal;
    QVBoxLayout *   _vLayoutSuper;

    // Line edit pour le text

    QLineEdit *  _lineEdit;

    //Explorateur de fichiers

    QFileDialog *   _dExplorer;

    //booleen bloquant l'accès aux fonctions affectant le dessin

    bool    peutDessiner;

public:

    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void connexion();

private slots:

/* ---------------------------------------- */
/* ---------------------------------------- */
/*              Slots    toolbar            */
/* ---------------------------------------- */
/* ---------------------------------------- */

    /*active le mode dessin figure
     *clear les elements selectionnés
     *affiche la toolbar des figures
     */
    void slotAfficheToolbarFigure();

    /*active le mode ajout texte
     *clear les elements selectionner
    */
    void slotSelectionnerTexte();

    //active le mode selection multiple
    void slotSelectionner();

    /*active le mode gomme
     *clear la selection
     */
    void slotGomme();

    //active le mode drag
    void slotDrag();

/* ---------------------------------------- */
/* ---------------------------------------- */
/*      fin     Slots    toolbar            */
/* ---------------------------------------- */
/* ---------------------------------------- */

/* ---------------------------------------- */
/* ---------------------------------------- */
/*     Slots    toolbar   figure            */
/* ---------------------------------------- */
/* ---------------------------------------- */
//les slots suivants sont activés quand on clic
//sur les QAction de la toolbar figure


    //mode dessin de figure:ligne
    void slotFigureLigne();
    //mode dessin de figure:triangle
    void slotFigureTriangle();
    //mode dessin de figure: carré
    void slotFigureCarre();
    //mode dessin de figure:rectangle
    void slotFigureRectangle();
    //mode dessin de figure:polygone
    void slotFigurePolygone();
    //mode dessin de figure:ellipse
    void slotFigureEllipse();
    //mode dessin de figure:cercle
    void slotFigureCercle();
    //Ouvre le QDialog permettant de choisir le nombre de point des polygones min 4 max 10
    void slotOuvrirNbPoint();

/* ---------------------------------------- */
/* ---------------------------------------- */
/*  fin   Slots    toolbar   figure         */
/* ---------------------------------------- */
/* ---------------------------------------- */


/* ---------------------------------------- */
/* ---------------------------------------- */
/*    Slots    edition document             */
/* ---------------------------------------- */
/* ---------------------------------------- */

    //ouvre l'explorateur de fichier pour la sauvegarde
    void slotSave();
    //permet d'eviter de perdre son dessin à partir d'un QDialog
    //pour demander si on veut save avant de load
    void slotPreLoad();
    //permet de charger un fichier
    void slotLoad();

    //ouvre une fenetre proposant de save
    //avant de clear la scene
    void slotNew();

/* ---------------------------------------- */
/* ---------------------------------------- */
/*   fin  Slots    edition document         */
/* ---------------------------------------- */
/* ---------------------------------------- */

    //Ouvre un popup permettant d'écrire le texte à ajouter à la scene
    void slotOuvrirLineEdit(int x, int y);
    //Ajout le texte à la scene
    void slotEnvoyerText();

    //Lie le disconnect du socket à la fermeture de l'application
    void slotAboutToQuit();
    //permet de savoir si on a la main ou pas et de changer se statut via le boolean b
    void slotPeutDessiner(bool b);

    //Informe qu'on nous a déconnecté via un QMessageBox et ferme l'application
    void slotInfoQuit();

signals:

    void ouvrirNbPoint();
    void envoyerLineEdit(QString m);
    void clear();
};

#endif // MAINWINDOW_H
