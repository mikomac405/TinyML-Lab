#include "Communication.h"
#include "Sensors/BME68X.h"

std::unique_ptr<SensorBase> sensor = nullptr;

unsigned long lastRead = 0;

void setup()
{
    Serial.begin(115200);
    Serial.println("Main fun");
    Communication::getInstance(WIFI_SSID, WIFI_PASSWORD, CLIENT_ID, MQTT_HOST, MQTT_PORT).connect();
    sensor = std::make_unique<BME68X>("Environmental Sensor");
    sensor->initialize();
}

void loop()
{
    if (auto config = Communication::getInstance().getDeviceConfig(); config->isConfigured())
    {
        if (millis() - lastRead > config->getActionInterval())
        {
            lastRead = millis();
            JsonDocument doc;
            JsonObject sensor_reading = doc["data"].to<JsonObject>();
            sensor->read(sensor_reading);

            if (config->getSendSensorData() && !sensor_reading.isNull())
            {
                doc["device"] = CLIENT_ID;
                doc["test_name"] = config->getTestName();
                String doc_string;
                serializeJson(doc, doc_string);
                Communication::getInstance().publish("data", doc_string);
            }

            switch (config->getMlConfig())
            {
            case MLConfig::TRAINING:
                // TinyML - Train
            case MLConfig::PREDICTION:
                // TinyML - Predict
                if (config->getSendMLData())
                {
                    // Send prediction data
                }
                break;
            case MLConfig::OFF:
            default:
                break;
            }
        }
    }
    else
    {
        Serial.println("System not configured.");
        Communication::getInstance().handleMqttLoop();
        delay(1000);
    }
}