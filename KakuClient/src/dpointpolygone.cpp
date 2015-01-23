#include "dpointpolygone.h"

DPointPolygone::DPointPolygone(QWidget * parent) : QDialog(parent)
{
    _sNbPoint = new QSpinBox;
    _sNbPoint->setRange(4, 10);
    _sNbPoint->setValue(4);
    _bOk = new QPushButton(tr("Ok"));
    _bAnnuler = new QPushButton(tr("Annuler"));

    _vLayoutPrincipal = new QVBoxLayout(this);
    _hLayoutPrincipal = new QHBoxLayout;

    _hLayoutPrincipal->addWidget(_bOk);
    _hLayoutPrincipal->addWidget(_bAnnuler);


    _vLayoutPrincipal->addWidget(_sNbPoint);
    _vLayoutPrincipal->addLayout(_hLayoutPrincipal);

    // SIGNAUX ET SLOTS

    connect(_bAnnuler,SIGNAL(clicked()),this,SLOT(slotAnnuler()));
    connect(_bOk,SIGNAL(clicked()),this,SLOT(slotOk()));
}

void DPointPolygone::slotAnnuler()
{
    emit sannuler();
}

void DPointPolygone::slotOk()
{
    emit sok(_sNbPoint->value());
}
