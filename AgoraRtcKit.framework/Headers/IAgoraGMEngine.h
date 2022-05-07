//
//  AgoraRtcEngine SDK
//
//  Copyright (c) 2019 Agora.io. All rights reserved.
//

/**
 @defgroup createAgoraGameMediaEngine Create an GameMediaEngine
 */

#ifndef AGORA_GAME_MEDIA_ENGINE_H
#define AGORA_GAME_MEDIA_ENGINE_H

#include <stdint.h>
#include "IAgoraRtcEngine.h"

namespace agora {
namespace rtc {
/** connection state of Game Media Engine
 */
enum GME_CONNECTION_STATE_TYPE {
  /* The SDK is connecting to the game server. */
  GME_CONNECTION_STATE_CONNECTING = 0,
  /* The SDK is connected to the game server. */
  GME_CONNECTION_STATE_CONNECTED,
  /* The SDK is disconnected from the game server. */
  GME_CONNECTION_STATE_DISCONNECTED
};

/** reason of connection state change of GME
 */
enum GME_CONNECTION_CHANGED_REASON_TYPE {
  /* The connection state is changed. */
  GME_CONNECTION_CHANGED_DEFAULT = 0,
  /* The SDK is connecting to the game server. */
  GME_CONNECTION_CHANGED_CONNECTING,
  /* The SDK fails to create the game room. */
  GME_CONNECTION_CHANGED_CREATE_ROOM_FAIL,
  /* The SDK is disconnected from the Agora RTM system. */
  GME_CONNECTION_CHANGED_RTM_DISCONNECT,
  /* The SDK is kicked out of the Agora RTM system. */
  GME_CONNECTION_CHANGED_RTM_ABORTED
};

enum SPACIAL_AUDIO_SYNC_MODE { SPACIAL_AUDIO_SYNC_MODE_SERVER = 0, SPACIAL_AUDIO_SYNC_MODE_LOCAL = 1 };

/** The IGMEngineEventHandler class enables callbacks to your application.
 */
class IGMEngineEventHandler {
 public:
  virtual ~IGMEngineEventHandler() {}
  /**
   * Triggered when the current token will expire.
   *
   * This callback notifies the app to renew the token. The app should retrieve a new token from
   * the server and call renewToken to pass the new token to the SDK. Failure to do so results in
   * the SDK disconnecting from the server.
   */
  virtual void onRequestToken() {}
  /**
   * Occurs when the player enters a specified game room.
   *
   * This callback is triggered after `IGameMediaEngine::enterRoom`
   * was called and the connection was setup with server successfully.
   */
  virtual void onEnterRoomSuccess() {}
  /**
   * Occurs when the player fails to enter a specified game room.
   *
   * This callback is triggered after `IGameMediaEngine::enterRoom`
   * was called and the connection cannot be setup with server. For detailed connection state, use the
   * `onConnectionStateChange` callback
   */
  virtual void onEnterRoomFail() {}
  /**
   * Occurs when the connection state between the SDK and the server changes.
   *
   * @param state The current connection state. See #GME_CONNECTION_STATE_TYPE for all connection states.
   * @param reason The reason causing the change of the connection state. See #GME_CONNECTION_CHANGED_REASON_TYPE
   * for all reasons.
   */
  virtual void onConnectionStateChange(GME_CONNECTION_STATE_TYPE state, GME_CONNECTION_CHANGED_REASON_TYPE reason) {}
  /**
   * Occurs when the synchronization between the SDK and the game server is interrupted.
   *
   * This callback is triggered when the SDK does not receive any data from the game server for more than 4 seconds.
   *
   * @param lostSyncTimeMs The time elapsed (ms) from when the SDK last received data from the
   * game server to when this callback is triggered.
   */
  virtual void onLostSync(int64_t lostSyncTimeMs) {}
  /**
   * Occurs when the synchronization between the SDK and the game server is established or restored.
   *
   * This callback is triggered when the SDK receives the first data or resumes receiving data from the game server.
   *
   * @param lostSyncTimeMs The Unix timestamp (ms) of when the first data is received or the time elapsed (ms)
   * from when the SDK lost synchronization with the game server to when this callback is triggered.
   */
  virtual void onGetSync(int64_t lostSyncTimeMs) {}
  /**
   * Occurs when the number of players in the team changes.
   *
   * The callback reports the current number of players on the team and their user IDs.
   *
   * @param uids The list of user IDs of all current players in the team.
   * @param userCount The total number of players in the team.
   */
  virtual void onTeamMatesChange(const unsigned int* uids, int userCount) {}
};

/** The definition of GMEngineContext
 */
struct GMEngineContext {
  /*The reference to \ref IRtcEngine, which is the base interface class of the Agora RTC SDK and provides
   * the real-time audio and video communication functionality.
   */
  agora::rtc::IRtcEngine* rtcEngine;
  /** The SDK uses the eventHandler interface class to send callbacks to the app.
   */
  IGMEngineEventHandler* eventHandler;
  /** The App ID must be the same App ID used for initializing the IRtcEngine object.
   */
  const char* appId;
  /**
   * The region for connection. This advanced feature applies to scenarios that have regional restrictions.
   *
   * For the regions that Agora supports, see #AREA_CODE. The area codes support bitwise operation.
   *
   * After specifying the region, the SDK connects to the Agora servers within that region.
   */
  unsigned int areaCode;
  GMEngineContext() : rtcEngine(NULL), eventHandler(NULL), appId(NULL), areaCode(AREA_CODE_CN) {}
};

/** The IGameMediaEngine class provides the main methods that can be invoked by your application.
 */
class IGameMediaEngine {
 protected:
  virtual ~IGameMediaEngine() {}

