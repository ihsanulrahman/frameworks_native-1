/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ANDROID_OVERLAY_H
#define ANDROID_OVERLAY_H

#include <stdint.h>
#include <sys/types.h>

#include <utils/Errors.h>
#include <utils/IInterface.h>
#include <utils/RefBase.h>
#include <utils/threads.h>

#include <ui/PixelFormat.h>
#include <ui/IOverlay.h>

#include <hardware/overlay.h>

namespace android {

class IMemory;
class IMemoryHeap;

// ----------------------------------------------------------------------------

class OverlayRef : public LightRefBase<OverlayRef>
{
public:
    OverlayRef(overlay_handle_t const*, const sp<IOverlay>&,
            uint32_t w, uint32_t h, int32_t f, uint32_t ws, uint32_t hs);

    static sp<OverlayRef> readFromParcel(const Parcel& data);
    static status_t writeToParcel(Parcel* reply, const sp<OverlayRef>& o);    

private:
    friend class LightRefBase<OverlayRef>;
    friend class Overlay;

    OverlayRef();
    virtual ~OverlayRef();

    overlay_handle_t const *mOverlayHandle;
    sp<IOverlay> mOverlayChanel;
    uint32_t mWidth;
    uint32_t mHeight;
    int32_t  mFormat;
    int32_t  mWidthStride;
    int32_t  mHeightStride;
    bool mOwnHandle;
};

// ----------------------------------------------------------------------------

class Overlay : public virtual RefBase
{
public:
    Overlay(const sp<OverlayRef>& overlayRef);

    /* destroys this overlay */
    void destroy();
    
    /* get the HAL handle for this overlay */
    overlay_handle_t const* getHandleRef() const;

    /* blocks until an overlay buffer is available and return that buffer. */
    overlay_buffer_t dequeueBuffer();

    /* release the overlay buffer and post it */
    int queueBuffer(overlay_buffer_t buffer);

    /* returns the address of a given buffer if supported, NULL otherwise. */
    void* getBufferAddress(overlay_buffer_t buffer);

    /* get physical informations about the overlay */
    uint32_t getWidth() const;
    uint32_t getHeight() const;
    int32_t getFormat() const;
    int32_t getWidthStride() const;
    int32_t getHeightStride() const;
    status_t getStatus() const;
    
private:
    virtual ~Overlay();

    sp<OverlayRef> mOverlayRef;
    overlay_data_device_t *mOverlayData;
    status_t mStatus;
};

// ----------------------------------------------------------------------------

}; // namespace android

#endif // ANDROID_OVERLAY_H
