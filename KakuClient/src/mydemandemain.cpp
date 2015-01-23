#include "mydemandemain.h"

MyDemandeMain::MyDemandeMain()
{
    _bDemande = new QPushButton(tr("Demander la main"));
    _bQuitte = new QPushButton(tr("Quitter la liste"));
    _tListeMain = new QTextEdit();
    _vLayoutPrincipal = new QVBoxLayout(this);
    _hLayoutBouton = new QHBoxLayout();

    _tListeMain->setReadOnly(true);

    _hLayoutBouton->addWidget(_bDemande);
    _hLayoutBouton->addWidget(_bQuitte);
    _vLayoutPrincipal->addWidget(_tListeMain);
    _vLayoutPrincipal->addLayout(_hLayoutBouton);

    connect(_bDemande, SIGNAL(clicked()), this, SLOT(demande()));
    connect(_bQuitte, SIGNAL(clicked()), this, SLOT(quitte()));
}

MyDemandeMain::~MyDemandeMain(){}

void MyDemandeMain::setNom(const QString& n)
{
    _nom = n;
    _lUtilisateurs.push_back(_nom);
}

void MyDemandeMain::demande()
{
    emit DemandeControl();
}

void MyDemandeMain::quitte()
{
    if(!_lUtilisateursDessin.isEmpty())
    {
        if(_nom == _lUtilisateursDessin.first())
            emit QuitterDessiner();
        else
            emit QuitterControl();
    }
}

void MyDemandeMain::slotCConnecte(QString name, bool admin)
{
    _lUtilisateurs.push_back(name);
    _tListeMain->append(name);
}

void MyDemandeMain::slotCDeconnecte(QString name)
{
    _lUtilisateurs.removeOne(name);
    _lUtilisateursDessin.removeOne(name);

    _tListeMain->clear();

    for(int i = 0 ; i < _lUtilisateursDessin.size(); i++)
        _tListeMain->append(_lUtilisateursDessin.at(i));

    _tListeMain->append("----------------------");

    for(int i = 0 ; i < _lUtilisateurs.size(); i++)
        _tListeMain->append(_lUtilisateurs.at(i));
}

void MyDemandeMain::slotListeMain(QStringList liste)
{
    if(liste.size() > _lUtilisateursDessin.size())
    {
        _lUtilisateurs.removeOne(liste.last());
    }
    else
    {
        for(auto i = _lUtilisateursDessin.begin(); i != _lUtilisateursDessin.end(); ++i)
        {
            if(!liste.contains(*i))
            {
                _lUtilisateurs.push_back(*i);
                break;
            }
       }
    }

    _lUtilisateursDessin = liste;

    _tListeMain->clear();

    for(int i = 0 ; i < _lUtilisateursDessin.size(); i++)
        _tListeMain->append(_lUtilisateursDessin.at(i));

    _tListeMain->append("----------------------");

    for(int i = 0 ; i < _lUtilisateurs.size(); i++)
        _tListeMain->append(_lUtilisateurs.at(i));

    if(_lUtilisateursDessin.isEmpty())
         emit PeutDessiner(false);
    else
    {
        if(_nom == _lUtilisateursDessin.first())
        {
            _bQuitte->setText(tr("Rendre la main"));
            emit PeutDessiner(true);
        }
        else
        {
            _bQuitte->setText(tr("Quitter la liste"));
            emit PeutDessiner(false);
        }
    }
}