 public:
  enum AUDIO_FRAME_TYPE {
    /** 0: PCM16. */
    FRAME_TYPE_PCM16 = 0,  // PCM 16bit little endian
  };
  /**
   * Initializes the IGameMediaEngine object and allocates the internal resources.
   *
   * @note Ensure that you call createAgoraGameMediaEngine and initialize before calling any other IGameMediaEngine APIs.
   *
   * @param ctx The pointer to the Agora Game Multimedia Engine context. See #GMEngineContext.
   *
   * @return
   * - 0: Success.
   * - <0: Failure.
   */
  virtual int initialize(const GMEngineContext& ctx) = 0;
  /**
   * Releases all the resources occupied by the IGameMediaEngine object.
   */
  virtual void release() = 0;
  virtual int addEventHandler(IGMEngineEventHandler* eh) = 0;
  virtual int removeEventHandler(IGMEngineEventHandler* eh) = 0;
  /**
   * This method enables the spatial sound effects.
   *
   * @note
   * - You can call this method either before or after calling enterRoom.
   * - Make sure `IRtcEngine::enableSoundPositionIndication` was called to use spacial audio feature.
   *
   * @param enable Whether to enable the spatial sound effects for the players within a specified audio range:
   * - true: Enable the spatial sound effects.
   * - false: Disable the spatial sound effects.

   * @param applyToTeam Whether to enable the spatial sound effects for the players in a team:
   * - true: Enable the spatial sound effects in a team.
   * - false: Disable the spatial sound effects in a team.
   *
   * @return
   * - 0: Success.
   * - <0: Failure.
   */
  virtual int enableSpatializer(bool enable, bool applyToTeam) = 0;
  /**
   * Sets the team ID of a player.
   *
   * The team ID setting affects the sound reachability between players in a game room as follows:
   * - The players with the same team ID are teammates. They can always hear each other, regardless of the audio mode and audio reception range.
   * - The players with different team IDs are opponents. Whether they can hear each other depends on the audio mode and
   *    audio reception range settings. For details, see Sound reachability between players.
   *
   * @note Call this method before calling `IGameMediaEngine::enterRoom`. If you call
   * this method after calling enterRoom, the SDK returns the error code -1.
   *
   * @param teamID The ID of a team. The value must be greater than 0. Setting the parameter to 0 means not teaming up with other players.
   * @return
   * - 0: Success.
   * - <0: Failure.
   */
  virtual int setRangeAudioTeamID(int teamID) = 0;
  /**
   * Sets the audio mode of a player.
   *
   * @note You can call this method either before or after calling enterRoom:
   * - Calling this method before enterRoom affects the audio mode the next time the player enters the room.
   * - Calling this method after enterRoom changes the current audio mode of the player.
   *
   * @param rangeAudioMode The audio mode:
   * - 0 : Everyone mode, in which the player can be heard by other players in the same room.
   * - 1: Team-only mode, in which only teammates can hear each other.
   * @return
   * - 0: Success.
   * - <0: Failure.
   */
  virtual int setRangeAudioMode(int rangeAudioMode) = 0;
  /**
   * This method sets the maximum number of streams that a player can receive in a
   * specified audio reception range.
   *
   * @note You can call this method either before or after calling enterRoom:
   * - Calling this method before enterRoom affects the maximum number of received streams
   *   the next time the player enters a room.
   * - Calling this method after entering a room changes the current maximum number of
   *   received streams of the player.
   *
   * @param maxCount The maximum number of streams that a player can receive within
   * a specified audio reception range. If the number of receivable streams exceeds
   * the set value, the SDK receives the set number of streams closest to the player.
   *
   * @return
   * - 0: Success.
   * - <0: Failure.
   */
  virtual int setMaxHearAudioCount(int maxCount) = 0;
  /**
   * This method sets the audio reception range. The unit of the audio reception range
   * is the same as the unit of distance between game engines.
   *
   * @note You can call this method either before or after calling enterRoom.
   * During the game, you can call it multiple times to update the audio reception range.
   * To implement the range audio and spatial sound effects, you must use this method together
   * with `updateSelfPosition`.
   *
   * @param range The maximum audio reception range, in the unit of game engine distance.
   *
   * @return
   * - 0: Success.
   * - <0: Failure.
   */
  virtual int setAudioRecvRange(int range) = 0;
  /**
   * This method sets the distance unit of game world.
   *
   * @note You can call this method either before or after calling enterRoom.
   * During the game, you can call it multiple times to update distance unit.
   *
   * @param unit The number of meters in real world, which is equal to 1 in game world.
   * Default value is 1. It must not be zero.
   *
   * @return
   * - 0: Success.
   * - <0: Failure.
   */
  virtual int setDistanceUnit(float unit) = 0;
  /**
   * This method updates the sound source position of a player to the server. The server uses
   * the players' world coordinates and audio reception range to determine whether they are
   * within each other's specified audio reception range.
   *
   * @note
   * - Call the method after calling joinChannel .
   * - The call frequency is determined by the app. Agora recommends calling this method every
   *   50 to 2000 ms. Otherwise, the SDK may lose synchronization with the server.
   * - To use the range audio feature, you must call this method at least once after calling enterRoom.
   *
   * @param position The sound position of the player. The coordinate order is forward, right, and up.
   * @param axisForward The unit vector in the direction of the forward axis in the coordinate system.
   * @param axisRight The unit vector in the direction of the right axis in the coordinate system.
   * @param axisUp The unit vector in the direction of the up axis in the coordinate system.
   * @return
   * - 0: Success.
   * - <0: Failure.
   */
  virtual int updateSelfPosition(int position[3], float axisForward[3], float axisRight[3], float axisUp[3]) = 0;
  virtual int updateRemotePosition(unsigned int uid, int position[3]) = 0;
  virtual int setParameters(const char* params) = 0;

