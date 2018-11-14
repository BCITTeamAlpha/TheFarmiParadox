#pragma once
//created by AssetLoader

#include "Al/al.h"
#include "Al/alc.h"

class Sound {
public:
    //methods
	Sound();
    ~Sound();
    
    void PlayAudio(ALuint source);
    void PauseAudio(ALuint source);
    bool isPlaying(ALuint source);
    void clearBuffer(ALuint buffer, ALuint source);
    void makeBuffer(ALuint* buffer);
    void makeSource(ALuint* source);
    void placeSource(ALuint source, float x, float y, float z);
    void toggleLooping(ALuint source, bool loop);
    void bufferData(ALuint buffer, ALuint source, const char * fn);


    //variables
    ALCdevice* device;
    ALCcontext* context;

private:
    
    char * ReadWavFile(const char* fn, int& chan, int& samplerate, int& bps, int& size);
    unsigned int determineFormat(int channels, int bps);
    int convertToInt(char* buffer, int len);
    bool isBigEndian();
};