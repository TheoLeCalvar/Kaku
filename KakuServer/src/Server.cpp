#include "Server.h"

Server::Server(const QString & password)
:_password(password)
{
    // _scene = new QGraphicsScene(0,0,5000,5000, this);
}

Server::~Server()
{
}

void Server::startServer(quint16 port)
{
    if(listen(QHostAddress::Any, port))
        qDebug() << "Serveur en écoute sur le port " << port;
    else
    {
        qDebug() << "Erreur lors de l'initialisation du serveur";
        QCoreApplication::quit();
    }
}

void Server::addLine(QDataStream & args)
{
    qreal x1, x2, y1, y2, scale;
    QPen pen;

    args >> x1 >> y1 >> x2 >> y2 >> scale >> pen;

    QGraphicsLineItem * item = new QGraphicsLineItem(0, 0, x2 - x1, y2 - y1);
    item->setPos(x1, y1);
    item->setScale(scale);
    item->setPen(pen);

    _scene << item;
}

void Server::addText(QDataStream & args)
{
    qreal x, y, scale;
    QFont font;
    QString text;
    QColor color;

    args >> x >> y >> scale >> text >> font >> color;


    QGraphicsTextItem * item = new QGraphicsTextItem(text);
    item->setX(x);
    item->setY(y);
    item->setScale(scale);
    item->setDefaultTextColor(color);
    item->setFont(font);

    _scene << item;
}

void Server::addRectangle(QDataStream & args)
{
    qreal x, y, w, h, scale;
    QPen pen;
    QBrush brush;

    args >> x >> y >> w >> h >> scale >> pen >> brush;

    QGraphicsRectItem * item = new QGraphicsRectItem(0, 0, w, h);
    item->setPos(x, y);
    item->setScale(scale);
    item->setPen(pen);
    item->setBrush(brush);

    _scene << item;
}

void Server::addPolygon(QDataStream & args)
{
    qreal x, y, scale;
    QPolygon points;
    QPen pen;
    QBrush brush;

    args >> x >> y >> points >> scale >> pen >> brush;

    QGraphicsPolygonItem * item = new QGraphicsPolygonItem(points);
    item->setPos(x, y);
    item->setScale(scale);
    item->setPen(pen);
    item->setBrush(brush);

    _scene << item;
}

void Server::addEllipse(QDataStream & args)
{
    qreal x, y, w, h, scale;
    QPen pen;
    QBrush brush;

    args >> x >> y >> w >> h >> scale >> pen >> brush;

    QGraphicsEllipseItem * item = new QGraphicsEllipseItem(0, 0, w, h);
    item->setPos(x, y);
    item->setScale(scale);
    item->setPen(pen);
    item->setBrush(brush);

    _scene << item;   
}

void Server::modLine(quint16 id, QDataStream & args)
{
    QGraphicsLineItem * item = (QGraphicsLineItem*)_scene.at(id);
    qreal x1, y1, x2, y2, scale;
    QPen pen;

    args >> x1 >> y1 >> x2 >> y2 >> scale >> pen;

    item->setLine(0, 0, x2 - x1, y2 - y1);
    item->setPos(x1, y1);
    item->setScale(scale);
    item->setPen(pen);
}

void Server::modText(quint16 id, QDataStream & args)
{
    QGraphicsTextItem * item = (QGraphicsTextItem*)_scene.at(id);
    qreal x, y, scale;
    QString text;
    QFont font;
    QColor color;

    args >> x >> y >> scale >> text >> font >> color;

    item->setPos(x, y);
    item->setScale(scale);
    item->setDefaultTextColor(color);
    item->setFont(font);
    item->setPlainText(text);
}

void Server::modRectangle(quint16 id, QDataStream & args)
{
    QGraphicsRectItem * item = (QGraphicsRectItem*)_scene.at(id);
    qreal x, y, w, h, scale;
    QPen pen;
    QBrush brush;

    args >> x >> y >> w >> h >> scale >> pen >> brush;

    item->setRect(0, 0, w, h);
    item->setPos(x, y);
    item->setScale(scale);
    item->setPen(pen);
    item->setBrush(brush);
}

