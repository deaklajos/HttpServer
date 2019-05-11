#ifndef RESPONSEWORKER_H
#define RESPONSEWORKER_H

#include <QRunnable>

class ResponseWorker : public QRunnable
{
public:
    ResponseWorker();

protected:
    void run();

public:
    qintptr socketDescriptor;

};

#endif
