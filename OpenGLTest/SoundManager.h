#pragma once
//has Sounds; created by main
#include "Sound.h"

enum TrackList
{MainBGM, MenuBGM};

enum SoundsList
{Jump};

class SoundManager {
public:
	SoundManager();
    ~SoundManager();
    void playSong(TrackList track);
    void playSound(SoundsList soundEffect);
    void cleanUp();
    void eventListener();

private:
    ALuint bgmBuffer;
    ALuint bgmSource;
    ALuint seBuffer;
    ALuint seSource;
    Sound* soundObject;
};