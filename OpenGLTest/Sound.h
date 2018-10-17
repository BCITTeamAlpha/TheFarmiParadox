#pragma once
//created by AssetLoader

#include "al.h"
#include "alc.h"

class Sound {
public:
    //methods
	Sound();
    ~Sound();
    void SwitchTrack();
    void PlayAudio();
    void PauseAudio();

    //variables
    ALCdevice* device;
    ALCcontext* context;
    unsigned int bgmBuffer;
    unsigned int bgmSource;

private:
    char * ReadWavFile(const char* fn, int& chan, int& samplerate, int& bps, int& size);
    unsigned int determineFormat(int channels, int bps);
    int convertToInt(char* buffer, int len);
    bool isBigEndian();
};