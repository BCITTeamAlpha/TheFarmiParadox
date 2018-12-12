#pragma once
//for referencing sounds
enum SoundsList {
    Jump,Damage
};

//for notifying the sound event
struct SoundParams {
    SoundsList sound;
    float x;
    float y;
    float z;
};

