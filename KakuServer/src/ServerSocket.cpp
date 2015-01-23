#include "ServerSocket.h"

ServerSocket::ServerSocket(qintptr socketDescriptor, QObject * parent):
QTcpSocket(parent), _name("anonymous"), _blockSize(0)
{
    if(setSocketDescriptor(socketDescriptor))
        qDebug() << "Connexion établie";
    else
        qDebug() << "Erreur lors de la connexion";

    QObject::connect(this, SIGNAL(readyRead()), this, SLOT(read()));
}

ServerSocket::~ServerSocket()
{
}

const QString & ServerSocket::getName() const
{
    return _name;
}

bool ServerSocket::isAdmin() const
{
    return _isAdmin;
}

void ServerSocket::setAdmin(bool b)
{
    _isAdmin = b;
}

void ServerSocket::sendMessage(const QString & message)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);

    out << (quint16)0 << QString("MESSAGE") << message;
    out.device()->seek(0);

    out << (quint16)(block.size() - sizeof(quint16));

    write(block);
}

void ServerSocket::controlList(const QStringList & list)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);

    out << (quint16)0 << QString("CONTROLLIST") << list;
    out.device()->seek(0);

    out << (quint16)(block.size() - sizeof(quint16));

    write(block);   
}

void ServerSocket::addedElement(quint16 type, const QByteArray & args)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);

    out << (quint16)0 << QString("ADDELEMENT") << type << args;
    out.device()->seek(0);

    out << (quint16)(block.size() - sizeof(quint16));

    write(block);
}

void ServerSocket::deletedElement(quint16 id)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);

    out << (quint16)0 << QString("DELELEMENT") << id;
    out.device()->seek(0);

    out << (quint16)(block.size() - sizeof(quint16));

    write(block);  
}

void ServerSocket::movedElement(quint16 id, qreal dx, qreal dy)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);

    out << (quint16)0 << QString("MOVELEMENT") << id << dx << dy;
    out.device()->seek(0);

    out << (quint16)(block.size() - sizeof(quint16));

    write(block);  
}

void ServerSocket::modedElement(quint16 id, quint16 type, const QByteArray & args)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);

    out << (quint16)0 << QString("MODELEMENT") << id<< type << args;
    out.device()->seek(0);

    out << (quint16)(block.size() - sizeof(quint16));

    write(block);  
}

void ServerSocket::kicked(const QString & name)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);

    out << (quint16)0 << QString("KICK") << name;
    out.device()->seek(0);

    out << (quint16)(block.size() - sizeof(quint16));

    write(block);  
}

void ServerSocket::cleared()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);

    out << (quint16)0 << QString("CLEAR");
    out.device()->seek(0);

    out << (quint16)(block.size() - sizeof(quint16));

    write(block); 
}

void ServerSocket::clientConnected(const QString & name, bool admin)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);

    out << (quint16)0 << QString("NEWCLIENT") << name << admin;
    out.device()->seek(0);

    out << (quint16)(block.size() - sizeof(quint16));

    write(block); 
}

void ServerSocket::clientDisconnected(const QString & name)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);

    out << (quint16)0 << QString("REMOVECLIENT") << name;
    out.device()->seek(0);

    out << (quint16)(block.size() - sizeof(quint16));

    write(block); 
}

void ServerSocket::error(const QString & error)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);

    out << (quint16)0 << QString("ERROR") << error;
    out.device()->seek(0);

    out << (quint16)(block.size() - sizeof(quint16));

    write(block); 
}

void ServerSocket::write(const QByteArray & packet)
{
    if(isValid())
    {
        qDebug() << "Write to " << _name;

        qDebug() << QTcpSocket::write(packet) << "bytes written";
    }
    else
    {
        qDebug() << "Socket invalide";
    }
}

void ServerSocket::read()
{
    if(!isValid())
    {
        qDebug() << "Socket invalide";

        return;
    }

    QString start;
    QDataStream in(this);
    in.setVersion(QDataStream::Qt_5_3);

    if(_blockSize == 0)
    {
        if(bytesAvailable() < (int)sizeof(quint16))
            return;

        in >> _blockSize;
    }


    if(bytesAvailable() < _blockSize)
        return;

    _blockSize = 0;

    qDebug() << "Read from" << _name;


    in >> start;


    if(start == "CONNECT")
    {
        in >> _name;
        emit connect(_name);
    }
    else if(start == "DISCONNECT")
    {
        emit disconnect();
    }
    else if(start == "ADMIN")
    {
        QString corps;
        in >> corps;
        emit admin(corps);
    }
    else if(start == "ASKCONTROL")
    {
        emit askForControl();
    }
    else if(start == "REMOVECONTROL")
    {
        emit removeControl();
    }
    else if(start == "RELEASECONTROL")
    {
        emit releaseControl();
    }
    else if(start == "GIVECONTROL")
    {
        QString corps;
        in >> corps;
        emit changeControlOwner(corps);
    }
    else if(start == "MESSAGE")
    {
        QString corps;
        in >> corps;
        emit receiveMessage(corps);
    }
    else if(start == "ADDELEMENT")
    {
        quint16 type;
        QByteArray args;
        in >> type >> args;

        emit addElement(type, args);
    }
    else if(start == "DELELEMENT")
    {
        quint16 id;

        in >> id;

        emit delElement(id);
    }
    else if(start == "MOVELEMENT")
    {   
        quint16 id;
        qreal dx, dy;
        in >> id >> dx >> dy;

        emit movElement(id, dx, dy);
    }
    else if(start == "MODELEMENT")
    {
        quint16 id,type;
        QByteArray args;

        in >> id >> type >> args;

        emit modElement(id, type, args);
    }
    else if(start == "KICK")
    {
        QString corps;
        in >> corps;
        emit kick(corps);
    }
    else if(start == "CLEAR")
    {
        emit clear();
    }
    else 
    {
        qWarning() << "Error parsing data";
    }   

    if(bytesAvailable() > 0)
    {
        qDebug() << "Il reste " << bytesAvailable() << " bytes, on retourne";
        read();
    }
}