//
//  AudioListener.hpp
//  c8051SwiftGL
//
//  Created by Thomas on 2022-03-29.
//

#ifndef AudioListener_hpp
#define AudioListener_hpp

#include <stdio.h>
#include <queue>

class AudioEvent
{
public:
    AudioEvent(int trackReferenceß);
    ~AudioEvent();
    
    int getTrack();
protected:
private:
    int track;
};

class AudioListener
{
public:
    ~AudioListener();
    
    void addAudioEvent(AudioEvent* event);
    AudioEvent* getNextAudioEvent();
    
    static AudioListener* GetInstance();
protected:
private:
    std::queue<AudioEvent*>* eventQueue;
    
    static AudioListener* INSTANCE;
    
    AudioListener();
};

#endif /* AudioListener_hpp */
