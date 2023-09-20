/*
 *  Copyright 2012 The WebRTC Project Authors. All rights reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef EXAMPLES_WEBRTC_API_H_
#define EXAMPLES_WEBRTC_API_H_
extern "C" {
#define WEBRTCAPI2 _declspec(dllexport)

typedef void (*VideoFrameCb)(void* arg, const char* buffer, int format, int width, int height);
typedef void (*SignalMessageCb)(void* arg, const char* msg);

typedef struct
{
    VideoFrameCb onLocalVideoFrame;
    VideoFrameCb onRemoteVideoFrame;
    SignalMessageCb onSignalMessage;
} PeerConnectionApiCallback;

typedef struct
{
    PeerConnectionApiCallback callback;
    void* callbackArg;
} PeerConnectionApiOptions;

WEBRTCAPI2 void* createPeerConnection(PeerConnectionApiOptions* opts);
WEBRTCAPI2 void startPeerConnection(void* pc);
WEBRTCAPI2 void stopPeerConnection(void* pc);
WEBRTCAPI2 void handleSignalMessage(void* pc, const char* msg);
WEBRTCAPI2 void destoryPeerConnection(void* pc);
WEBRTCAPI2 int getPeerConnectionStatus(void* pc);
}

#endif  // EXAMPLES_WEBRTC_API_H_
