#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QTcpSocket>
#include <QString>
#include <QStringList>
#include <QByteArray>
#include <QDataStream>
#include <QCoreApplication>


class ClientSocket: public QTcpSocket
{
Q_OBJECT

private:
	quint16 	_blockSize;

public:
	ClientSocket(QObject * parent = nullptr);
	~ClientSocket();

public slots:
	//Initie la connexion au serveur
	//TODO : gérer les erreurs de connexion
	void connectToHost(const QString & hostname = "localhost", int port = 4242);

	//S'idientifie sur le serveur avec le nom passé en paramètre
	void connect(const QString & name);
	//Se déconnecte du serveur
	void disconnect();

	//S'ajoute dans la liste des clients qui demandent la main
	void askForControl();
	//SE retire de la liste des clients qui demandent la main
	void removeControl();
	//Relache la main si on est la personne qui l'a
	void releaseControl();
	//Force le changement de personne qui dessinen nécessite d'être admin
	void changeControlOwner(const QString & name);

	//Force la déconnexion de l'utilisateur
	void kick(const QString & name);
	
	//Envoie le message aux autres utilisateurs
	void sendMessage(const QString & message);

	//Ajoute un élément nouvellement dessiné, effectif seulement si on a la main
	void addElement(quint16 type, const QByteArray & args);
	//Supprime un élément déjà dessiné
	void delElement(quint16 id);
	//Déplace un élément déjà dessiné
	void movElement(quint16 id, qreal dx, qreal dy);
	//Modifie un élément déjà dessiné
	void modElement(quint16 id, quint16 type, const QByteArray & args);

	//Vide la scène de dessin
	void clear();

	//Envoie une erreur, pas vraiment gérer au niveau serveur, à revoir plus tard
	void sendError(const QString & error);


signals:
	//Emis quand un autre client envoie un message
	void receivedMessage(const QString & message);

	//Emis quand la liste des utilisateurs demandant la main est mise à jour
	void controlList(const QStringList & liste);

	//Emis lorsqu'un élément a été ajouté par celui qui controle
	void addedElement(quint16 type, const QByteArray & args);
	//Emis lorsqu'un élément a été supprimé
	void deletedElement(quint16 id);
	//Emis lorsqu'un élément a été déplacé
	void movedElement(quint16 id, qreal dx, qreal dy);
	//Emis lorsqu'un élément a été modifié
	void modedElement(quint16 id, quint16 type, const QByteArray & args);

	//Emis lorsque que le client s'est fait kick (en fait il est peut être pas émis, faut voir, j'ai un doute là)
	void kicked();

	//Emis lorsque la scène vient d'être nettoyée
	void cleared();
	
	//Emis lorsqu'un utilisateur s'est enregistré sur le serveur
	void clientConnected(const QString & name, bool admin);
	//Emis lorsqu'un utilsiateur s'est déconnecté
	void clientDisconnected(const QString & name);

	//Emis lorsque le serveur n'a pu traiter une demande (pas emis pour l'instant, à faire plus tard)
	void error(const QString & error);

private slots:
	void write(const QByteArray & packet);
	void read();
};

#endif