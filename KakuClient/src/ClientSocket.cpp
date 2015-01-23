#include "ClientSocket.h"

ClientSocket::ClientSocket(QObject * parent):
QTcpSocket(parent), _blockSize(0)
{
}

ClientSocket::~ClientSocket()
{
}

void ClientSocket::connectToHost(const QString & hostname, int port)
{
	QTcpSocket::connectToHost(hostname, port);
	qDebug() << "Connexion en cours";
	if(waitForConnected())
        qDebug() << "Connexion Ok";
	else
    {
        //qDebug() << "Connexion impossible";
        
        emit kicked();

        //qDebug() << "apres kicked";
    }
    QObject::connect(this, SIGNAL(readyRead()), this, SLOT(read()));
}

void ClientSocket::connect(const QString & name)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);

    out << (quint16)0 << QString("CONNECT") << name;
    out.device()->seek(0);

    out << (quint16)(block.size() - sizeof(quint16));

    write(block);
}

void ClientSocket::disconnect()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);

    out << (quint16)0 << QString("DISCONNECT");
    out.device()->seek(0);

    out << (quint16)(block.size() - sizeof(quint16));

    write(block);
}

void ClientSocket::askForControl()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);

    out << (quint16)0 << QString("ASKCONTROL");
    out.device()->seek(0);

    out << (quint16)(block.size() - sizeof(quint16));

    write(block);
}

void ClientSocket::removeControl()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);

    out << (quint16)0 << QString("REMOVECONTROL");
    out.device()->seek(0);

    out << (quint16)(block.size() - sizeof(quint16));

    write(block);
}

void ClientSocket::releaseControl()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);

    out << (quint16)0 << QString("RELEASECONTROL");
    out.device()->seek(0);

    out << (quint16)(block.size() - sizeof(quint16));

    write(block);
}

void ClientSocket::sendMessage(const QString & send)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);

    out << (quint16)0 << QString("MESSAGE") << send;
    out.device()->seek(0);

    out << (quint16)(block.size() - sizeof(quint16));

    write(block);
}

void ClientSocket::addElement(quint16 type, const QByteArray & args)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);

    out << (quint16)0 << QString("ADDELEMENT") << type << args;
    out.device()->seek(0);

    out << (quint16)(block.size() - sizeof(quint16));

    write(block);
}

void ClientSocket::delElement(quint16 id)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);

    out << (quint16)0 << QString("DELELEMENT") << id;
    out.device()->seek(0);

    out << (quint16)(block.size() - sizeof(quint16));

    write(block); 
}

void ClientSocket::movElement(quint16 id, qreal dx, qreal dy)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);

    out << (quint16)0 << QString("MOVELEMENT") << id << dx << dy;
    out.device()->seek(0);

    out << (quint16)(block.size() - sizeof(quint16));

    write(block);
}

void ClientSocket::modElement(quint16 id, quint16 type, const QByteArray & args)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);

    out << (quint16)0 << QString("MODELEMENT") << id << type << args;
    out.device()->seek(0);

    out << (quint16)(block.size() - sizeof(quint16));

    write(block); 
}

void ClientSocket::clear()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);

    out << (quint16)0 << QString("CLEAR");
    out.device()->seek(0);

    out << (quint16)(block.size() - sizeof(quint16));

    write(block); 
}

void ClientSocket::sendError(const QString & error)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);

    out << (quint16)0 << QString("ERROR") << error;
    out.device()->seek(0);

    out << (quint16)(block.size() - sizeof(quint16));

    write(block); 
}

void ClientSocket::changeControlOwner(const QString & name)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);

    out << (quint16)0 << QString("GIVECONTROL") << name;
    out.device()->seek(0);

    out << (quint16)(block.size() - sizeof(quint16));

    write(block); 
}

void ClientSocket::kick(const QString & name)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);

    out << (quint16)0 << QString("KICK") << name;
    out.device()->seek(0);

    out << (quint16)(block.size() - sizeof(quint16));

    write(block);
}

void ClientSocket::write(const QByteArray & packet)
{
    if(isValid())
    {
        qDebug() << QTcpSocket::write(packet) << " bytes written";
    }
    else
    {
        qDebug() << "Socket invalide";
    }
}

void ClientSocket::read()
{
    if(!isValid())
    {
        qDebug() << "Socket invalide";
        return;
    }

    QString start;
    QDataStream in(this);
    in.setVersion(QDataStream::Qt_5_0);

    if(_blockSize == 0)
    {
        if(bytesAvailable() < (int)sizeof(quint16))
            return;

        in >> _blockSize;
    }


    if(bytesAvailable() < _blockSize)
        return;

    _blockSize = 0;

    // qDebug() << "Read message : " << in;


    in >> start;

	if(start == "MESSAGE")
	{
		QString corps;
        in >> corps;
        emit receivedMessage(corps);
	}
	else if(start == "CONTROLLIST")
	{
		QStringList liste;
		in >> liste;
		emit controlList(liste);
	}
	else if(start == "ADDELEMENT")
    {
        quint16 type;
        QByteArray args;
        in >> type >> args;

        emit addedElement(type, args);
    }
    else if(start == "DELELEMENT")
    {
        quint16 id;
        in >> id;

        emit deletedElement(id);
    }
    else if(start == "MOVELEMENT")
    {   
        quint16 id;
        qreal dx, dy;
        in >> id >> dx >> dy;

        emit movedElement(id, dx, dy);
    }
    else if(start == "MODELEMENT")
    {
        quint16 id, type;
        QByteArray args;

        in >> id  >> type >> args;

        emit modedElement(id, type, args);
    }
	else if(start == "KICK")
	{
		emit kicked();
	}
	else if(start == "CLEAR")
	{
		emit cleared();
	}
	else if(start == "NEWCLIENT")
	{
		QString corps;
		bool admin;
        in >> corps >> admin;
		emit clientConnected(corps, admin);
	}
	else if(start == "REMOVECLIENT")
	{
		QString corps;
        in >> corps;
		emit clientDisconnected(corps);
	}
	else if(start == "ERROR")
	{
		QString corps;
        in >> corps;
		emit error(corps);
	}
	else 
	{
		qWarning() << "Error parsing data";
	}	

    if(bytesAvailable() > 0)
    {
        qDebug() << "Il reste " << bytesAvailable() << " bytes, on y retourne";
        read(); 
    }
}
