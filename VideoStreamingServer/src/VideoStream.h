#pragma once

#include <fstream>
#include <queue>
#include <chrono>
#include <stdint.h>

namespace NextFlix
{
    class VideoStream
    {
    public:
        VideoStream() = delete;
        VideoStream(uint64_t userUUID, std::ifstream *videoFile, uint64_t timeStamp);
        ~VideoStream();

        void setIsStreaming(bool isStreaming);
        void setVideoTimeStamp(uint64_t timeStamp);
        void clearBuffer();

        uint64_t getVideoTimeStamp();
        uint64_t getUserUUID();
        uint64_t getLastUpdateDelta();
        uint64_t getBufferSize();

        bool fillBuffer();
        bool isUserStreaming();
        bool isVideoCompleted();
        char *getBufferNext();

    private:
        uint64_t userUUID;
        uint64_t timeStamp;

        std::ifstream *videoFile;
        std::vector<char> buffer;
        std::chrono::system_clock::time_point lastUpdate;

        bool isStreaming;
        bool videoCompleted;
    };
}