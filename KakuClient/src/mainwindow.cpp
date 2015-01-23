#include "mainwindow.h"

#include <QDebug>
#include <iostream>


MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    //Socket

    _socket = new ClientSocket(this);

    connect( qApp, SIGNAL(aboutToQuit()), this, SLOT(slotAboutToQuit()) );
    //connect(_socket, SIGNAL(kicked()), this, SLOT(slotAboutToQuit()));


    this->setMinimumSize(1100,600);
    this->setWindowTitle(tr("Kaku Project"));

    peutDessiner = false;

    // Layout

    _vLayoutPrincipal = new QVBoxLayout();
    _hLayoutPrincipal = new QHBoxLayout();
    _vLayoutSuper = new QVBoxLayout(this);

    // Boutons

    _mListeMain = new MyDemandeMain();
    _cChatBox = new MyChatBox();

    // Zone de dessin
    _scene = new QGraphicsScene(0,0,5000,5000);
    _view = new MyQView(_scene);



    _dialogConnexion = new DialogConnexion(this);
    int i = _dialogConnexion->exec();

    if(i == 1)
    {
        connexion();

        // lineEdit pour la création de texte
        _lineEdit = new QLineEdit();
        _lineEdit->setWindowTitle(tr("Création de texte"));

        connect(_view,SIGNAL(ouvrirLineEdit(int,int)),this,SLOT(slotOuvrirLineEdit(int,int)));
        connect(_lineEdit,SIGNAL(returnPressed()),this,SLOT(slotEnvoyerText()));
        connect(_lineEdit,SIGNAL(returnPressed()),_lineEdit,SLOT(close()));

        connect(this,SIGNAL(envoyerLineEdit(QString)), _view,SLOT(slotAfficherText(QString)));

        // QDialog pour le nombre de points d'un polygone
        _dNbPoint = new DPointPolygone();
        _dNbPoint->setWindowTitle(tr("Nombre de points"));

        connect(this,SIGNAL(ouvrirNbPoint()),this,SLOT(slotOuvrirNbPoint()));
        connect(_dNbPoint,SIGNAL(sannuler()),_dNbPoint,SLOT(close()));
        connect(_dNbPoint,SIGNAL(sok(int)),_dNbPoint,SLOT(close()));
        connect(_dNbPoint,SIGNAL(sok(int)),_view,SLOT(slotNbPoint(int)));

        //dialogue
        _dExplorer = new QFileDialog();

        /* ***************************************** */
        /*          bar de menu                      */
        /* ***************************************** */
        // creation Menu

        _mBar = new QMenuBar(this);
        _mFichier = new QMenu(tr("Fichier"),_mBar);
        _mEdition = new QMenu(tr("Edition"),_mBar);
        _mOption = new QMenu(tr("Options"), _mBar);

        _aNouveau = new QAction(tr("Nouveau"), _mFichier);
        QKeySequence raccourciNew("Ctrl+n");
        _aNouveau->setShortcut(raccourciNew);

        _aSauvegarde = new QAction(tr("Sauvegarder"), _mFichier);
        QKeySequence raccourciSave("Ctrl+s");
        _aSauvegarde->setShortcut(raccourciSave);

        _aChargement = new QAction(tr("Chargement"), _mFichier);
        QKeySequence raccourciLoad("Ctrl+o");
        _aChargement->setShortcut(raccourciLoad);

        _aQuitter = new QAction(tr("Quitter"), _mFichier);
        QKeySequence raccourciQuit("Ctrl+q");
        _aQuitter->setShortcut(raccourciQuit);
            connect(  _aQuitter, SIGNAL(triggered()), this, SLOT(close()));

        _aboutQT = new QAction(tr("QT?"),_mOption);
            _aboutQT->setIcon(QIcon("Image/QT.jpeg"));
            connect(_aboutQT,SIGNAL(triggered()),qApp,SLOT(aboutQt()));

        // Ajout des element
        QList<QKeySequence> listRacc;
        listRacc << QKeySequence::Delete;
        listRacc << Qt::Key_Backspace;

        _aSuppr = new QAction(tr("Supprimer"),_mEdition);
        _aSuppr->setShortcuts(listRacc);
            connect(_aSuppr, SIGNAL(triggered()), _view, SLOT(slotSuppression()));


        _mBar->addMenu(_mFichier);
        _mBar->addMenu(_mEdition);
        _mBar->addMenu(_mOption);

        _mFichier->addAction(_aNouveau);
            _aNouveau->setIcon(QIcon("Image/iconenouveau.png"));
        _mFichier->addAction(_aSauvegarde);
            _aSauvegarde->setIcon(QIcon("Image/sauvegarder.png"));
        _mFichier->addAction(_aChargement);
            _aChargement->setIcon(QIcon("Image/load.jpeg"));
        _mFichier->addAction(_aQuitter);

        _mEdition->addAction(_aSuppr);

        _mOption->addAction(_aboutQT);

        /* ***************************************** */
        /*         fin  bar de menu                  */
        /* ***************************************** */



        /* ***************************************** */
        /*          Toolbar princiale                */
        /* ***************************************** */

        //creation de la toolbare
        _tBarMenu = new QToolBar();

        //qaction unique a la toolbar principale
        _actionText = new QAction(tr("Texte"),_mFichier);
            _actionText->setIcon(QIcon("Image/texte.png"));
            _actionText->setCheckable(true);
            _actionText->setChecked(true);//choix par defaut au lancement du programe
            connect(_actionText,SIGNAL(triggered()), this, SLOT(slotSelectionnerTexte()));

        _actionfigure = new QAction(tr("Figure"),_mFichier);
            _actionfigure->setIcon(QIcon(("Image/Triangle.svg")));
            _actionfigure->setCheckable(true);
            connect(_actionfigure,SIGNAL(triggered()), this, SLOT(slotAfficheToolbarFigure()));

         _actionGomme = new QAction(tr("Gomme"),_mFichier);
            _actionGomme->setIcon(QIcon("Image/gomme.jpeg"));
            _actionGomme->setCheckable(true);
            connect(_actionGomme,SIGNAL(triggered()),this, SLOT(slotGomme()));

         _actionSelection = new QAction(tr("Selection"),_mFichier);
            _actionSelection->setIcon(QIcon("Image/selection.png"));
            _actionSelection->setCheckable(true);
            connect(_actionSelection,SIGNAL(triggered()),this,SLOT(slotSelectionner()));

        _actionDrag = new QAction(tr("Deplacer"),_mFichier);
            _actionDrag->setIcon(QIcon("Image/main.png"));
            _actionDrag->setCheckable(true);
            connect(_actionDrag,SIGNAL(triggered()),this,SLOT(slotDrag()));

        //widget de la toolbare principal
        _kCouleur = new KakuSelectColor();
        _kCouleurTexte = new KakuSelectColor();
        _kCouleurbord = new KakuSelectColorBorder();

        _cTypePinceau = new QComboBox();
            _cTypePinceau->addItem(QIcon("Image/Solid.svg"), tr("Ligne pleine"));
            _cTypePinceau->addItem(QIcon("Image/Dash.svg"), tr("Ligne en tirets"));
            _cTypePinceau->addItem(QIcon("Image/Dot.svg"), tr("Ligne en pointillés"));

        _eTaillePinceau = new QSpinBox;
            _eTaillePinceau->setRange(1, 10);
            _eTaillePinceau->setValue(1);
        _eTailleTexte = new QSpinBox;
            _eTailleTexte->setRange(10, 30);
            _eTailleTexte->setValue(12);

        // Ajout dans les toolBar
        _tBarMenu->addAction(_aNouveau);
            connect(_aNouveau,SIGNAL(triggered()),this,SLOT(slotNew()));
        _tBarMenu->addAction(_aSauvegarde);
            connect(_aSauvegarde,SIGNAL(triggered()),this,SLOT(slotSave()));
        _tBarMenu->addAction(_aChargement);
            connect(_aChargement,SIGNAL(triggered()),this,SLOT(slotPreLoad()));

        _tBarMenu->addSeparator();
        _tBarMenu->addAction(_actionText);
        _tBarMenu->addWidget(_kCouleurTexte);
        _tBarMenu->addWidget(_eTailleTexte);
        _tBarMenu->addSeparator();
        _tBarMenu->addAction(_actionfigure);
        _tBarMenu->addWidget(_kCouleur);
        _tBarMenu->addWidget(_kCouleurbord);
        _tBarMenu->addWidget(_cTypePinceau);
        _tBarMenu->addWidget(_eTaillePinceau);
        _tBarMenu->addSeparator();
        _tBarMenu->addAction(_actionSelection);
        _tBarMenu->addAction(_actionDrag);
        //_tBarMenu->addAction(_actionGomme);


        /* ***************************************** */
        /*         Fin  Toolbar principale           */
        /* ***************************************** */


        /* ***************************************** */
        /*          Toolbar figure                   */
        /* ***************************************** */

        //creation de la toolbar

        _toolbarfigure = new QToolBar();
            _toolbarfigure->setOrientation(Qt::Vertical);
            _toolbarfigure->setHidden(true);

        //creation des actions de la toolbarfigure

        QAction* figureLigne = new QAction(tr("Ligne"), _toolbarfigure);
        QAction* figureTriangle = new QAction(tr("Triangle"), _toolbarfigure);
        QAction* figureRectangle = new QAction(tr("Rectangle"), _toolbarfigure);
        QAction* figureCarre = new QAction(tr("Carre"), _toolbarfigure);
        QAction* figurePolygone = new QAction(tr("Polygone"), _toolbarfigure);
        QAction* figureElypse = new QAction(tr("Elypse"), _toolbarfigure);
        QAction* figureCercle = new QAction(tr("Cercle"), _toolbarfigure);
        //fixe les image des qaction de la toolbarfigure

        figureTriangle->setChecked(true);

        figureLigne->setCheckable(true);
        figureTriangle->setCheckable(true);
        figureRectangle->setCheckable(true);
        figureCarre->setCheckable(true);
        figurePolygone->setCheckable(true);
        figureElypse->setCheckable(true);
        figureCercle->setCheckable(true);

        connect(figureLigne,SIGNAL(triggered()), this, SLOT(slotFigureLigne()));
        connect(figureTriangle,SIGNAL(triggered()), this, SLOT(slotFigureTriangle()));
        connect(figureRectangle,SIGNAL(triggered()), this, SLOT(slotFigureRectangle()));
        connect(figureCarre,SIGNAL(triggered()), this, SLOT(slotFigureCarre()));
        connect(figurePolygone,SIGNAL(triggered()), this, SLOT(slotFigurePolygone()));
        connect(figureElypse,SIGNAL(triggered()), this, SLOT(slotFigureEllipse()));
        connect(figureCercle,SIGNAL(triggered()), this, SLOT(slotFigureCercle()));

        figureLigne->setIcon(QIcon("Image/Ligne.svg"));
        figureTriangle->setIcon(QIcon("Image/Triangle.svg"));
        figureRectangle->setIcon(QIcon("Image/Rectangle.svg"));
        figureCarre->setIcon(QIcon("Image/Carre.svg"));
        figurePolygone->setIcon(QIcon("Image/Polygone.svg"));
        figureElypse->setIcon(QIcon("Image/Ellipse.svg"));
        figureCercle->setIcon(QIcon("Image/Cercle.svg"));

         // Ajout dans les toolBar

        _toolbarfigure->addAction(figureLigne);
        _toolbarfigure->addAction(figureTriangle);
        _toolbarfigure->addAction(figureRectangle);
        _toolbarfigure->addAction(figureCarre);
        _toolbarfigure->addAction(figurePolygone);
        _toolbarfigure->addAction(figureElypse);
        _toolbarfigure->addAction(figureCercle);

        _figures[tr("ligne")] = figureLigne;
        _figures[tr("triangle")] = figureTriangle;
        _figures[tr("rectangle")] = figureRectangle;
        _figures[tr("carre")] = figureCarre;
        _figures[tr("polygone")] = figurePolygone;
        _figures[tr("ellipse")] = figureElypse;
        _figures[tr("cercle")] = figureCercle;

        /* ***************************************** */
        /*          Fin  Toolbar figure              */
        /* ***************************************** */

        /*ajout tollbare figure*/
        _hLayoutPrincipal->addWidget(_toolbarfigure);

        // Ajout dans les Layout
        _vLayoutPrincipal->addWidget(_mListeMain);
        _vLayoutPrincipal->addSpacing(20);
        _vLayoutPrincipal->addWidget(_cChatBox);

        _hLayoutPrincipal->addWidget(_view);
        _hLayoutPrincipal->addLayout(_vLayoutPrincipal);

        _vLayoutSuper->addWidget(_tBarMenu);
        _vLayoutSuper->addLayout(_hLayoutPrincipal);

        _vLayoutSuper->setMenuBar(_mBar);

        // Connect QView

        connect(_kCouleur,SIGNAL(signalColor(QColor)),_view,SLOT(setMainColor(QColor)));
        connect(_kCouleurbord,SIGNAL(signalColor(QColor)),_view,SLOT(setBorderColor(QColor)));
        connect(_kCouleurTexte,SIGNAL(signalColor(QColor)), _view,SLOT(setTextColor(QColor)));
        connect(_eTaillePinceau,SIGNAL(valueChanged(int)),_view,SLOT(setWidthPen(int)));
        connect(_eTailleTexte,SIGNAL(valueChanged(int)),_view,SLOT(setWidthText(int)));
        connect(_cTypePinceau,SIGNAL(currentIndexChanged(int)),_view,SLOT(setStylePen(int)));

        connect(_mListeMain,SIGNAL(PeutDessiner(bool)),_view,SLOT(slotPeutDessiner(bool)));

        // Connect MyChatBox
        connect(_socket,SIGNAL(clientConnected(QString,bool)),_cChatBox,SLOT(slotClientConnecte(QString,bool)));
        connect(_socket,SIGNAL(clientDisconnected(QString)),_cChatBox,SLOT(slotClientDeconnecte(QString)));
        connect(_socket,SIGNAL(receivedMessage(QString)),_cChatBox,SLOT(slotMessageRecu(QString)));

        connect(_cChatBox,SIGNAL(envoiMessage(QString)),_socket,SLOT(sendMessage(QString)));

        // Connect MyDemandeMain
        connect(_socket,SIGNAL(clientConnected(QString,bool)),_mListeMain,SLOT(slotCConnecte(QString,bool)));
        connect(_socket,SIGNAL(clientDisconnected(QString)),_mListeMain,SLOT(slotCDeconnecte(QString)));
        connect(_socket,SIGNAL(controlList(QStringList)),_mListeMain,SLOT(slotListeMain(QStringList)));

        connect(_mListeMain,SIGNAL(DemandeControl()),_socket,SLOT(askForControl()));
        connect(_mListeMain,SIGNAL(QuitterControl()),_socket,SLOT(removeControl()));
        connect(_mListeMain,SIGNAL(QuitterDessiner()),_socket,SLOT(releaseControl()));

        connect(_mListeMain,SIGNAL(PeutDessiner(bool)),this,SLOT(slotPeutDessiner(bool)));

        // Connect view au socket
        connect(_socket, SIGNAL(cleared()), _view, SLOT(slotCleared() ));
        connect(_view, SIGNAL(addedElement(quint16,QByteArray)), _socket, SLOT(addElement(quint16,QByteArray)));
        connect(_socket, SIGNAL(addedElement(quint16,QByteArray)), _view, SLOT(slotAddedElement(quint16,QByteArray)));
        connect(_view, SIGNAL(modedElement(quint16,quint16,QByteArray)), _socket, SLOT(modElement(quint16,quint16,QByteArray)));
        connect(_socket, SIGNAL(modedElement(quint16,quint16,QByteArray)), _view, SLOT(slotModedElement(quint16,quint16,QByteArray)));
        connect(_view, SIGNAL(deletedElement(quint16)), _socket, SLOT(delElement(quint16)));
        connect(_socket, SIGNAL(deletedElement(quint16)), _view, SLOT(slotDeletedElement(quint16)));

        //connexion du _socket disconnect au quit de l'app
        connect(_socket, SIGNAL(disconnected()), this, SLOT(slotInfoQuit()));

        
        if( _socket->state() != QAbstractSocket::ConnectedState)
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("Erreur de connexion"));
            msgBox.setText(tr("Connexion impossible"));
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();            

            exit(0);

        }
        
    }
    else
    {
        exit(0);
    }

}


