#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QThreadPool>
#include <QElapsedTimer>

/**
 * @brief HTTP server implementation.
 *
 * Privilege handling:<br>
 * The following commands should be executed on the executable using this class:<br>
 * chown root [name_of_executable]<br>
 * chmod 6555 [name_of_executable]<br>
 * The reason behind theese extra modifications is that the server needs to run
 * with ruid = not_root_user_id, rgid = not_root_group_id, euid = root_user_id,
 * egid = root_group_id so that listen can be called on protected ports and
 * chroot can be called to create a jail for the process.<br>
 * After theese procedures the privileges will be dropped.
 */
class Server : public QTcpServer
{
    Q_OBJECT
public:
    /**
     * @brief Constructs a Server object.
     * @param port Port number where the server will listen.
     * @param maxThreadCount Maximum size of the threadpool that serves requests.
     * @param maxPendingConnections Maximum number of requests waiting in the queue.
     * @param maxResponsePerSecond Maximum number of responses made in one second.
     * @param parent Passed to the QObject constructor.
     */
    explicit Server(qint16 port = 80,
                    int maxThreadCount = 5,
                    int maxPendingConnections = 30,
                    int maxResponsePerSecond = 100,
                    QObject *parent = 0);

    /**
     * @brief Starts the server.
     *
     * Checks if the privileges are correct.<br>
     * Checks for the php package.<br>
     * Creates the environment for the server.<br>
     * Starts listening on all network interfaces using the designated port.
     * @return Returns 0 if successful, othervise not 0.
     */
    int startServer();

protected:
    /**
     * @brief Handels the incoming requests.
     *
     * Overrides QTcpServer::incomingConnection().<br>
     * Starts the handling of a request on another thread.
     * @param socketDescriptor Native socket descriptor for the accepted connection.
     */
    void incomingConnection(qintptr socketDescriptor);

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
