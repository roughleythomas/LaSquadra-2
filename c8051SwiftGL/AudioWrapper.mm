//
//  AudioWrapper.m
//  c8051SwiftGL
//
//  Created by Thomas on 2022-03-30.
//

#import "AudioWrapper.h"
#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import "AudioListener.hpp"

@interface AudioWrapper()
{
    AudioListener* listener;
    AVAudioEngine* engine;
    AVAudioPlayerNode *playerNode;
    AVAudioFile* audioFile;
}
@end

@implementation AudioWrapper

-(void) setup
{
    listener = AudioListener::GetInstance();
    [[AVAudioSession sharedInstance] setCategory:AVAudioSessionCategoryPlayback error:nil];
    [[AVAudioSession sharedInstance] overrideOutputAudioPort:AVAudioSessionPortOverrideSpeaker error:nil];
    [[AVAudioSession sharedInstance] setActive:YES error:nil];
    engine = [[AVAudioEngine alloc] init];
    playerNode = [[AVAudioPlayerNode alloc] init];
    NSURL *url = [[NSBundle mainBundle] URLForResource:@"test" withExtension:@"wav"];
    audioFile = [[AVAudioFile alloc] initForReading:url error:nil];
    
    [engine connect:playerNode to:engine.mainMixerNode format:nil];
    [engine prepare];
}

-(void) dealloc
{
    [playerNode stop];
    [engine stop];
}

-(void) update
{
    AudioEvent* event = listener->getNextAudioEvent();
    while(event != 0)
    {
        [playerNode scheduleFile:audioFile atTime:0 completionHandler:nil];
        [playerNode play];
    }
}

@end