MainWindow::~MainWindow(){}

void MainWindow::connexion()
{
    _socket->connectToHost(_dialogConnexion->getHost(), _dialogConnexion->getPort());
    _socket->connect(_dialogConnexion->getName());

    _mListeMain->setNom(_dialogConnexion->getName());
}

/* ---------------------------------------- */
/* ---------------------------------------- */
/*              Slots                       */
/* ---------------------------------------- */
/* ---------------------------------------- */

void MainWindow::slotAboutToQuit()
{
    _socket->disconnect();
}

//toolbar principal (figure)
void MainWindow::slotAfficheToolbarFigure()
{
    _scene->clearSelection();

    if(_actionText->isChecked()) 
    {
        _actionText->setChecked(false);
    }//enlever la selection text pour passer a selection figure

    if(_actionSelection->isChecked())
    {
        _actionSelection->setChecked(false);
    }//idem pour selection

    if(_actionGomme->isChecked())
    {
         _actionGomme->setChecked(false);
    }//idem pour gome

    if(_actionDrag->isChecked())
    {
         _actionDrag->setChecked(false);
    }//idem pour drag

    if(_toolbarfigure->isHidden())
    {
        _toolbarfigure->setHidden(false);

        //signals
        slotFigureTriangle();//si hiden au moment de l'ouvrir on l'initialise a triangle
    }

    _actionfigure->setChecked(true);
}

