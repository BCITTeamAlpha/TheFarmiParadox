#pragma once
//has Sounds; created by main
#include "Sound.h"
#include "EventManager.h"
#include "SoundParams.h"
#include "TrackParams.h"
#include <unordered_map>
#define MAX_SOUND_BUFFERS 5




//actual Manager class
class SoundManager : public ISubscriber {
public:
	SoundManager();
    ~SoundManager();
    void playSong(TrackList track, float x, float y, float z);
    void playSound(SoundsList soundEffect, float x, float y, float z);
    void cleanUp();
    void ISubscriber::notify(EventName eventName, Param* param);

private:
    void loadAudioData();
    ALuint bgmBuffer;
    ALuint bgmSource;
    ALuint seBuffer[MAX_SOUND_BUFFERS];
    ALuint seSource[MAX_SOUND_BUFFERS];
    Sound* soundObject;
    std::unordered_map<TrackList, AudioData> Music;
    std::unordered_map<SoundsList, AudioData> SoundEffects;
};