#include "SoundManager.h"
#include <iostream>
#include <mutex>
//defines for track filenames
#define MAIN_BGM "../Music/bgm1.wav" 
#define MENU_BGM "../Music/bgm2.wav"

//defines for SoundEffect Names
#define JUMP "../Sounds/jumpSE.wav"
#define DAMAGE "../Sounds/damageSE.wav"

//private mutex variables
std::mutex SoundMtx;
int currentSoundBuffer;

SoundManager::SoundManager() {
    soundObject = new Sound();
    soundObject->makeBuffer(&bgmBuffer);
    soundObject->makeSource(&bgmSource);
    for (int i = 0; i < MAX_SOUND_BUFFERS; i++) {
        soundObject->makeSource(&seSource[i]);
        soundObject->makeBuffer(&seBuffer[i]);
    }
    currentSoundBuffer = 0;
    loadAudioData();
}

void SoundManager::loadAudioData() {
    //Load Music
    
    //load main BGM and store it
    AudioData temp;
    temp = soundObject->getAudioData(MAIN_BGM);
    if (temp.data == NULL) {
        std::cout << "Main BGM failed to store correctly." << std::endl;
    }
    Music.insert(std::pair<TrackList,AudioData>(MainBGM, temp));
    //load menu BGM
    temp = soundObject->getAudioData(MENU_BGM);
    if (temp.data == NULL) {
        std::cout << "Menu BGM failed to store correctly." << std::endl;
    }
    Music.insert(std::pair<TrackList, AudioData>(MenuBGM, temp));

    //Load Sounds
    //Jump Sound
    temp = soundObject->getAudioData(JUMP);
    if (temp.data == NULL) {
        std::cout << "Jump SE failed to store correctly." << std::endl;
    }
    SoundEffects.insert(std::pair<SoundsList, AudioData>(Jump, temp));

    //Explosion Sound
    temp = soundObject->getAudioData(DAMAGE);
    if (temp.data == NULL) {
        std::cout << "Explosion SE failed to store correctly." << std::endl;
    }
    SoundEffects.insert(std::pair<SoundsList, AudioData>(Damage, temp));
}

SoundManager::~SoundManager() {
    cleanUp();
}

void SoundManager::playSong(TrackList track, float x, float y, float z) {
    if (soundObject->isPlaying(bgmSource)) {
        soundObject->PauseAudio(bgmSource);
        soundObject->clearBuffer(bgmBuffer,bgmSource);
    }

    switch (track){
        case MainBGM:
            soundObject->bufferData(bgmBuffer, bgmSource, Music.find(MainBGM)->second);
            soundObject->toggleLooping(bgmSource, true);
            soundObject->placeSource(bgmSource, x, y, z);
            soundObject->PlayAudio(bgmSource);
            break;
        case MenuBGM:
            soundObject->bufferData(bgmBuffer, bgmSource, Music.find(MenuBGM)->second);
            soundObject->toggleLooping(bgmSource, true);
            soundObject->placeSource(bgmSource, x, y, z);
            soundObject->PlayAudio(bgmSource);
            break;
        default:
            break;
    }
}

void SoundManager::playSound(SoundsList soundEffect, float x, float y, float z) {
    //mutex check to ensure we don't commit access violations by accessing dead buffers while they're remade.
    
    SoundMtx.lock();
    int selectedBuffer = currentSoundBuffer;
    currentSoundBuffer++;
    if (currentSoundBuffer >= MAX_SOUND_BUFFERS) {
        currentSoundBuffer = 0;
    }
    SoundMtx.unlock();
    

    //check if our selected buffer
    if (soundObject->isPlaying(seSource[selectedBuffer])) {
        soundObject->PauseAudio(seSource[selectedBuffer]);
        soundObject->clearBuffer(seBuffer[selectedBuffer], seSource[selectedBuffer]);
    }

    switch (soundEffect) {
    case Jump:
        soundObject->bufferData(seBuffer[selectedBuffer], seSource[selectedBuffer], SoundEffects.find(Jump)->second);
        soundObject->toggleLooping(seSource[selectedBuffer], false);
        soundObject->placeSource(seSource[selectedBuffer], x, y, z);
        soundObject->PlayAudio(seSource[selectedBuffer]);
        break;
    case Damage:
        soundObject->bufferData(seBuffer[selectedBuffer], seSource[selectedBuffer], SoundEffects.find(Damage)->second);
        soundObject->toggleLooping(seSource[selectedBuffer], false);
        soundObject->placeSource(seSource[selectedBuffer], x, y, z);
        soundObject->PlayAudio(seSource[selectedBuffer]);
        break;
    default:
        break;
    }
}

void SoundManager::cleanUp() {
    //clean up step

    //bgm cleanup
    alDeleteSources(1, &bgmSource);
    alDeleteBuffers(1, &bgmBuffer);

    //sound effect cleanup
    for (int i = 0; i < MAX_SOUND_BUFFERS; i++) {
        alDeleteSources(1, &seSource[i]);
        alDeleteBuffers(1, &seBuffer[i]);
    }

    //sound object cleanup
    soundObject->~Sound();
}

void SoundManager::notify(EventName eventName, Param* param) {

    switch (eventName) {
        case PLAY_SONG: {
            TrackParams* TrackInfo;
            // Safetly cast generic param pointer to a specific type
            TypeParam<TrackParams *> *p = dynamic_cast<TypeParam<TrackParams*> *>(param);
            if (p != nullptr) {
                // Successful type cast
                TrackInfo = p->Param;
                playSong(TrackInfo->track, TrackInfo->x, TrackInfo->y, TrackInfo->z);
            }
            break;
        }
        case PLAY_SOUND:{
            SoundParams* SoundInfo;
            // Safetly cast generic param pointer to a specific type
            TypeParam<SoundParams*> *sound = dynamic_cast<TypeParam<SoundParams*> *>(param);
            if (sound != nullptr) {
                // Successful type cast
                SoundInfo = sound->Param;
                playSound(SoundInfo->sound, SoundInfo->x, SoundInfo->y, SoundInfo->z);
                delete SoundInfo;
                delete param;
            }
            break;
        }
        default:
            break;
    }

}