void MainWindow::slotSelectionnerTexte()
{
    _scene->clearSelection();

    if(_actionfigure->isChecked())
    {
        _actionfigure->setChecked(false);
        _toolbarfigure->setHidden(true); 
    }//enlever la selection figure pour passer a selection texte

    if(_actionSelection->isChecked())
    {
        _actionSelection->setChecked(false);
    }//idem pour selection

    if(_actionGomme->isChecked())
    {
        _actionGomme->setChecked(false);
    }//idem pour gome

    if(_actionDrag->isChecked())
    {
        _actionDrag->setChecked(false);
    }//idem pour drag

    _actionText->setChecked(true);

    //signals
    _view->setTextActivated();
}

void MainWindow::slotSelectionner()
{
    if(_actionText->isChecked())
    {
        _actionText->setChecked(false);
    }//enlever la selection text pour passer a selection figure

    if(_actionfigure->isChecked())
    {
        _actionfigure->setChecked(false);
        _toolbarfigure->setHidden(true);
    }//enlever la selection figure pour passer a selection texte

    if(_actionGomme->isChecked())
    {
        _actionGomme->setChecked(false);
    }//idem pour gome

    if(_actionDrag->isChecked())
    {
        _actionDrag->setChecked(false);
    }//idem pour drag

    _actionSelection->setChecked(true);

    //signals
    _view->setSelectionActivated();
}

