//
//  AgoraGmeKit.h
//  AgoraRtcKit
//
//  Copyright (c) 2018 Agora. All rights reserved.
//

#ifndef AgoraGmeKit_h
#define AgoraGmeKit_h

#import <Foundation/Foundation.h>
#import "AgoraEnumerates.h"

typedef NS_ENUM(NSInteger, AgoraGMEAudioMode) { AgoraGMEAudioModeWorld = 0, AgoraGMEAudioModeTeam = 1 };

typedef NS_ENUM(NSInteger, AgoraGMEConnectionState) {
  AgoraGMEConnectionStateConnecting = 0,
  AgoraGMEConnectionStateConnected = 1,
  AgoraGMEConnectionStateDisconnected = 2,
};

typedef NS_ENUM(NSInteger, AgoraGMEConnectionChangedReason) {
  AgoraGMEConnectionChangedReasonDefault = 0,
  AgoraGMEConnectionChangedReasonConnecting = 1,
  AgoraGMEConnectionChangedReasonCreateRoomFail = 2,
  AgoraGMEConnectionChangedReasonRtmDisconnect = 3,
  AgoraGMEConnectionChangedReasonAborted = 4,
};

typedef NS_ENUM(NSInteger, AgoraGMESpacialAudioSyncMode) {
  AgoraGMESpacialAudioSyncModeServer = 0,
  AgoraGMESpacialAudioSyncModeLocal = 1,
};

@class AgoraRtcEngineKit, AgoraGmeKit;

__attribute__((visibility("default"))) @interface AgoraGMEngineConfig : NSObject
@property(nonatomic, assign) AgoraRtcEngineKit* rtcEngine;
/** The App ID issued to you by Agora. See [How to get the App ID](https://docs.agora.io/en/Agora%20Platform/token#get-an-app-id). Only users in apps with the same App ID can join the same channel and communicate with each other. Use an App ID to create only one AgoraRtcEngineKit instance.  To change your App ID, call [destroy]([AgoraRtcEngineKit destroy]) to `destroy` the current AgoraRtcEngineKit instance, and after `destroy` returns 0, call [sharedEngineWithConfig]([AgoraRtcEngineKit sharedEngineWithConfig:delegate:]) to create an AgoraRtcEngineKit instance with the new App ID.
 */
@property(copy, nonatomic) NSString* _Nullable appId;
/** The region for connection. This advanced feature applies to scenarios that have regional restrictions. <p>For the regions that Agora supports, see AgoraAreaCode. The area codes support bitwise operation. After specifying the region, the SDK connects to the Agora servers within that region.</p>
 */
@property(nonatomic, assign) NSUInteger areaCode;
@end

@protocol AgoraGMEngineDelegate <NSObject>

- (void)gmEngine:(AgoraGmeKit* _Nonnull)engine didOccurError:(AgoraErrorCode)errorCode;

- (void)gmEngineDidRequestToken:(AgoraGmeKit* _Nonnull)engine;

- (void)gmEngineDidEnterRoom:(AgoraGmeKit* _Nonnull)engine;

- (void)gmEngineDidFailedToEnterRoom:(AgoraGmeKit* _Nonnull)engine;

- (void)gmEngine:(AgoraGmeKit* _Nonnull)engine didLostSynchronizationWithTimeInterval:(NSTimeInterval)lostSynchronizationTimeInterval;

- (void)gmEngine:(AgoraGmeKit* _Nonnull)engine didGetSynchronizedWithTimeInterval:(NSTimeInterval)lostSynchronizationTimeInterval;

- (void)gmEngine:(AgoraGmeKit* _Nonnull)engine teamMatesDidChangedWithUsers:(NSArray<NSNumber*>* _Nonnull)users;

- (void)gmEngine:(AgoraGmeKit* _Nonnull)engine connectionDidChangedToState:(AgoraGMEConnectionState)state withReason:(AgoraGMEConnectionChangedReason)reason;
@end

__attribute__((visibility("default"))) @interface AgoraGmeKit : NSObject

+ (instancetype _Nonnull)sharedGMEngineWithConfig:(AgoraGMEngineConfig* _Nonnull)config delegate:(id<AgoraGMEngineDelegate> _Nullable)delegate;

- (int)enableSpatializer:(BOOL)enable applyToTeam:(BOOL)applyToTeam;

- (int)setRangeAudioTeamId:(NSInteger)teamId;

- (int)setRangeAudioMode:(AgoraGMEAudioMode)audioModel;

- (int)setMaxHearAudioCount:(NSUInteger)maxCount;

- (int)setAudioRecvRange:(NSUInteger)recvRange;

- (int)setDistanceUnit:(float)unit;

- (int)updateSelfPosition:(NSArray<NSNumber*>* _Nonnull)selfPosition axisForward:(NSArray<NSNumber*>* _Nullable)axisForward axisRight:(NSArray<NSNumber*>* _Nullable)axisRight axisUp:(NSArray<NSNumber*>* _Nullable)axisUp;

- (int)updateRemotePosition:(NSUInteger)userId position:(NSArray<NSNumber*>* _Nullable)pos;

- (int)setParameters:(NSString* _Nonnull)parameters;

- (int)enterRoomByToken:(NSString* _Nullable)token roomName:(NSString* _Nonnull)roomName userId:(NSUInteger)userId syncMode:(AgoraGMESpacialAudioSyncMode)syncMode;

- (BOOL)isRoomEntered;

- (int)renewToken:(NSString* _Nonnull)token;

- (int)exitRoom;

- (int)enableMic:(BOOL)enable;

- (int)enableSpeaker:(BOOL)enable;

- (int)setBlacklist:(NSArray<NSNumber*>* _Nonnull)blacklist;

- (void)enableMainQueueDispatch:(BOOL)enabled;

@end

#endif /* AgoraGmeKit_h */