  /**
   * Enters a game room.
   *
   * @note
   * - The range audio and spatial sound effects take effect after the method call of enterRoom.
   * - Call this method after calling `IRtcEngine::joinChannel`.
   *
   * @param token The token used for authentication. Ensure that the token used for entering
   * the room is the same token used in joinChannel.
   *
   * @param roomName The room name, which must be the same as the channelName used in joinChannel.
   * @param uid The user ID. The uid must be unique and not be set to 0.
   * @param mode The spacial audio compution mode.
   * @return
   * - 0: Success.
   * - <0: Failure.
   */
  virtual int enterRoom(const char* token, const char* roomName, unsigned int uid, SPACIAL_AUDIO_SYNC_MODE mode) = 0;
  virtual bool isRoomEntered() = 0;
  /**
   * Renews the token when the current token expires.
   *
   * The app should retrieve a new token from the server and call this method to pass the new token
   * to the SDK. Otherwise, the SDK disconnects from the server when the token expires.
   * You can use this method to replace renewToken of the IRtcEngine class.
   *
   * @param token The new token. When generating the new token, ensure the following:
   * - Generate the new token in the same way as you generate the token for `IRtcEngine::joinChannel`.
   * - Add the permission to log into the Agora RTM system when generating Agora RTC token:
   *    builder.addPrivilege(AccessToken.Privileges.kRtmLogin, privilegeTs);
   * @return
   * - 0: Success.
   * - <0: Failure.
   */
  virtual int renewToken(const char* token) = 0;
  /**
   * Exits a game room.
   *
   * @return 0: Success.
   */
  virtual int exitRoom() = 0;

  virtual int enableMic(bool enable) = 0;
  virtual int enableSpeaker(bool enable) = 0;
  virtual int setBlacklist(const uid_t* uids, int uidCount) = 0;
};

}  // namespace rtc
}  // namespace agora

/**
 * Creates an IGameMediaEngine object and returns the pointer.
 *
 * @note The SDK supports creating only one IGameMediaEngine object for an app.
 *
 * @return The pointer to the IGameMediaEngine object.
 */
AGORA_API agora::rtc::IGameMediaEngine* AGORA_CALL createAgoraGameMediaEngine();

#endif
