#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QThreadPool>
#include <QElapsedTimer>

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(qint16 port = 80,
                    int maxThreadCount = 5,
                    int maxPendingConnections = 30,
                    int maxResponsePerSecond = 100,
                    QObject *parent = 0);

    int startServer();

protected:
    void incomingConnection( qintptr handle );

signals:

public slots:

private:
    QThreadPool *pool;
    QElapsedTimer timer;
    qlonglong connectionCounter = 0;
    int maxResponsePerSecond;
    qint16 port;

    int checkPHPInstalled() const;
    int CreateJailWithPHP() const;
};

#endif
