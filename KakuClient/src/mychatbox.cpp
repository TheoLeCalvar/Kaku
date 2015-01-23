#include "mychatbox.h"

MyChatBox::MyChatBox()
{
    _bEnvoi = new QPushButton(tr("Envoyer"));
    _lTexte = new QLineEdit();
    _tChat = new QTextEdit();

    _tChat->setReadOnly(true); // La ChatBox ne peut être éditée

    _vLayoutPrincipal = new QVBoxLayout(this);
    _hLayoutEnvoi = new QHBoxLayout();

    _hLayoutEnvoi->addWidget(_lTexte);
    _hLayoutEnvoi->addWidget(_bEnvoi);
    _vLayoutPrincipal->addWidget(_tChat);
    _vLayoutPrincipal->addLayout(_hLayoutEnvoi);

    connect(_bEnvoi, SIGNAL(clicked()), this, SLOT(envoi()));
    connect(_lTexte, SIGNAL(returnPressed()), this, SLOT(envoi()));
}

MyChatBox::~MyChatBox(){}

void MyChatBox::envoi()
{
    if(_lTexte->text() != "")
    {
        _tChat->append(tr("Moi : ") + _lTexte->text());
        emit envoiMessage(_lTexte->text());
        _lTexte->clear();
    }
}

void MyChatBox::slotClientConnecte(QString name, bool admin)
{
    _tChat->append(name + tr(" s'est connecté"));
}

void MyChatBox::slotClientDeconnecte(QString name)
{
    _tChat->append(name + tr(" s'est déconnecté"));
}

void MyChatBox::slotMessageRecu(QString message)
{
    _tChat->append(message);
}