void Server::modPolygon(quint16 id, QDataStream & args)
{
    QGraphicsPolygonItem * item = (QGraphicsPolygonItem*)_scene.at(id);
    qreal x, y, scale;
    QPolygon points;
    QPen pen;
    QBrush brush;

    args >> x >> y >> points >> scale >> pen >> brush;

    item->setPolygon(points);
    item->setPos(x, y);
    item->setScale(scale);
    item->setPen(pen);
    item->setBrush(brush);
}

void Server::modEllipse(quint16 id, QDataStream & args)
{
    QGraphicsEllipseItem * item = (QGraphicsEllipseItem*)_scene.at(id);
    qreal x, y, w, h, scale;
    QPen pen;
    QBrush brush;

    args >> x >> y >> w >> h >> scale >> pen >> brush;

    item->setRect(0, 0, w, h);
    item->setPos(x, y);
    item->setScale(scale);
    item->setPen(pen);
    item->setBrush(brush);
}

void Server::sendScene(ServerSocket * client)
{
// * - RectItem : 3
// * - EllipseItem : 4
// * - PolygonItem : 5
// * - LineItem : 6
// * - PixmapItem : 7
// * - TextItem : 8
// * - SimpleTextItem : 9

    qDebug() << "On envoie la scène au nouveau " << _scene.size() << " éléments";

    for(QGraphicsItem * it : _scene)
    {
        QByteArray data;
        QDataStream stream(&data, QIODevice::WriteOnly);
        stream.setVersion(QDataStream::Qt_5_3);

        switch(it->type())
        {
            case 3:
            {
                QGraphicsRectItem * item = (QGraphicsRectItem *)(it);

                stream  << item->x() 
                        << item->y()
                        << item->rect().width()
                        << item->rect().height()
                        << item->scale()
                        << item->pen()
                        << item->brush();

                client->addedElement(RECTANGLE, data);

                break;
            }

            case 4:
            {
                QGraphicsEllipseItem * item = (QGraphicsEllipseItem *)(it);

                stream  << item->x() 
                        << item->y()
                        << item->rect().width()
                        << item->rect().height()
                        << item->scale()
                        << item->pen()
                        << item->brush();

                client->addedElement(ELLIPSE, data);

                break;
            }

            case 5:
            {
                QGraphicsPolygonItem * item = (QGraphicsPolygonItem *)(it);

                stream  << item->x()
                        << item->y()
                        << item->polygon().toPolygon()
                        << item->scale()
                        << item->pen()
                        << item->brush();

                client->addedElement(POLYGONE, data);

                break;
            }

            case 6:
            {
                QGraphicsLineItem * item = (QGraphicsLineItem *)(it);
                QLineF line = item->line();
                stream  << item->x() 
                        << item->y() 
                        << (line.x2() + item->x()) 
                        << (line.y2() + item->y())
                        << item->scale()
                        << item->pen();

                client->addedElement(LIGNE, data);

                break;
            }

            case 8:
            {
                QGraphicsTextItem * item = (QGraphicsTextItem *)(it);
                stream  << item->x() 
                        << item->y() 
                        << item->scale()
                        << item->toPlainText() 
                        << item->font() 
                        << item->defaultTextColor();

                client->addedElement(TEXTE, data);

                break;
            }
        }
    }
}

void Server::connected(const QString & name)
{
    ServerSocket * s = dynamic_cast<ServerSocket *>(sender());

    if(s && !_clients.contains(name) && _unknownClients.contains(s))
    {
        
        qDebug() << "Client connected : " << name;

        for(auto client : _clients)
        {
            client->clientConnected(name, false);
            s->clientConnected(client->getName(), client->isAdmin());
        }

        QStringList liste;

        for(auto client: _controlList)
            liste << client->getName();


        s->controlList(liste);


        _unknownClients.removeOne(s);
        _clients[name] = s;


        sendScene(s);



    }
    else
    {
        qDebug() << "Client déjà connecté avec le même nom, on le déconnecte";


        _unknownClients.removeOne(s);

        s->disconnectFromHost();
        s->deleteLater();
    }
}

