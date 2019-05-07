#ifndef LOGGER_H
#define LOGGER_H

#include <QString>

class Logger
{
public:
    static Logger& getInstance()
    {
        static Logger instance;
        return instance;
    }

    void Log(QtMsgType type, const QString& message);
private:
    Logger() {}

public:
    Logger(Logger const&) = delete;
    void operator=(Logger const&) = delete;
};
#endif
