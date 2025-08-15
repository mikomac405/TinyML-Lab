#include "TinyMlManager.h"

TinyMlManager::TinyMlManager(const std::vector<SensorType> outputs)
{
    for (auto sensorType : outputs)
    {
        m_sdpMap.emplace(sensorConfigs.at(sensorType).name, std::make_shared<RTPNN::SDP>(sensorType));
    }
}

void TinyMlManager::runML(JsonObject &readingObj, JsonObject &resultsObj, bool train)
{
    for (auto reading : readingObj)
    {
        auto sdpName = reading.key().c_str();
        auto value = reading.value().as<double>();
        auto sdp = m_sdpMap.at(sdpName);

        resultsObj[sdpName] = sdp->perform(value, train);
    }
}