void Server::disconnected()
{
    ServerSocket * s = dynamic_cast<ServerSocket *>(sender());
    
    if(s)
    {
        if(_controlList.contains(s))
        {
            _controlList.removeOne(s);
            controlList(false);
        }

        if(_clients[s->getName()] == s)
        {
            _clients.remove(s->getName()); 
            for(auto client : _clients)
                client->clientDisconnected(s->getName());
        }
        else
            _unknownClients.removeOne(s);

        s->deleteLater();


        qDebug() << "Client disconnected";
    }
}

void Server::admin(const QString & mdp)
{
    ServerSocket * s = dynamic_cast<ServerSocket *>(sender());

    if(s && (_clients[s->getName()] == s))
    {
        if(mdp == _password)
        {
            s->setAdmin(true);

            for(auto client : _clients)
            {
                client->clientDisconnected(s->getName());
                client->clientConnected(s->getName(), true);
            }

            qDebug() << "Nouvel admin : " << s->getName();
        }
    }    
}

void Server::askedForControl()
{
    ServerSocket * s = dynamic_cast<ServerSocket *>(sender());

    if(s && (_clients[s->getName()] == s))
    {
        if(!_controlList.contains(s))
        {
            _controlList << s;

            controlList(false);
            qDebug() << s->getName() << " added in control list";
            qDebug() << _controlList;
        }
        else
        {
            qDebug() << "Le client est déjà dans la liste";
        }
    }
    else
        qDebug() << "Client not registered";
}

void Server::removedControl()
{
    ServerSocket * s = dynamic_cast<ServerSocket *>(sender());

    if(s && (_clients[s->getName()] == s))
    {
        if(_controlList.contains(s))
        {
            _controlList.removeOne(s);
            

            controlList(false);

            qDebug() << s->getName() << " removed from control list";
            qDebug() << _controlList;
        }
        else
            qDebug() << "Le client n'est pas dans la liste d'attente";
    }
    else
        qDebug() << "Client not registered";
}

void Server::releasedControl()
{
    ServerSocket * s = dynamic_cast<ServerSocket *>(sender());

    if(s && (_clients[s->getName()] == s))
    {
        if(_controlList.first() == s)
        {
            _controlList.pop_front();

            controlList(false);

            qDebug() << s->getName() << " released control";
            qDebug() << _controlList;
        }
    }
    else
        qDebug() << "Client not registered";
}

void Server::receivedMessage(const QString & message)
{
    ServerSocket * s = dynamic_cast<ServerSocket *>(sender());

    if(s && (_clients[s->getName()] == s))
    {
        qDebug() << "Meesage received from " << s->getName() << " : " << message;

        for(auto client : _clients)
            if(client != s)
                client->sendMessage(s->getName() + " : " + message);
    }
    else
        qDebug() << "Client not registered";
}

void Server::controlList(bool unicast)
{
    ServerSocket * s = dynamic_cast<ServerSocket *>(sender());

    QStringList liste;

    for(auto client : _controlList)
        liste << client->getName();


    if(s && unicast)
        s->controlList(liste);
    else
        for(auto client : _clients)
            client->controlList(liste);
}

void Server::addedElement(quint16 type, const QByteArray& args)
{
    ServerSocket * s = dynamic_cast<ServerSocket *>(sender());

    if(s && (_controlList.front() == s))
    {

        QDataStream in(args);
        in.setVersion(QDataStream::Qt_5_3);

        qDebug() << "On add un " << type;

        switch(type)
        {
            case LIGNE:
                addLine(in);
                break;

            case TEXTE:
                addText(in);
                break;

            case CARRE:
            case RECTANGLE:
                addRectangle(in);
                break;

            case TRIANGLE:
            case POLYGONE:
                addPolygon(in);
                break;

            case CERCLE:
            case ELLIPSE:
                addEllipse(in);
                break;

            default:
                qWarning() << "Forme inconnue (" << type << ")";
        }

        for(auto client : _clients)
            if(client != s)
                client->addedElement(type, args);
    }
}

void Server::deletedElement(quint16 id)
{
    ServerSocket * s = dynamic_cast<ServerSocket *>(sender());

    if(s && (_controlList.first() == s))
    {
        QGraphicsItem * item = _scene.at(id);

        _scene.removeAt(id);
        delete item;

        for(auto client : _clients)
            if(client != s)
                client->deletedElement(id);
    }   
}

