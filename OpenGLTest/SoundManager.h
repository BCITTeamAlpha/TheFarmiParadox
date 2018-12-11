#pragma once
//has Sounds; created by main
#include "Sound.h"
#include "EventManager.h"
#include <unordered_map>

//for referencing songs
enum TrackList
{MainBGM, MenuBGM};

//for referencing sounds
enum SoundsList
{Jump};

//for notifying the song event
struct TrackParams {
    TrackList track;
    float x;
    float y;
    float z;
};

//for notifying the sound event
struct SoundParams {
    SoundsList sound;
    float x;
    float y;
    float z;
};

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
    ALuint seBuffer;
    ALuint seSource;
    Sound* soundObject;
    std::unordered_map<TrackList, AudioData> Music;
    std::unordered_map<SoundsList, AudioData> SoundEffects;
};