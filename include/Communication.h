#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#ifdef ESP8266
#include "ESP8266WiFi.h"
#else
#include "WiFi.h"
#endif

#include <PubSubClient.h>
#include <DeviceConfig.h>

class Communication
{
private:
    String m_wifiSsid;
    String m_wifiPassword;
    String m_clientId;
    String m_mqttHost;
    int m_mqttPort;

    const String _cls = "Communication";

    WiFiClient m_wifiClient;
    PubSubClient m_mqttClient = PubSubClient(m_wifiClient);
    std::shared_ptr<DeviceConfig> m_deviceConfig = std::make_shared<DeviceConfig>();

protected:
    Communication(
        const String wifiSsid,
        const String wifiPassword,
        const String clientId,
        const String mqttHost,
        int mqttPort);

    static Communication *communication_;

public:
    Communication(Communication &other) = delete;
    void operator=(const Communication &) = delete;

    static Communication &getInstance(
        const char *wifiSsid = "",
        const char *wifiPassword = "",
        const char *clientId = "",
        const char *mqttHost = "",
        int mqttPort = 0);

    void publish(String topic, String payload, bool retain = false);
    void handleMqttLoop();
    std::shared_ptr<DeviceConfig> getDeviceConfig();
    String getClientId();
    void connect();

private:
    void wifiConnect();
    void mqttConnect();
    void mqttCallback(char *topic, unsigned char *payload, unsigned int length);
};

#endif