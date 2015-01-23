#include "ServerWindow.h"
#include <QApplication>
#include <QDebug>

ServerWindow::ServerWindow()
{
    QStringList args = QApplication::arguments();
    quint16 port = 4242;
    QString password = "azerty";

    for(auto arg = args.begin(); arg != args.end(); ++arg)
    {
        if(*arg == "-p" || *arg == "--port")
        {
            bool ok;
            ++arg;
            port = (quint16)arg->toInt(&ok);

            if(!ok)
            {
                qDebug() << "Erreur dans les paramètres, après le port on veut un nombre";
                QApplication::exit();
            }
        }

        else if(*arg == "-q" || *arg == "--password")
        {
            ++arg;
            password = *arg;
        }
    }

    _server = new Server(password);
    _server->startServer(port);

    QVBoxLayout * layout = new QVBoxLayout(this);
    QLabel * message = new QLabel(QString("Serveur en écoute sur le port %1").arg(port), this);

    layout->addWidget(message);
}

ServerWindow::~ServerWindow()
{

}

