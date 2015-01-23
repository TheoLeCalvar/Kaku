#include "ServerWindow.h"

#include <QApplication>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    ServerWindow s;

    s.show();
    
    return app.exec();
}