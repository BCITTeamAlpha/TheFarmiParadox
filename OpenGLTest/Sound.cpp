#include "Sound.h"
#include <windows.h>
#include <iostream>
#include <fstream>
#include <cstring>

#define MAIN_BGM "../Music/bgm1.wav" 

Sound::Sound() {
    //open default audio device
    device = alcOpenDevice(NULL);
    if (device == NULL) //ensure we have an actual device
    {
        std::cout << "cannot open sound card" << std::endl;
        return;
    }

    //create the audio context
    context = alcCreateContext(device, NULL);
    if (context == NULL) //more error handling incase the context fails to be established
    {
        std::cout << "cannot open context" << std::endl;
        return;
    }
    alcMakeContextCurrent(context); //assign ourselves to the created context

    //generate buffer
    alGenBuffers(1, &bgmBuffer);

    //generate our music source
    alGenSources(1, &bgmSource);

    //position variables
    float x = 0, z = 0;

    //place the music source
    alSource3f(bgmSource, AL_POSITION, x, 0, z);
    //set the music to loop
    alSourcei(bgmSource, AL_LOOPING, AL_TRUE);

    //place our listener
    float f[] = { 1,0,0,0,1,0 };
    alListenerfv(AL_ORIENTATION, f);
}

Sound::~Sound(){

    //clean up step
    alDeleteSources(1, &bgmSource);
    alDeleteBuffers(1, &bgmBuffer);

    alcDestroyContext(context);
    alcCloseDevice(device);
}


//method for switching tracks. Will make more robust and include a check for if we are already playing later
void Sound::SwitchTrack() {
    //Check for already playing and pause music
        //to do

    int channel, sampleRate, bps, size;
    
    //will include a switch statement here to determine which track we want in a later release
        //to do

    char* data = ReadWavFile(MAIN_BGM, channel, sampleRate, bps, size); //load the wav file

    //check that we actually got a return from our read attempt
    if (data == NULL) {
        std::cout << "Data was not read correctly for WAV file" << std::endl;
        return;
    }
    
    //determine our format
    unsigned int format = determineFormat(channel, bps);

    //load in the buffer data
    alBufferData(bgmBuffer, format, data, size, sampleRate);

    //attach buffer to source
    alSourcei(bgmSource, AL_BUFFER, bgmBuffer);

    delete[] data;
}

//action that starts the source playing
void Sound::PlayAudio() {
    //play the music
    alSourcePlay(bgmSource);

    //maybe a semaphore here, needs testing?
}

//action that pauses the source playing
void Sound::PauseAudio() {
    alSourcePause(bgmSource);
}

//custom method to read a Wav file for audio processing, uses filename, channel, samplerate, bits per sample and data size storage variables
char * Sound::ReadWavFile(const char * fn, int & chan, int & samplerate, int & bps, int & size) {
    //input filestream for the file (binary file)
    std::ifstream myFile;
    myFile.open(fn, std::ios::binary);

    //check to see if file was found
    if (!myFile.is_open()) {
        std::cout << "File not Found" << std::endl;
        return NULL;
    }

    char buffer[4];//buffer for reading data into as we decode important info

    myFile.read(buffer, 4); //location to read to, and amount to read.
    if (strncmp(buffer, "RIFF", 4) != 0) //RIFF is the wav header identifier
    {
        std::cout << "this is not a valid WAV file - RIFF" << std::endl;
        std::cout << buffer << std::endl;
        return NULL;
    }
    myFile.read(buffer, 4);      //size
    myFile.read(buffer, 4);      //WAVE
    if (strncmp(buffer, "WAVE", 4) != 0) //Format should be WAVE 
    {
        std::cout << "this is not a valid WAV file - WAVE" << std::endl;
        std::cout << buffer << std::endl;
        return NULL;
    }
    myFile.read(buffer, 4);      //fmt header, should contain 'fmt '
    if (strncmp(buffer, "fmt ", 4) != 0) //check for fmt
    {
        std::cout << "this is not a valid WAV file - FMT" << std::endl;
        std::cout << buffer << std::endl;
        return NULL;
    }
    myFile.read(buffer, 4);      //16
    myFile.read(buffer, 2);      //1
    myFile.read(buffer, 2);     //number of channels
    chan = convertToInt(buffer, 2);
    myFile.read(buffer, 4); //sample rate
    samplerate = convertToInt(buffer, 4);
    myFile.read(buffer, 4);
    myFile.read(buffer, 2);
    myFile.read(buffer, 2);
    bps = convertToInt(buffer, 2); // bits per sample
    myFile.read(buffer, 4);      //data
    if (strncmp(buffer, "data", 4) != 0) //data header should be contained here
    {
        std::cout << "this is not a valid WAV file - DATA" << std::endl;
        std::cout << buffer << std::endl;
        return NULL;
    }
    myFile.read(buffer, 4); //size of the contained data
    size = convertToInt(buffer, 4); //number of bytes in the date
    char* data = new char[size];
    myFile.read(data, size);
    return data;
}

//method to determine if we are stereo or mono
unsigned int Sound::determineFormat(int channels, int bps) {
    //determine format
    if (channels == 1) //1 channel mono sound
    {
        if (bps == 8)
        {
            return AL_FORMAT_MONO8;
        }
        else {
            return AL_FORMAT_MONO16;
        }
    }
    else { //1+ channels, stereo sound
        if (bps == 8)
        {
            return AL_FORMAT_STEREO8;
        }
        else {
            return AL_FORMAT_STEREO16;
        }
    }
}

//method to convert a byte array to an integer
int Sound::convertToInt(char * buffer, int len) {
    //create empty int for storing  bytes
    int a = 0;
    if (!isBigEndian()) //little endian machines
        for (int i = 0; i<len; i++)
            ((char*)&a)[i] = buffer[i]; //copy directly, as wav files use little endian in their spec
    else
        for (int i = 0; i<len; i++)
            ((char*)&a)[3 - i] = buffer[i]; //copy in reverse order, to transform them from little endian to big endian
    return a;
}

//determine what type of endianness the computer has
bool Sound::isBigEndian() {
    // 1 in bytes:
    //big endian renders this as 0 0 0 1
    //little endian renders this as 1 0 0 0
    int a = 1;
    return !((char*)&a)[0];//check the first byte of our integer, and it will be equal to a truth value, return the opposite to get correct endian-ness
}
