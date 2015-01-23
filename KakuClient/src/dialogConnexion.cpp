#include "dialogConnexion.h"

DialogConnexion::DialogConnexion(QWidget *parent):QDialog(parent)
{
    _layoutV = new QVBoxLayout(this);
    _layoutHLabel = new QHBoxLayout;
    _layoutHButton = new QHBoxLayout;

    _buttonAccept = new QPushButton("Connexion");
    _buttonRejected = new QPushButton("Annuler");

    _labelHost = new QLabel("Host");
    _labelPort = new QLabel("Port");
    _labelName = new QLabel("Pseudonyme");

    _editHost = new QLineEdit("localhost");
    _editPort = new QLineEdit("4242");
    _editName = new QLineEdit("Drawer");

    //Ajout au layout

    _layoutHLabel->addWidget(_labelHost);
    _layoutHLabel->addWidget(_editHost);
    _layoutHLabel->addSpacing(10);
    _layoutHLabel->addWidget(_labelPort);
    _layoutHLabel->addWidget(_editPort);
    _layoutHLabel->addSpacing(10);
    _layoutHLabel->addWidget(_labelName);
    _layoutHLabel->addWidget(_editName);

    _layoutHButton->addStretch(5);
    _layoutHButton->addWidget(_buttonAccept);
    _layoutHButton->addWidget(_buttonRejected);


    _layoutV->addLayout(_layoutHLabel);
    _layoutV->addSpacing(10);
    _layoutV->addLayout(_layoutHButton);

    //connect
    connect(_buttonAccept, SIGNAL(clicked()), this, SLOT(accept()));
    connect(_buttonRejected, SIGNAL(clicked()), this, SLOT(reject()));

}

QString DialogConnexion::getHost()
{
    return _editHost->text();
}

int  DialogConnexion::getPort()
{
    return _editPort->text().toInt(NULL, 10);
}

QString DialogConnexion::getName()
{
    return _editName->text();
}

