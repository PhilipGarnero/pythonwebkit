/*
 * Copyright (C) 2010 Stephan Aßmus <superstippi@gmx.de>
 *
 * All rights reserved.
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
#include "StillImageDFB.h"

#include "GraphicsContext.h"
#include "IntSize.h"

extern "C" {
#include <direct/debug.h>
#include <direct/messages.h>
}


D_DEBUG_DOMAIN( WebKit_StillImage, "WebKit/StillImage", "WebKit StillImage" );

/**********************************************************************************************************************/

namespace WebCore {

StillImage::StillImage( IDirectFBSurface *surface )
    : m_surface(surface)
{
     D_DEBUG_AT( WebKit_StillImage, "StillImage::%s( surface %p )\n", __func__, surface );

     D_ASSERT( m_surface != NULL );

     m_surface->AddRef( m_surface );

     m_surface->GetSize( m_surface, &m_size.w, &m_size.h );
     m_surface->GetPixelFormat( m_surface, &m_format );

     D_DEBUG_AT( WebKit_StillImage, "  -> size %dx%d, format 0x%08x\n", m_size.w, m_size.h, m_format );
}

StillImage::~StillImage()
{
     D_DEBUG_AT( WebKit_StillImage, "StillImage::%s() <- m_surface %p\n", __func__, m_surface );

     D_ASSERT( m_surface != NULL );

     m_surface->Release( m_surface );
}

void
StillImage::destroyDecodedData(bool destroyAll)
{
     D_DEBUG_AT( WebKit_StillImage, "StillImage::%s() <- m_surface %p\n", __func__, m_surface );
}

unsigned int
StillImage::decodedSize() const
{
     D_DEBUG_AT( WebKit_StillImage, "StillImage::%s() <- m_surface %p\n", __func__, m_surface );

     return m_size.w * m_size.h * DFB_BYTES_PER_PIXEL( m_format );
}

IntSize
StillImage::size() const
{
     D_DEBUG_AT( WebKit_StillImage, "StillImage::%s() <- m_surface %p\n", __func__, m_surface );

     return IntSize( m_size.w, m_size.h );
}

NativeImagePtr
StillImage::nativeImageForCurrentFrame()
{
     D_DEBUG_AT( WebKit_StillImage, "StillImage::%s() <- m_surface %p\n", __func__, m_surface );

     D_ASSERT( m_surface != NULL );

     return m_surface;
}

void StillImage::draw(GraphicsContext* context, const FloatRect& destRect,
                      const FloatRect& sourceRect, ColorSpace colorSpace, CompositeOperator op)
{
     D_DEBUG_AT( WebKit_StillImage, "StillImage::%s( context %p ) <- m_surface %p\n", __func__, context, m_surface );
     D_DEBUG_AT( WebKit_StillImage, "  -> dstRect    %7.2f,%7.2f-%7.2fx%7.2f\n", destRect.x(), destRect.y(), destRect.width(), destRect.height() );
     D_DEBUG_AT( WebKit_StillImage, "  -> srcRect    %7.2f,%7.2f-%7.2fx%7.2f\n", sourceRect.x(), sourceRect.y(), sourceRect.width(), sourceRect.height() );
     D_DEBUG_AT( WebKit_StillImage, "  -> operator   %d\n", op );

     D_ASSERT( m_surface != NULL );

     IDirectFBSurface *destSurface = context->platformContext();

     DFBRectangle dstRect = { destRect.x(), destRect.y(), destRect.width(), destRect.height() };
     DFBRectangle srcRect = { sourceRect.x(), sourceRect.y(), sourceRect.width(), sourceRect.height() };

     context->save();
     context->setCompositeOperation(op);

     destSurface->StretchBlit( destSurface, m_surface, &srcRect, &dstRect );

     context->restore();
}

}