void Server::movedElement(quint16 id, qreal dx, qreal dy)
{
    ServerSocket * s = dynamic_cast<ServerSocket *>(sender());

    if(s && (_controlList.front() == s))
    {
        QGraphicsItem * item = _scene.at(id);

        if(item)
        {
            item->moveBy(dx, dy);

            for(auto client : _clients)
                if(client != s)
                    client->movedElement(id, dx, dy);
        }
        else
            qWarning() << "Item non enregistré";
    }
}

void Server::modedElement(quint16 id, quint16 type, const QByteArray & args)
{
    ServerSocket * s = dynamic_cast<ServerSocket *>(sender());

    if(s && (_controlList.front() == s))
    {
        QGraphicsItem * item = _scene.at(id);

        if(!item)
        {
            qWarning() << "Item existe pas";
        }
        else
        {
            QDataStream in(args);
            in.setVersion(QDataStream::Qt_5_3);

            switch(type)
            {
                case LIGNE:
                    modLine(id, in);
                    break;

                case TEXTE:
                    modText(id, in);
                    break;

                case CARRE:
                case RECTANGLE:
                    modRectangle(id, in);
                    break;

                case TRIANGLE:
                case POLYGONE:
                    modPolygon(id, in);
                    break;

                case CERCLE:
                case ELLIPSE:
                    modEllipse(id, in);
                    break;

                default:
                    qWarning() << "Forme inconnue (" << type << ")";
            }

            for(auto client: _clients)
                if(client != s)
                    client->modedElement(id, type, args);
        }
    }
}

void Server::cleared()
{
    ServerSocket * s = dynamic_cast<ServerSocket *>(sender());

    if(s && (_controlList.first() == s))
    {
        _scene.clear();

        for(auto client : _clients)
            if(client != s)
                client->cleared();
    }
}

void Server::changedControlOwner(const QString & name)
{
    ServerSocket * s = dynamic_cast<ServerSocket *>(sender());

    if(s && s->isAdmin() && _clients.contains(s->getName()))
    {
        ServerSocket * nouveau = _clients[name];

        _controlList.removeOne(nouveau);

        if (nouveau)
            _controlList.push_front(nouveau);

        controlList();
    }
}

void Server::kickedUser(const QString & name)
{
    ServerSocket * s = dynamic_cast<ServerSocket *>(sender());

    if(s && s->isAdmin() && _clients.contains(s->getName()))
    {
        ServerSocket * victime = _clients[name];

        if(victime)
        {
            if(_controlList.removeOne(victime))
            {
                victime->deleteLater();
                controlList();
            }

            _clients.remove(victime->getName());
        }
    } 
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    ServerSocket * s = new ServerSocket(socketDescriptor);

    connect(s, SIGNAL(connect(const QString &)), this, SLOT(connected(const QString &)));
    connect(s, SIGNAL(disconnect()), this, SLOT(disconnected()));
    connect((QAbstractSocket *)s, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(s, SIGNAL(admin(const QString &)), this, SLOT(admin(const QString &)));

    connect(s, SIGNAL(askForControl()), this, SLOT(askedForControl()));
    connect(s, SIGNAL(removeControl()), this, SLOT(removedControl()));
    connect(s, SIGNAL(releaseControl()), this, SLOT(releasedControl()));
    connect(s, SIGNAL(changeControlOwner(const QString &)), this, SLOT(changedControlOwner(const QString &)));
    connect(s, SIGNAL(receiveMessage(const QString &)), this, SLOT(receivedMessage(const QString &)));
    
    connect(s, SIGNAL(kick(const QString &)), this, SLOT(kickedUser(const QString &)));
    
    connect(s, SIGNAL(addElement(quint16, const QByteArray &)), this, SLOT(addedElement(quint16, const QByteArray &)));
    connect(s, SIGNAL(delElement(quint16)), this, SLOT(deletedElement(quint16)));
    connect(s, SIGNAL(movElement(quint16, qreal, qreal)), this, SLOT(movedElement(quint16, qreal, qreal)));
    connect(s, SIGNAL(modElement(quint16, quint16, const QByteArray &)), this, SLOT(modedElement(quint16, quint16, const QByteArray &)));
    connect(s, SIGNAL(clear()), this, SLOT(cleared()));

    _unknownClients << s;
}