void MainWindow::slotGomme()
{
    _scene->clearSelection();

    if(_actionfigure->isChecked())
    {
        _actionfigure->setChecked(false);
        _toolbarfigure->setHidden(true);
    }//enlever la selection figure pour passer a selection texte

    if(_actionSelection->isChecked())
    {
        _actionSelection->setChecked(false);
    }//idem pour selection

    if(_actionText->isChecked())
    {
        _actionText->setChecked(false);
    }//idem pour text

    if(_actionDrag->isChecked())
    {
        _actionDrag->setChecked(false);
    }//idem pour drag

    _actionGomme->setChecked(true);
    //signals
    _view->setDeleteActivated();
}
void MainWindow::slotDrag()
{
    if(_actionfigure->isChecked())
    {
        _actionfigure->setChecked(false);
        _toolbarfigure->setHidden(true);
    }//enlever la selection figure pour passer a selection texte

    if(_actionSelection->isChecked())
    {
        _actionSelection->setChecked(false);
    }//idem pour selection

    if(_actionText->isChecked())
    {
        _actionText->setChecked(false);
    }//idem pour text

    if(_actionGomme->isChecked())
    {
        _actionGomme->setChecked(false);
    }//idem pour gomme

    _actionDrag->setChecked(true);

    //signals
    _view->setDragActivated();
}


