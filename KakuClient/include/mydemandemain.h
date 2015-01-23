#ifndef MYDEMANDEMAIN_H
#define MYDEMANDEMAIN_H

#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QList>
#include <QDebug>

class MyDemandeMain: public QWidget
{
Q_OBJECT

private:

    QList<QString>  _lUtilisateursDessin;
    QList<QString>  _lUtilisateurs;
    QString         _nom;

    QPushButton *   _bDemande;
    QPushButton *   _bQuitte;
    QTextEdit *     _tListeMain;
    QHBoxLayout *   _hLayoutBouton;
    QVBoxLayout *   _vLayoutPrincipal;

public:

    MyDemandeMain();
    ~MyDemandeMain();

private slots:

    void demande();
    void quitte();

    void slotCConnecte(QString name, bool admin);
    void slotCDeconnecte(QString name);
    void slotListeMain(QStringList liste);

public slots:

    void setNom(const QString& n);

signals:

    void DemandeControl();
    void QuitterControl();
    void QuitterDessiner();

    void PeutDessiner(bool dessin);
};

#endif // MYDEMANDEMAIN_H
