#ifndef RESPONSERUNNABLE_H
#define RESPONSERUNNABLE_H

#include <QRunnable>

/**
 * @brief QRunnable override for serving HTTP requests.
 *
 * Serves HTTP 1.0 requests<br>
 * Derives from QRunnable, it is meant to be executed on a thread
 * more precisely on a threadpool.
 */
class ResponseRunnable : public QRunnable
{
public:
    /**
     * @brief Constructs a ResponseRunnable.
     */
    ResponseRunnable();

protected:
    /**
     * @brief Serves the incoming request.
     *
     * If the client will not provide readable data in 500 milliseconds the
     * connection is closed.
     */
    void run();

public:
    /**
     * @brief Native socket descriptor for the accepted connection.
     */
    qintptr socketDescriptor;

};

#endif
