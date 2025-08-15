#include "Communication.h"
#include "Sensors/BME68X.h"
#include "TinyMlManager.h"

std::unique_ptr<SensorBase> sensor = nullptr;
std::unique_ptr<TinyMlManager> mlManager = nullptr;

unsigned long lastRead = 0;

void setup()
{
    Serial.begin(115200);
    Communication::getInstance(WIFI_SSID, WIFI_PASSWORD, CLIENT_ID, MQTT_HOST, MQTT_PORT).connect();
    sensor = std::make_unique<BME68X>("Environmental Sensor");
    sensor->initialize();
    mlManager = std::make_unique<TinyMlManager>(sensor->getSensorOutputs());
}

void loop()
{
    if (auto config = Communication::getInstance().getDeviceConfig(); config->isConfigured())
    {
        if (millis() - lastRead > static_cast<unsigned long>(config->getActionInterval()))
        {
            lastRead = millis();

            // JsonDocument Base
            JsonDocument doc;
            doc["test_name"] = config->getTestName();
            JsonObject data = doc["data"].to<JsonObject>();
            JsonObject sensor_data = data["sensor_data"].to<JsonObject>();
            JsonObject ml_data;
            auto mlConfig = config->getMlConfig();
            sensor->read(sensor_data);

            if (mlConfig != MLConfig::OFF)
            {
                ml_data = data["ml_data"].to<JsonObject>();
                mlManager->runML(sensor_data, ml_data, mlConfig == MLConfig::TRAINING);
            }

            if (config->getSendData() && !(sensor_data.isNull() && ml_data.isNull()))
            {
                String data_string;
                serializeJson(doc, data_string);
                Communication::getInstance().publish("data", data_string);
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