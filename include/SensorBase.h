#ifndef SENSORBASE_H
#define SENSORBASE_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Utils.h>

class SensorBase
{
protected:
    String m_sensorName;
    bool m_isInitialized;

public:
    SensorBase(const String &sensorName) : m_sensorName(sensorName),
                                           m_isInitialized(false) {};

    virtual ~SensorBase() = default;

    virtual bool initialize() = 0;
    virtual void read(JsonObject &readingObj) = 0;
    virtual void loop() = 0;
    virtual bool isInitialized() = 0;
    virtual const std::vector<SensorType> getSensorOutputs() = 0;
};

#endif