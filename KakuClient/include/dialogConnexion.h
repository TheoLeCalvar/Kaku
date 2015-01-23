#ifndef DIALOGCONNEXION_H
#define DIALOGCONNEXION_H

#include <QDialog>
#include <QLabel>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>

class DialogConnexion : public QDialog
{

private:

    QLabel *        _labelHost;
    QLabel *        _labelPort;
    QLabel *        _labelName;

    QLineEdit *     _editHost;
    QLineEdit *     _editPort;
    QLineEdit *     _editName;

    QPushButton *   _buttonAccept;
    QPushButton *   _buttonRejected;

    QHBoxLayout *   _layoutHLabel;
    QHBoxLayout *   _layoutHButton;
    QVBoxLayout *   _layoutV;

public:

    DialogConnexion(QWidget *parent = 0);

    QString getHost();
    QString getName();

    int getPort();



};

#endif // DIALOGCONNEXION_H
