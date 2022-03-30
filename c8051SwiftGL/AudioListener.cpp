//
//  AudioListener.cpp
//  c8051SwiftGL
//
//  Created by Thomas on 2022-03-29.
//

#include "AudioListener.hpp"

AudioEvent::AudioEvent(int trackReference):track(trackReference) {}

AudioEvent::~AudioEvent() {}

int AudioEvent::getTrack() { return track; }

AudioListener::AudioListener():eventQueue(new std::queue<AudioEvent*>()) {}

AudioListener::~AudioListener() { if(eventQueue) delete eventQueue; }

void AudioListener::addAudioEvent(AudioEvent *event)
{
    if(event)
        eventQueue->push(event);
}

AudioEvent* AudioListener::getNextAudioEvent()
{
    AudioEvent* event = 0;
    if(!eventQueue->empty())
    {
        event = eventQueue->front();
        eventQueue->pop();
    }
    return event;
}

AudioListener* AudioListener::GetInstance()
{
    if(!INSTANCE)
        INSTANCE = new AudioListener();
    return INSTANCE;
}
