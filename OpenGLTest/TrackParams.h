#pragma once
//for referencing songs
enum TrackList {
    MainBGM, MenuBGM
};



//for notifying the song event
struct TrackParams {
    TrackList track;
    float x;
    float y;
    float z;
};


