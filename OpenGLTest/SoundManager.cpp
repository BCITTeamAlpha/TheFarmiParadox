#include "SoundManager.h"

//defines for track filenames
#define MAIN_BGM "../Music/bgm1.wav" 
#define MENU_BGM "../Music/bgm2.wav"

//defines for SoundEffect Names
#define JUMP ""

SoundManager::SoundManager() {
    soundObject = new Sound();
    soundObject->makeBuffer(&bgmBuffer);
    soundObject->makeBuffer(&seBuffer);
    soundObject->makeSource(&bgmSource);
    soundObject->makeSource(&seSource);
}

SoundManager::~SoundManager() {
    cleanUp();
}

void SoundManager::playSong(TrackList track) {
    if (soundObject->isPlaying(bgmSource)) {
        soundObject->PauseAudio(bgmSource);
        soundObject->clearBuffer(bgmBuffer,bgmSource);
    }

    switch (track){
        case MainBGM:
            soundObject->bufferData(bgmBuffer, bgmSource, MAIN_BGM);
            soundObject->toggleLooping(bgmSource, true);
            soundObject->placeSource(bgmSource, 0, 0);
            soundObject->PlayAudio(bgmSource);
            break;
        case MenuBGM:
            soundObject->bufferData(bgmBuffer, bgmSource, MENU_BGM);
            soundObject->toggleLooping(bgmSource, true);
            soundObject->placeSource(bgmSource, 0, 0);
            soundObject->PlayAudio(bgmSource);
            break;
        default:
            break;
    }
}

void SoundManager::playSound(SoundsList soundEffect) {
    if (soundObject->isPlaying(seSource)) {
        soundObject->PauseAudio(seSource);
        soundObject->clearBuffer(seBuffer, seSource);
    }

    switch (soundEffect) {
    case Jump:
        soundObject->bufferData(seBuffer, bgmSource, JUMP);
        soundObject->toggleLooping(seSource, true);
        soundObject->placeSource(seSource, 0, 0);
        soundObject->PlayAudio(seSource);
        break;
    default:
        break;
    }
}

void SoundManager::cleanUp() {
    //clean up step
    alDeleteSources(1, &bgmSource);
    alDeleteSources(1, &seSource);
    alDeleteBuffers(1, &bgmBuffer);
    alDeleteBuffers(1, &seBuffer);

    soundObject->~Sound();
}
