#pragma once

#include <string>

class AudioSource {
private:
    unsigned int source;
    unsigned int buffer;

    bool init();
public:
    AudioSource();
    AudioSource(std::string path);
    ~AudioSource();

    void attachBuffer(unsigned int);
    void play();
};