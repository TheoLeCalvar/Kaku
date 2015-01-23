#ifndef SERVER_WINDOW_H
#define SERVER_WINDOW_H 

#include "Server.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QWidget>

class ServerWindow: public QWidget
{
Q_OBJECT
private:
    Server * _server;

public:
    ServerWindow();
    ~ServerWindow();
    


};

#endif