/*
 * Copyright (C) 2006 Apple Computer, Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#include "config.h"
#include "ImageDecoder.h"

#include <directfb.h>

namespace WebCore {

NativeImagePtr RGBA32Buffer::asNewNativeImage() const
{
#if 1
    DFBResult              ret;
    IDirectFB             *dfb;
    IDirectFBSurface      *surface;
    DFBSurfaceDescription  desc;
    DFBRectangle           rect;

    DirectFBCreate( &dfb );

    desc.flags       = (DFBSurfaceDescriptionFlags)( DSDESC_WIDTH | DSDESC_HEIGHT | DSDESC_PIXELFORMAT );
    desc.width       = width();
    desc.height      = height();
    desc.pixelformat = DSPF_ARGB;

    ret = dfb->CreateSurface( dfb, &desc, &surface );
    if (ret) {
        D_DERROR( (DirectResult) ret, "RGBA32Buffer::asNewNativeImage: CreateSurface( %dx%d ) failed!\n", width(), height() );
        return NULL;
    }

    rect.x = 0;
    rect.y = 0;
    rect.w = desc.width;
    rect.h = desc.height;

    ret = surface->Write( surface, &rect, reinterpret_cast<void*>(const_cast<PixelData*>(m_bytes)), rect.w * 4 );
    if (ret) {
        D_DERROR( (DirectResult) ret, "RGBA32Buffer::asNewNativeImage: Surface::Write( %dx%d ) failed!\n", width(), height() );
        surface->Release( surface );
        return NULL;
    }

    dfb->Release( dfb );

    return surface;
#else
    DFBResult              ret;
    IDirectFB             *dfb;
    IDirectFBSurface      *surface;
    DFBSurfaceDescription  desc;
    void                  *data;
    int                    pitch;

    DirectFBCreate( &dfb );

    desc.flags       = (DFBSurfaceDescriptionFlags)( DSDESC_WIDTH | DSDESC_HEIGHT | DSDESC_PIXELFORMAT );
    desc.width       = width();
    desc.height      = height();
    desc.pixelformat = DSPF_ARGB;

    ret = dfb->CreateSurface( dfb, &desc, &surface );
    if (ret) {
        D_DERROR( (DirectResult) ret, "RGBA32Buffer::asNewNativeImage: CreateSurface( %dx%d ) failed!\n", width(), height() );
        return NULL;
    }

    ret = surface->Lock( surface, DSLF_WRITE, &data, &pitch );
    if (ret) {
        D_DERROR( (DirectResult) ret, "RGBA32Buffer::asNewNativeImage: Surface::Lock( %dx%d ) failed!\n", width(), height() );
        surface->Release( surface );
        return NULL;
    }

    for (int y=0; y<height(); y++) {
         u8 *dst = (u8*) data + pitch * y;
         u8 *src = (u8*) m_bytes.data() + width()*4 * y;

         memcpy( dst, src, width()*4 );
    }

    surface->Unlock( surface );

    dfb->Release( dfb );

    return surface;
#endif
}

} // namespace WebCore

