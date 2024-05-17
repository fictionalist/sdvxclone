#include "AudioSource.hpp"

#include <string>

#include <al.h>

#include "Audio.hpp"
#include "Logging.hpp"

AudioSource::AudioSource() {
    if (!init()) {
        return;
    }
}

AudioSource::AudioSource(std::string path) {
    if (!init()) {
        return;
    }

    buffer = Audio::loadFile(path);
    attachBuffer(buffer);
}

bool AudioSource::init() {
    source = 0;
    alGenSources(1, &source);
    ALenum err = alGetError();
    if (err != AL_NO_ERROR) {
        Logging::error("Failed to create audio source: %s", alGetString(err));
        source = 0;
        return false;
    }

    alSourcef(source, AL_REFERENCE_DISTANCE, 0.0f);
    alSource3f(source, AL_POSITION, 0.0f, 0.0f, 5.0f);
    alSourcef(source, AL_ROLLOFF_FACTOR, 1.0f);

    return true;
}

void AudioSource::attachBuffer(unsigned int buffer) {
    alSourcei(source, AL_BUFFER, buffer);
}

void AudioSource::play() {
    alSourcePlay(source);
}

AudioSource::~AudioSource() {
    if (source != 0) {
        alDeleteSources(1, &source);
    }
}