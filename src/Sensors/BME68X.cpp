#include "Sensors/BME68X.h"
#include "Logger.h"

BME68X::BME68X(const String &sensorName)
    : SensorBase(sensorName) {};

bool BME68X::initialize()
{
    m_sensor.begin(BME68X_I2C_ADDR_LOW, Wire);

    LOG_DEBUG(_cls, "initialize", "[BSEC] Library version " + String(m_sensor.version.major) + "." + String(m_sensor.version.minor) + "." + String(m_sensor.version.major_bugfix) + "." + String(m_sensor.version.minor_bugfix));

    checkIaqSensorStatus();

    bsec_virtual_sensor_t sensorList[13] = {
        BSEC_OUTPUT_IAQ,
        BSEC_OUTPUT_STATIC_IAQ,
        BSEC_OUTPUT_CO2_EQUIVALENT,
        BSEC_OUTPUT_BREATH_VOC_EQUIVALENT,
        BSEC_OUTPUT_RAW_TEMPERATURE,
        BSEC_OUTPUT_RAW_PRESSURE,
        BSEC_OUTPUT_RAW_HUMIDITY,
        BSEC_OUTPUT_RAW_GAS,
        BSEC_OUTPUT_STABILIZATION_STATUS,
        BSEC_OUTPUT_RUN_IN_STATUS,
        BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE,
        BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY,
        BSEC_OUTPUT_GAS_PERCENTAGE};

    m_sensor.updateSubscription(sensorList, 13, BSEC_SAMPLE_RATE_LP);

    checkIaqSensorStatus();

    m_isInitialized = true;

    LOG_DEBUG(_cls, "initialize", "[BSEC] Initialized");
    return m_isInitialized;
}

void BME68X::read(JsonObject &readingObj)
{
    if (m_sensor.run())
    {
        readingObj["Temperature"] = m_sensor.temperature;
        readingObj["Pressure"] = m_sensor.pressure;
        readingObj["Humidity"] = m_sensor.humidity;
        if (m_sensor.iaqAccuracy != 0)
        {
            readingObj["IAQ"] = m_sensor.iaq;
        }
    }
    else
    {
        checkIaqSensorStatus();
    }
}

void BME68X::loop()
{
}

bool BME68X::isInitialized()
{
    return m_isInitialized;
}

const std::vector<SensorType> BME68X::getSensorOutputs()
{
    return {SensorType::Temperature, SensorType::Humidity, SensorType::Pressure, SensorType::IAQ};
}

void BME68X::checkIaqSensorStatus()
{
    String output;
    if (m_sensor.bsecStatus != BSEC_OK)
    {
        if (m_sensor.bsecStatus < BSEC_OK)
        {
            LOG_ERROR(_cls, "checkIaqSensorStatus", "[BSEC] Error code : " + String(m_sensor.bsecStatus));
        }
        else
        {
            LOG_WARNING(_cls, "checkIaqSensorStatus", "[BSEC] Warning code : " + String(m_sensor.bsecStatus));
        }
    }

    if (m_sensor.bme68xStatus != BME68X_OK)
    {
        if (m_sensor.bme68xStatus < BME68X_OK)
        {
            LOG_ERROR(_cls, "checkIaqSensorStatus", "[BME68X] Error code : " + String(m_sensor.bme68xStatus));
        }
        else
        {
            LOG_WARNING(_cls, "checkIaqSensorStatus", "[BME68X] Warning code : " + String(m_sensor.bme68xStatus));
        }
    }
}
