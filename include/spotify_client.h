#ifndef SPOTIFY_CLIENT_H
#define SPOTIFY_CLIENT_H

#include <Arduino.h>

struct SpotifyTrack {
    String title = "";
    String artist = "";
    String album = "";
    String albumArtUrl = "";
    bool isPlaying = false;
    long progressMs = 0;
    long durationMs = 0;
};

SpotifyTrack getCurrentlyPlaying(String& accessToken);

#endif