//toolbare figure
void MainWindow::slotFigureCarre()
{
    _actionfigure->setIcon(QIcon("Image/Carre.svg"));
    //pour pas  decheked en cas de reclic

    for(auto * it : _figures)
        it->setChecked(false);

    _figures[tr("carre")]->setChecked(true);

    //on emet le changement
    _view->setCarre();
}

void MainWindow::slotFigureLigne()
{
    _actionfigure->setIcon(QIcon("Image/Ligne.svg"));

    //pour pas dechedcked en cas de reclic
    for(auto * it : _figures)
        it->setChecked(false);

    _figures[tr("ligne")]->setChecked(true);

    //on emet le changement
    _view->setLigne();
}

void MainWindow::slotFigureTriangle()
{
    _actionfigure->setIcon(QIcon("Image/Triangle.svg"));

    //pour pas dechedcked en cas de reclic
    for(auto * it : _figures)
        it->setChecked(false);

    _figures[tr("triangle")]->setChecked(true);

    //on emet le changement
    _view->setTriangle();
}

void MainWindow::slotFigureRectangle()
{
    _actionfigure->setIcon(QIcon("Image/Rectangle.svg"));

    //pour pas dechedcked en cas de reclic
     for(auto * it : _figures)
        it->setChecked(false);

    _figures[tr("rectangle")]->setChecked(true);

    //on emet le changement
    _view->setRectangle();
}

void MainWindow::slotFigurePolygone()
{
    _actionfigure->setIcon(QIcon("Image/Polygone.svg"));

    //pour pas dechedcked en cas de reclic
    for(auto * it : _figures)
        it->setChecked(false);

    _figures[tr("polygone")]->setChecked(true);

    //on emet le changement
    _view->setPolygone();

    if(peutDessiner)
        emit ouvrirNbPoint();
}

