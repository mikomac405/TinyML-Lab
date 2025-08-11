#include "Logger.h"
#include "Communication.h"

const std::map<LogLevel, const char *> logLevelString{
    {DEBUG, "DEBUG"},
    {INFO, "INFO"},
    {WARNING, "WARNING"},
    {ERROR, "ERROR"}};

std::optional<LogLevel> toLogLevel(int value)
{
    switch (value)
    {
    case 0:
    case 1:
    case 2:
    case 3:
        return static_cast<LogLevel>(value);
    default:
        return std::nullopt;
    }
}

void log(LogLevel level, const String &origin, const String &msg)
{
    if (LOG_LEVEL > level)
        return;

    JsonDocument log;
    log["timestamp"] = millis();
    log["level"] = logLevelString.at(level);
    log["origin"] = origin;
    log["message"] = msg;

    String jsonString;
    serializeJson(log, jsonString);
    Serial.println(jsonString);
    Communication::getInstance().publish("log", jsonString);
}
