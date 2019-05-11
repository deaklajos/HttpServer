#include <QCoreApplication>
#include <iostream>

#include "Server.h"
#include <libconfig.h++>

using namespace libconfig;

int main(int argc, char *argv[])
{
    QCoreApplication::setSetuidAllowed(true);
    QCoreApplication a(argc, argv);

    std::cout << "Are you sure that you want to host a server from here(y/n)?" << std::endl;
    std::cout << "chroot jail will be created here containing php and its dependecies!" << std::endl;
    char answer;
    std::cin >> answer;
    if(answer != 'y')
        return 0;

    if(argc < 2)
    {
        qDebug() << "Config file required as parameter.";
        return(EXIT_FAILURE);
    }

    Config cfg;

    // Read the file. If there is an error, report it and exit.
    try
    {
        cfg.readFile(argv[1]);
    }
    catch(const FileIOException &fioex)
    {
        qDebug() << "I/O error while reading file.";
        return(EXIT_FAILURE);
    }
    catch(const ParseException &pex)
    {
        qDebug() << "Parse error at " << pex.getFile() << ":" << pex.getLine()
                  << " - " << pex.getError();
        return(EXIT_FAILURE);
    }

    // Get the port number
    int port = -1;
    int maxPendingConntection = -1;
    int maxResponsePerSecond = -1;
    int threadPoolSize = -1;

    try
    {
        port = cfg.lookup("port");
    }
    catch(const SettingNotFoundException &nfex)
    {
        qDebug() << "No 'port' setting in configuration file.";
        return(EXIT_FAILURE);
    }
    try
    {
        maxPendingConntection = cfg.lookup("max_pending_connection");
    }
    catch(const SettingNotFoundException &nfex)
    {
        qDebug() << "No 'max_pending_connection' setting in configuration file.";
        return(EXIT_FAILURE);
    }
    try
    {
        maxResponsePerSecond = cfg.lookup("max_response_per_second");
    }
    catch(const SettingNotFoundException &nfex)
    {
        qDebug() << "No 'max_response_per_second' setting in configuration file.";
        return(EXIT_FAILURE);
    }
    try
    {
        threadPoolSize = cfg.lookup("threadpool_size");
    }
    catch(const SettingNotFoundException &nfex)
    {
        qDebug() << "No 'threadpool_size' setting in configuration file.";
        return(EXIT_FAILURE);
    }

    Server server(port, threadPoolSize, maxPendingConntection, maxResponsePerSecond);
    int code = server.startServer();
    if(code)
        return code;

    return a.exec();
}
