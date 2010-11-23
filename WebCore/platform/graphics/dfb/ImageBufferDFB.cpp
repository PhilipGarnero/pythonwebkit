/*
 * Copyright (C) 2009-2011 Denis Oliver Kropp <dok@directfb.org>
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
#include "ImageBuffer.h"
#include "ImageBufferData.h"

#include "GraphicsContext.h"
#include "ImageData.h"
#include "NotImplemented.h"

#include "Globals.h"
#include "StillImageDFB.h"

extern "C" {
#include <direct/debug.h>
#include <direct/messages.h>
}


D_DEBUG_DOMAIN( WebKit_ImageBuffer, "WebKit/ImageBuffer", "WebKit ImageBuffer" );

/**********************************************************************************************************************/

namespace WebCore {

ImageBufferData::ImageBufferData(const IntSize& size)
{
     D_DEBUG_AT( WebKit_ImageBuffer, "ImageBufferData::%s( size %dx%d )\n", __func__, size.width(), size.height() );
}

ImageBuffer::ImageBuffer(const IntSize& size, ColorSpace colorSpace, bool& success)
    : m_data(size)
{
     D_DEBUG_AT( WebKit_ImageBuffer, "ImageBuffer::%s( size %dx%d, space %d )\n", __func__, size.width(), size.height(), colorSpace );

     success = false;


     DFBResult             ret;
     DFBSurfaceDescription desc;

     desc.flags       = (DFBSurfaceDescriptionFlags)( DSDESC_WIDTH | DSDESC_HEIGHT | DSDESC_PIXELFORMAT );
     desc.width       = size.width();
     desc.height      = size.height();
     desc.pixelformat = DSPF_ARGB;

     ret = Globals::dfb->CreateSurface( Globals::dfb, &desc, &m_data.m_surface );
     if (ret) {
          D_DERROR( (DirectResult) ret, "WebKit/ImageBuffer: CreateSurface( %dx%d ARGB ) failed!\n", desc.width, desc.height );
          return;
     }

     m_data.m_desc = desc;

     m_context.set( new GraphicsContext( m_data.m_surface ) );

     success = true;
}

ImageBuffer::~ImageBuffer()
{
     D_DEBUG_AT( WebKit_ImageBuffer, "ImageBuffer::%s()\n", __func__ );

     m_data.m_surface->Release( m_data.m_surface );
}

GraphicsContext* ImageBuffer::context() const
{
     D_DEBUG_AT( WebKit_ImageBuffer, "ImageBuffer::%s()\n", __func__ );

     return m_context.get();
}

bool ImageBuffer::drawsUsingCopy() const
{
     D_DEBUG_AT( WebKit_ImageBuffer, "ImageBuffer::%s()\n", __func__ );

     return false;
}

PassRefPtr<Image> ImageBuffer::copyImage() const
{
     D_DEBUG_AT( WebKit_ImageBuffer, "ImageBuffer::%s()\n", __func__ );

     DFBResult         ret;
     IDirectFBSurface *surface;

     ret = Globals::dfb->CreateSurface( Globals::dfb, &m_data.m_desc, &surface );
     if (ret) {
          D_DERROR( (DirectResult) ret, "WebKit/ImageBuffer: CreateSurface( %dx%d ARGB ) failed!\n",
                    m_data.m_desc.width, m_data.m_desc.height );
          return RefPtr<Image>();
     }

     surface->Blit( surface, m_data.m_surface, NULL, 0, 0 );

     RefPtr<StillImage> result = StillImage::create( surface );

     surface->Release( surface );

     return result;
}

void ImageBuffer::clip(GraphicsContext*, const FloatRect&) const
{
     D_DEBUG_AT( WebKit_ImageBuffer, "ImageBuffer::%s()\n", __func__ );

     D_UNIMPLEMENTED();
     // See https://bugs.webkit.org/show_bug.cgi?id=23526 for why this is unimplemented.
}

void ImageBuffer::draw(GraphicsContext* context, ColorSpace styleColorSpace, const FloatRect& destRect, const FloatRect& srcRect,
                       CompositeOperator op , bool useLowQualityScale)
{
     D_DEBUG_AT( WebKit_ImageBuffer, "ImageBuffer::%s()\n", __func__ );

     RefPtr<Image> image = StillImage::create( m_data.m_surface );

     context->drawImage(image.get(), styleColorSpace, destRect, srcRect, op, useLowQualityScale);
}

void ImageBuffer::drawPattern(GraphicsContext* context, const FloatRect& srcRect, const AffineTransform& patternTransform,
                              const FloatPoint& phase, ColorSpace styleColorSpace, CompositeOperator op, const FloatRect& destRect)
{
     D_DEBUG_AT( WebKit_ImageBuffer, "ImageBuffer::%s()\n", __func__ );

     RefPtr<Image> image = StillImage::create( m_data.m_surface );

     image->drawPattern(context, srcRect, patternTransform, phase, styleColorSpace, op, destRect);
}

PassRefPtr<ImageData> ImageBuffer::getUnmultipliedImageData(const IntRect& rect) const
{
     D_DEBUG_AT( WebKit_ImageBuffer, "ImageBuffer::%s()\n", __func__ );

     D_UNIMPLEMENTED();

     return 0;
}

PassRefPtr<ImageData> ImageBuffer::getPremultipliedImageData(const IntRect& rect) const
{
     D_DEBUG_AT( WebKit_ImageBuffer, "ImageBuffer::%s()\n", __func__ );

     D_UNIMPLEMENTED();

     return 0;
}

void ImageBuffer::putUnmultipliedImageData(ImageData* source, const IntRect& sourceRect, const IntPoint& destPoint)
{
     D_DEBUG_AT( WebKit_ImageBuffer, "ImageBuffer::%s()\n", __func__ );

     D_UNIMPLEMENTED();
}

void ImageBuffer::putPremultipliedImageData(ImageData* source, const IntRect& sourceRect, const IntPoint& destPoint)
{
     D_DEBUG_AT( WebKit_ImageBuffer, "ImageBuffer::%s()\n", __func__ );

     D_UNIMPLEMENTED();
}

String ImageBuffer::toDataURL(const String&, const double*) const
{
     D_DEBUG_AT( WebKit_ImageBuffer, "ImageBuffer::%s()\n", __func__ );

     D_UNIMPLEMENTED();

     return String();
}

void ImageBuffer::platformTransformColorSpace(const Vector<int>& lookUpTable)
{
     D_DEBUG_AT( WebKit_ImageBuffer, "ImageBuffer::%s()\n", __func__ );

     D_UNIMPLEMENTED();
}

} // namespace WebCore

