#ifndef SERVER_H
#define SERVER_H 

#include <QCoreApplication>

#include <QString>
#include <QDataStream>
#include <QTcpServer>
#include <QHostAddress>

#include <QGraphicsItem>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QColor>


#include "ServerSocket.h"
#include "Common.h"

//Classe principale du serveur
//Se charge de gérer les connexion et de relayer les informations concernant le dessin
class Server: public QTcpServer
{
Q_OBJECT
private:
	QMap<QString, ServerSocket *>   _clients;
    QList<QGraphicsItem * >         _scene;
    QList<ServerSocket *>           _controlList;
    QList<ServerSocket *>           _unknownClients;
    QString                         _password;

public:
	Server(const QString & password = "azerty");
	~Server();

    //Lance le serveur sur le port donné en paramètre
    void startServer(quint16 port = 4242);

private:
    //Fonctions ajoutant une figure à la scène du serveur
    //Se référer au protocole pour connaître la structure du QDataStream
    void addLine(QDataStream & args);
    void addText(QDataStream & args);
    void addRectangle(QDataStream & args);
    void addPolygon(QDataStream & args);
    void addEllipse(QDataStream & args);

    //Fonctions se chargeant de la modifications locale d'items
    //L'id est la position de l'item à modifier dans la liste des items
    //Se référer au protocole pour la structure du QDataStream
    void modLine(quint16 id, QDataStream & args);
    void modText(quint16 id, QDataStream & args);
    void modRectangle(quint16 id, QDataStream & args);
    void modPolygon(quint16 id, QDataStream & args);
    void modEllipse(quint16 id, QDataStream & args);

    //Envoie de la scène complète à un utilisateur
    //Utilisé lors de la connexion d'un nouveau client
    void sendScene(ServerSocket * client);

public slots:
    //Appelée lors de l'enregistrement d'un client
    void connected(const QString & name);
    //Appelée lors de la déconnexion d'un client
	void disconnected();
    //Appelée lors de l'authentification d'un administrateur (non utilisé)
    void admin(const QString & mdp);

    //Appelée lorsqu'un client se rajoute dans la liste de ceux qui demandent la main
    void askedForControl();
    //Appelée lorsqu'un client se retire de la liste
    void removedControl();
    //Appelée lorsque le client qui dessine se retire de la liste
    void releasedControl();
    //Appelée lorsque la liste de ceux qui demandent la main a été modifiée
    //Envoie la nouvelle liste à tout les utilisateurs connectés
    void controlList(bool unicast = true);
    //Appelée lorsqu'un utilisateur envoie un message dans le chat
    //Broadcast ce message à tout les autres utilisateurs
    void receivedMessage(const QString & message);

    //Appelée quand le dessinateur ajoute un item
    //Se référer au protocole pour décoder le byteArray
    void addedElement(quint16 type, const QByteArray & args);
    //Appelée quand le dessinateur supprime un élément
    void deletedElement(quint16 id);
    //Appelé lors du déplacement d'un item
    //NB : non utilisé, préférer modedElement
    void movedElement(quint16 id, qreal dx, qreal dy);
    //Appelée lors de la modification d'un item (position, scale, couleur, …)
    void modedElement(quint16 id, quint16 type, const QByteArray & args);

    //appelée lorsque le dessinateur nettoie le dessin
    void cleared();

    //Appelée lorsqu'un admin force le changement de main
    //NB : non utilisé
    void changedControlOwner(const QString & name);

    //Appelée lorsqu'un admin kick un utilisateur
    //NB : non utilisé
    void kickedUser(const QString & name);

protected: 
    void incomingConnection(qintptr socketDescriptor);


};

#endif