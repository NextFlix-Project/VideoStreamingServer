#include "VideoStream.h"
#include <iostream>
 
#define BUFFER_SIZE 5242880  // 5 Megabit

using namespace NextFlix;

VideoStream::VideoStream(uint64_t userUUID, std::ifstream *videoFile, uint64_t timeStamp)
{
    this->userUUID = userUUID;
    this->videoFile = videoFile;
    this->timeStamp = timeStamp;
    this->buffer.resize(BUFFER_SIZE);
    this->isStreaming = true;
    this->videoCompleted = false;
}

VideoStream::~VideoStream()
{
     
}

bool VideoStream::fillBuffer()
{
    if (!videoFile || !videoFile->is_open() || videoCompleted)
        return false;
    
    clearBuffer();

    videoFile->seekg(0, std::ios::end);
    uint64_t fileSize = videoFile->tellg();

    videoFile->seekg(timeStamp);
    
    if (fileSize - timeStamp < BUFFER_SIZE)
        buffer.resize(fileSize - timeStamp);
    else
        buffer.resize(BUFFER_SIZE);

    char data[BUFFER_SIZE];
    videoFile->read(&buffer[0], BUFFER_SIZE);
    
    uint64_t bytesRead = videoFile->gcount();


    std::cout << "Read " << bytesRead << " bytes from the file. " << buffer.size() << "  timeStamp   " << timeStamp << std::endl;
   

    if (videoFile->eof())
    {
        videoCompleted = true;
        isStreaming = false;
    }
    
    timeStamp += BUFFER_SIZE;

    return true;
}

char *VideoStream::getBufferNext()
{
    lastUpdate = std::chrono::system_clock::now();

    if (videoCompleted) return NULL;
 

    return buffer.data();
}

bool VideoStream::isUserStreaming()
{
    return isStreaming;
}

bool VideoStream::isVideoCompleted()
{
    return videoCompleted;
}

void VideoStream::setIsStreaming(bool isStreaming)
{
    this->isStreaming = isStreaming;
}

uint64_t VideoStream::getVideoTimeStamp()
{
    return timeStamp;
}

uint64_t VideoStream::getUserUUID()
{
    return userUUID;
}

uint64_t VideoStream::getBufferSize()
{
    return buffer.size();
}

void VideoStream::setVideoTimeStamp(uint64_t timeStamp)
{
    this->timeStamp = timeStamp;
}

uint64_t VideoStream::getLastUpdateDelta()
{
    std::chrono::duration<double> delta = std::chrono::system_clock::now() - lastUpdate;
    uint64_t timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::minutes>(delta).count();

    return timeSinceLastUpdate;
}

void VideoStream::clearBuffer()
{
    buffer.clear();
}