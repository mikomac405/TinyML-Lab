#include "DeviceConfig.h"

std::optional<MLConfig> toMlConfig(int value)
{
    switch (value)
    {
    case 0:
    case 1:
    case 2:
        return static_cast<MLConfig>(value);
    default:
        return std::nullopt;
    }
}

DeviceConfig::DeviceConfig() {}

void DeviceConfig::initConfig(const JsonDocument &config)
{
    if (config.containsKey("sendData"))
        m_sendData = config["sendData"].as<bool>();
    if (config.containsKey("getMLDataFromGateway"))
        m_getMLDataFromGateway = config["getMLDataFromGateway"].as<bool>();
    if (config.containsKey("actionInterval"))
        m_actionInterval = config["actionInterval"].as<int>();
    if (config.containsKey("testName"))
        m_testName = config["testName"].as<String>();
    if (config.containsKey("mlConfig"))
    {
        if (auto config_value = toMlConfig(config["mlConfig"].as<int>()); config_value.has_value())
            m_mlConfig = config_value.value();
    }
    m_configured = true;
}
