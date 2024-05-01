#include "Audio.hpp"

#include <vector>

#include <al.h>
#include <alc.h>

#include "Logging.hpp"

namespace Audio {
    std::vector<unsigned int> audioBuffers;

    ALCdevice* device;
    ALCcontext* context;
}

bool Audio::init() {
    device = alcOpenDevice(0);
    if (device == NULL) {
        Logging::error("Failed to initialize OpenAL: %s", alGetString(alGetError()));
        return false;
    }
    context = alcCreateContext(device, 0);
    if (context == NULL) {
        Logging::error("Failed to create OpenAL context: %s", alGetString(alGetError()));
        alcCloseDevice(device);
        return false;
    }
    alcMakeContextCurrent(context);
    
    return true;
}

void Audio::quit() {
    alcDestroyContext(context);
    alcCloseDevice(device);
}