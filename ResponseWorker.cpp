#include "ResponseWorker.h"

ResponseWorker::ResponseWorker()
{
}

void ResponseWorker::run()
{
    if(!socketDescriptor) return;

    QTcpSocket socket;
    socket.setSocketDescriptor(socketDescriptor);

    socket.write("From server: hello world");
    socket.flush();
    socket.waitForBytesWritten();
    socket.close();
}
