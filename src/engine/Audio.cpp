#include "Audio.hpp"

#include <vector>
#include <fstream>

#include <al.h>
#include <alc.h>
#include <sndfile.hh>

#include "Logging.hpp"

namespace Audio {
    ALCdevice* device;
    ALCcontext* context;
    
    unsigned int source = 0;
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
    alGetError();

    alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED);
    
    return true;
}

unsigned int Audio::loadFile(std::string path) {
    std::ifstream tryOpenFile(path.c_str());
    if (!tryOpenFile.good()) {
        Logging::error("Failed to open audio file \"%s\".", path.c_str());
        return 0;
    }
    SF_INFO info;
    SndfileHandle file(path, SFM_READ);

    if (file.error()) {
        Logging::error("Failed to load audio file \"%s\": %s", path.c_str(), file.strError());
        return 0;
    }

    int frames = file.frames();
    int channels = file.channels();
    int samplerate = file.samplerate();

    short* bufferData = (short*)malloc(frames * channels * sizeof(short));

    file.read(bufferData, frames * channels);

    if (file.error()) {
        Logging::error("Failed to read audio file \"%s\": %s", path.c_str(), file.strError());
        return 0;
    }

    unsigned int buffer = 0;
    alGenBuffers(1, &buffer);

    alBufferData(buffer, AL_FORMAT_STEREO16, bufferData, frames * channels * sizeof(short), file.samplerate());
    
    free(bufferData);

    return buffer;
}

void Audio::quit() {
    if (source != 0) {
        alDeleteSources(1, &source);
    }
    if (context != 0) {
        alcDestroyContext(context);
    }
    if (device != 0) {
        alcCloseDevice(device);
    }
}