void MainWindow::slotFigureEllipse()
{
    _actionfigure->setIcon(QIcon("Image/Ellipse.svg"));

    //pour pas dechedcked en cas de reclic
    for(auto * it : _figures)
        it->setChecked(false);

    _figures[tr("ellipse")]->setChecked(true);

    //on emet le changement
    _view->setEllipse();
}

void MainWindow::slotFigureCercle()
{
    _actionfigure->setIcon(QIcon("Image/Cercle.svg"));

    //pour pas dechedcked en cas de reclic
    for(auto * it : _figures)
        it->setChecked(false);

    _figures[tr("cercle")]->setChecked(true);

    _view->setCercle();
    //on emet le changement

}

void MainWindow::slotOuvrirLineEdit(int x, int y)
{
    _lineEdit->move(this->x() + x - _view->horizontalScrollBar()->value(),this->y() + y - _view->verticalScrollBar()->value());
    _lineEdit->setText("");
    _lineEdit->setWindowFlags(Qt::Popup);
    _lineEdit->show();
}

void MainWindow::slotEnvoyerText()
{
    emit envoyerLineEdit(_lineEdit->text());
}

void MainWindow::slotOuvrirNbPoint()
{
    _dNbPoint->exec();
}

//slot pour save et load
void MainWindow::slotSave()
{
    _scene->clearSelection();

    QString path = QFileDialog::getSaveFileName(_dExplorer,tr("Enregistrer"),QString(), "Extension (*svg)");

    QString s(".svg");
    if(!path.endsWith(s,Qt::CaseSensitive)){
        path.append(s);
    }

    if(_scene->items().size()>0)
        SVG::writeSVG(_scene,path);
}


void MainWindow::slotLoad()
{
    QString path = QFileDialog::getOpenFileName(_dExplorer,tr("Charger"),QString(), "Extension (*svg)");

    if(! path.isEmpty())
    {
        QList<QGraphicsItem *> list= SVG::readFromSVG(path);
        _view->clear();
        _socket->clear();
        _view->setListItem(list);

        //qDebug() << "Taille de la liste renvoyé par le chargement : " << list.size();

        for ( auto it = list.begin(); it != list.end(); ++it)
        {
            QGraphicsItem * item = *it;

           // qDebug() << "Ajout de l'item " << item;

            _scene->addItem(item);

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

                _socket->addElement(RECTANGLE, data);
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

                _socket->addElement(ELLIPSE, data);
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

                _socket->addElement(POLYGONE, data);;

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

                _socket->addElement(LIGNE, data);
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

                _socket->addElement(TEXTE, data);
            }
        }
        //qDebug() << "Fin du chargement Scene = " << _scene->items();
    }
}

void MainWindow::slotPreLoad()
{
    if(peutDessiner)
    {
        _scene->clearSelection();

        QMessageBox msgBox;
        msgBox.setText(tr("Vous allez charger un  nouveau document"));
        msgBox.setInformativeText("Voulez vous sauvegarder");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();

        switch (ret)
        {
            case QMessageBox::Save:
                // Save was clicked
                slotSave();
                slotLoad();
                break;

            case QMessageBox::Discard:
                // Don't Save was clicked
                slotLoad();
                break;

            case QMessageBox::Cancel:
                // Cancel was clicked
                break;

            default:
                // should never be reached
                break;
        }
    }
}

void MainWindow::slotNew()
{
    if(peutDessiner)
    {
        _scene->clearSelection();

        QMessageBox msgBox;
        msgBox.setText(tr("Vous allez crée un  nouveau document"));
        msgBox.setInformativeText(tr("Voulez vous sauvegarder"));
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();

        switch (ret)
        {
            case QMessageBox::Save:
                // Save was clicked
                slotSave();
                _view->clear();
                _socket->clear();
                break;

            case QMessageBox::Discard:
                // Don't Save was clicked
                _view->clear();
                _socket->clear();
                break;

            case QMessageBox::Cancel:
                // Cancel was clicked
                break;

            default:
                // should never be reached
                break;
        }
    }
}

void MainWindow::slotPeutDessiner(bool b)
{
    peutDessiner = b;
}

void MainWindow::slotInfoQuit()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Erreur de connexion"));
    msgBox.setText(tr("Vous avez été déconnecté"));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();

    QCoreApplication::instance()->quit();
}
