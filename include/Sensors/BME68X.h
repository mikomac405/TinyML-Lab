#ifndef BME68X_H
#define BME68X_H

#include "SensorBase.h"
#include "bsec.h"

class BME68X : public SensorBase
{
public:
    BME68X(const String &sensorName);

    bool initialize() override;
    void read(JsonObject &readingObj) override;
    void loop() override;
    bool isInitialized() override;
    const std::vector<SensorType> getSensorOutputs() override;

private:
    void checkIaqSensorStatus();

    Bsec m_sensor;
    const String _cls = "BME68X";
};

#endif