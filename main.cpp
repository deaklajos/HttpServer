#include <QCoreApplication>
#include "Server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Server server;
    int code = server.startServer();
    if(code)
        return code;

    return a.exec();
}
