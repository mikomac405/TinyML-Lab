#ifndef TINYMLMANANGER_H
#define TINYMLMANANGER_H
#include <memory>
#include <TinyRTPNN.h>

class TinyMlManager
{
private:
    std::map<String, std::shared_ptr<RTPNN::SDP>> m_sdpMap;

public:
    TinyMlManager(const std::vector<SensorType> outputs);
    void runML(JsonObject &readingObj, JsonObject &resultsObj, bool train = false);
};
#endif