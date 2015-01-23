#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include <QTcpSocket>
#include <QByteArray>
#include <QDataStream>
#include <QString>

class ServerSocket: public QTcpSocket
{
Q_OBJECT

private:
	QString _name;
	bool 	_isAdmin;
	quint16 _blockSize;

public:
	ServerSocket(qintptr socketDescriptor, QObject * parent = nullptr);
	~ServerSocket();

	const QString & getName() const;
	bool 			isAdmin() const;
	void 			setAdmin(bool b);


signals:
	//Emit lors de l'enregistrement du client
	//Permet de s'enregistrer auprès du serveur avec le nom
	void connect(const QString & name);
	//Emit lors de la déconnexion du client
	//Se déconnecte du serveur qui stopera la connexion
	void disconnect();
	//Emit lors d'une demande d'authentification du client
	void admin(const QString & mdp);

	//Emit lorsque le client s'ajoute aux demandeurs de main
	//Ajoute le client dans la liste s'il n'y est pas déjà et s'il est enregistré
	void askForControl();
	//Emit lors du retrait des demandeurs
	//Le retire s'il est dans la liste
	void removeControl();
	//Emit lors que le client qui dessine laisse la main
	//Le retire s'il est le premier de la liste
	void releaseControl();
	//Emit lors que le client force le changement de main
	//NB : non utilisé
	void changeControlOwner(const QString & name);
	//Emit lorsque le client envoie un message
	//Le serveur renvoie ce message à tout les autres utilisateurs
	void receiveMessage(const QString & message);

	//Emit lorsque le client kick un utilisateur
	//Déconnecte le client portant le nom name
	void kick(const QString & name);

	//Emit lorsque le client ajoute un élément, se référer au protocole pour les args
	//Ajoute l'élément côté serveur et retransmet le message aux autres 
	void addElement(quint16 type, const QByteArray & args);
	//Emit lorsque le client supprime un élément
	//Supprime l'élément côté client et retransmet l'information aux autres
	void delElement(quint16 id);
	//Emit lorsque le client déplace un élément
	//Déplace l'élément sur le serveur et retransmet l'information
	//NB: utiliser modElement
	void movElement(quint16 id, qreal dx, qreal dy);
	//Emit lorsque le client modifie un élément, se référer au protocole pour les args
	//Modifie l'élément et retransmet l'information
	void modElement(quint16 id, quint16 type, const QByteArray & args);

	//Emit lorsque le client nettoie le dessin
	//Vide le dessin côté serveur et retransmet l'information
	void clear();

public slots:
	//Appelée lorsqu'un client a envoyé un message
	//Envoie le message à l'utilisateur
	void sendMessage(const QString & message);

	//Appelée à chaque modification de la liste de controle
	//Envoie la nouvelle liste à l'utilsiateur
	void controlList(const QStringList & list);

	//Appelée lorsqu'un élément a été ajouté par le dessinateur ou lors de l'envoie initial du dessin
	void addedElement(quint16 type, const QByteArray & args);
	//Appelée lors de la suppression d'un élément par le dessinateur
	void deletedElement(quint16 id);
	//Appelée lors du déplacement d'un élément par le dessinateur
	//NB : ne pas utiliser
	void movedElement(quint16 id, qreal dx, qreal dy);
	//appelée lors de la modification d'un élément par le dessinteur
	void modedElement(quint16 id, quint16 type, const QByteArray & args);

	//Appelée pour indiqué qu'un utilisateur a été kické par un admin
	//NB : non utilisé
	void kicked(const QString & name);

	//Appelée lorsque le dessin a été nettoyé par le dessinateur
	void cleared();

	//Appelée lors de la connexion d'un client
	void clientConnected(const QString & name, bool admin);
	//Appelée lors de la déconnexion d'un client
	void clientDisconnected(const QString & name);

	//Appelé lorsque le serveur renvoie une erreur au client
	//Nb : non utilisé
	void error(const QString & error);

private slots:
	void write(const QByteArray & packet);
	void read();
};

#endif