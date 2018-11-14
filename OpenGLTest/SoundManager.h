#pragma once
//has Sounds; created by main
#include "Sound.h"
#include "EventManager.h"

//for referencing songs
enum TrackList
{MainBGM, MenuBGM};

//for referencing sounds
enum SoundsList
{Jump};

//for notifying the song event
struct TrackParams {
    TrackList track;
    float position[3];
};

//for notifying the sound event
struct SoundParams {
    SoundsList sound;
    float position[3];
};

class SoundManager : public ISubscriber {
public:
	SoundManager();
    ~SoundManager();
    void playSong(TrackList track, float position[3]);
    void playSound(SoundsList soundEffect, float position[3]);
    void cleanUp();
    void ISubscriber::notify(EventName eventName, Param* param);

private:
    ALuint bgmBuffer;
    ALuint bgmSource;
    ALuint seBuffer;
    ALuint seSource;
    Sound* soundObject;
};