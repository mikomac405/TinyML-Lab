#ifndef LOGGER_H
#define LOGGER_H

#include <ArduinoJson.h>
#include <map>

enum LogLevel
{
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

std::optional<LogLevel> toLogLevel(int value);

extern const std::map<LogLevel, const char *> logLevelString;

void log(LogLevel level, const String &origin, const String &msg);

#define LOG_DEBUG(cls, func, msg) log(DEBUG, String(cls) + "::" + String(func) + "()", String(msg))
#define LOG_INFO(cls, func, msg) log(INFO, String(cls) + "::" + String(func) + "()", String(msg))
#define LOG_WARNING(cls, func, msg) log(WARNING, String(cls) + "::" + String(func) + "()", String(msg))
#define LOG_ERROR(cls, func, msg) log(ERROR, String(cls) + "::" + String(func) + "()", String(msg))

#endif