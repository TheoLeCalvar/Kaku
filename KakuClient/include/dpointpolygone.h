#ifndef DPOINTPOLYGONE_H
#define DPOINTPOLYGONE_H

#include <QtCore>
#include <QtGui>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDialog>

class DPointPolygone : public QDialog
{
Q_OBJECT

private:
    
    QSpinBox *      _sNbPoint;
    QPushButton *   _bOk;
    QPushButton *   _bAnnuler;

    QVBoxLayout *   _vLayoutPrincipal;
    QHBoxLayout *   _hLayoutPrincipal;

public:

    DPointPolygone(QWidget *parent = 0);

signals:

    void sannuler();
    void sok(int);

private slots:

    void slotAnnuler();
    void slotOk();
};

#endif // DPOINTPOLYGONE_H
