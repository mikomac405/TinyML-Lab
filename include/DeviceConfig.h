#ifndef CONFIG_H
#define CONFIG_H

#include <ArduinoJson.h>
#include <optional>

enum MLConfig
{
    OFF = 0,
    PREDICTION = 1,
    TRAINING = 2
};

std::optional<MLConfig> toMlConfig(int value);

class DeviceConfig
{
private:
    bool m_configured = false;
    bool m_sendData = false;
    bool m_getMLDataFromGateway = false;
    int m_actionInterval = 0;
    MLConfig m_mlConfig = MLConfig::OFF;
    String m_testName = "default";

public:
    DeviceConfig();
    void initConfig(const JsonDocument &config);
    bool isConfigured() const { return m_configured; };
    bool getSendData() const { return m_sendData; }
    bool getGetMLDataFromGateway() const { return m_getMLDataFromGateway; }
    int getActionInterval() const { return m_actionInterval; }
    MLConfig getMlConfig() const { return m_mlConfig; }
    String getTestName() const { return m_testName; }
};
#endif