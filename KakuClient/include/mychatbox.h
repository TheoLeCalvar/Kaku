#ifndef MYCHATBOX_H
#define MYCHATBOX_H

#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>

class MyChatBox: public QWidget
{
Q_OBJECT

private:

    QPushButton *   _bEnvoi;
    QLineEdit *     _lTexte;
    QTextEdit *     _tChat;

    QVBoxLayout *   _vLayoutPrincipal;
    QHBoxLayout *   _hLayoutEnvoi;

public:

    MyChatBox();
    ~MyChatBox();

signals:
    void envoiMessage(const QString & message);

private slots:

    void envoi();

    void slotClientConnecte(QString name, bool admin);
    void slotClientDeconnecte(QString name);
    void slotMessageRecu(QString message);
};

#endif // MYCHATBOX_H
