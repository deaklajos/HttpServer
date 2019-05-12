#ifndef LOGGER_H
#define LOGGER_H

#include <QString>

/**
 * @brief Singleton threadsafe class for verbose file logging.
 */
class Logger
{
public:
    /**
     * @brief Returns the singleton instance.
     * @return Singleton instace.
     */
    static Logger& getInstance()
    {
        static Logger instance;
        return instance;
    }

    /**
     * @brief Writes a log line into the log.log file.
     *
     * The log file is lazy initialized.<br>
     * This fuction is thread safe.<br>
     * The log line has the following signature:<br>
     * [QtMsgType] [thread_id] time_in("dd-MM-yyyy HH:mm:ss:zzz") message
     *
     * @param type Represents the type of the message.
     * @param message The message string to be logged.
     */
    void Log(QtMsgType type, const QString& message);

private:
    Logger() {}

public:
    Logger(Logger const&) = delete;
    void operator=(Logger const&) = delete;
};
#endif
