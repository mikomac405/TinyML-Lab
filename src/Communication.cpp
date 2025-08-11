#include "Communication.h"
#include "DeviceConfig.h"
#include "Logger.h"

Communication *Communication::communication_ = nullptr;

Communication::Communication(
    const String wifiSsid,
    const String wifiPassword,
    const String clientId,
    const String mqttHost,
    int mqttPort) : m_wifiSsid(wifiSsid),
                    m_wifiPassword(wifiPassword),
                    m_clientId(clientId),
                    m_mqttHost(mqttHost),
                    m_mqttPort(mqttPort)
{
    // Do not connect inside constructor
}

void Communication::wifiConnect()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(m_wifiSsid.c_str(), m_wifiPassword.c_str());

    Serial.println("[WiFi] Connecting...");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
    }
    Serial.println("[WiFi] Connected | IP address: " + WiFi.localIP().toString());
};

void Communication::mqttConnect()
{
    m_mqttClient.setServer(m_mqttHost.c_str(), m_mqttPort);
    m_mqttClient.setCallback(std::bind(&Communication::mqttCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

    Serial.println("[MQTT] Connecting...");
    while (!m_mqttClient.connected())
    {
        m_mqttClient.connect(m_clientId.c_str());
        delay(1000);
    }
    Serial.println("[MQTT] Connected");
    LOG_INFO(_cls, "mqttConnect", "MQTT and WiFi connected");
}

void Communication::connect()
{
    if (!WiFi.isConnected())
    {
        Serial.println("wifiConnect");
        wifiConnect();
    }

    if (!m_mqttClient.connected())
    {
        Serial.println("mqttConnect");
        mqttConnect();
    }

    if (!m_deviceConfig->isConfigured())
    {
        m_mqttClient.subscribe("configuration");
    }
}
void Communication::mqttCallback(char *topic, unsigned char *payload, unsigned int length)
{
    String topicStr = String(topic);
    String payloadStr = String((char *)payload);
    if (topicStr == "configuration")
    {
        JsonDocument doc;
        deserializeJson(doc, payload);
        m_deviceConfig->initConfig(doc);
        m_mqttClient.unsubscribe("configuration");
        m_mqttClient.subscribe(("cmd/" + getClientId() + "/in").c_str());
    }
    if (topicStr == "cmd/" + getClientId() + "/in")
    {
        JsonDocument doc;
        deserializeJson(doc, payload);
        if (doc["cmd"].as<String>() == "get_params")
        {
        }
        if (payloadStr.startsWith("set_params;"))
        {
        }
    }
};

Communication &Communication::getInstance(const char *wifiSsid, const char *wifiPassword, const char *clientId, const char *mqttHost, int mqttPort)
{
    if (communication_ == nullptr)
    {
        Serial.println("getInst construction");
        communication_ = new Communication(wifiSsid, wifiPassword, clientId, mqttHost, mqttPort);
    }
    return *communication_;
}

void Communication::publish(String topic, String payload, bool retain)
{
    m_mqttClient.publish(String(topic + '/' + getClientId()).c_str(), payload.c_str(), retain);
}

void Communication::handleMqttLoop()
{
    while (!WiFi.isConnected() || !m_mqttClient.connected())
    {
        connect();
    }
    m_mqttClient.loop();
    delay(10);
}

std::shared_ptr<DeviceConfig> Communication::getDeviceConfig()
{
    return m_deviceConfig;
}

String Communication::getClientId()
{
    return m_clientId;
}