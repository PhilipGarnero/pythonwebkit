/*
 * Copyright (C) 2007 Ryan Leavengood <leavengood@gmail.com>
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
#include "Path.h"

#include "FloatRect.h"
#include "NotImplemented.h"
#include "PlatformString.h"

#include "DFBPath.h"

#include <directfb.h>



extern "C" {
#include <direct/debug.h>
#include <direct/messages.h>
}


D_DEBUG_DOMAIN( WebKit_Path, "WebKit/Path", "WebKit Path" );

/**********************************************************************************************************************/

namespace WebCore {

Path::Path()
    : m_path(new DFBPath)
{
     D_DEBUG_AT( WebKit_Path, "Path::%s()\n", __func__ );
}

Path::~Path()
{
     D_DEBUG_AT( WebKit_Path, "Path::%s()\n", __func__ );

     delete m_path;
}

Path::Path(const Path& other)
    : m_path(other.platformPath())
{
     D_DEBUG_AT( WebKit_Path, "Path::%s( other %p )\n", __func__, &other );
}

Path& Path::operator=(const Path& other)
{
     D_DEBUG_AT( WebKit_Path, "Path::%s( other %p )\n", __func__, &other );

    if (&other != this)
        m_path = other.platformPath();

    return *this;
}

bool Path::hasCurrentPoint() const
{
     D_DEBUG_AT( WebKit_Path, "Path::%s()\n", __func__ );
    return !isEmpty();
}

FloatPoint Path::currentPoint() const 
{
     D_DEBUG_AT( WebKit_Path, "Path::%s()\n", __func__ );
    // FIXME: implement safe way to return current point of subpath.
    D_UNIMPLEMENTED();
    float quietNaN = std::numeric_limits<float>::quiet_NaN();
    return FloatPoint(quietNaN, quietNaN);
}

bool Path::contains(const FloatPoint& point, WindRule rule) const
{
     D_DEBUG_AT( WebKit_Path, "Path::%s()\n", __func__ );
    return m_path->Contains(point);
}

bool Path::strokeContains(StrokeStyleApplier* applier, const FloatPoint& point) const
{
     D_DEBUG_AT( WebKit_Path, "Path::%s()\n", __func__ );
     D_UNIMPLEMENTED();
     return false;
/*
     ASSERT(applier);

    CGContextRef context = scratchContext();

    CGContextSaveGState(context);
    CGContextBeginPath(context);
    CGContextAddPath(context, platformPath());

    GraphicsContext gc(context);
    applier->strokeStyle(&gc);

    bool hitSuccess = CGContextPathContainsPoint(context, point, kCGPathStroke);
    CGContextRestoreGState(context);
    
    return hitSuccess;
*/
}

void Path::translate(const FloatSize& size)
{
     D_DEBUG_AT( WebKit_Path, "Path::%s()\n", __func__ );
    D_UNIMPLEMENTED();
}

FloatRect Path::boundingRect() const
{
     D_DEBUG_AT( WebKit_Path, "Path::%s()\n", __func__ );
    return m_path->Bounds();
}

void Path::moveTo(const FloatPoint& point)
{
     D_DEBUG_AT( WebKit_Path, "Path::%s()\n", __func__ );
    // FIXME: Use OffsetBy?
    D_UNIMPLEMENTED();
}

void Path::addLineTo(const FloatPoint& p)
{
     D_DEBUG_AT( WebKit_Path, "Path::%s()\n", __func__ );
    D_UNIMPLEMENTED();
}

void Path::addQuadCurveTo(const FloatPoint& cp, const FloatPoint& p)
{
     D_DEBUG_AT( WebKit_Path, "Path::%s()\n", __func__ );
    D_UNIMPLEMENTED();
}

void Path::addBezierCurveTo(const FloatPoint& cp1, const FloatPoint& cp2, const FloatPoint& p)
{
     D_DEBUG_AT( WebKit_Path, "Path::%s()\n", __func__ );
    D_UNIMPLEMENTED();
}

void Path::addArcTo(const FloatPoint& p1, const FloatPoint& p2, float radius)
{
     D_DEBUG_AT( WebKit_Path, "Path::%s()\n", __func__ );
    D_UNIMPLEMENTED();
}

void Path::closeSubpath()
{
     D_DEBUG_AT( WebKit_Path, "Path::%s()\n", __func__ );
    D_UNIMPLEMENTED();
}

void Path::addArc(const FloatPoint& p, float r, float sar, float ear, bool anticlockwise)
{
     D_DEBUG_AT( WebKit_Path, "Path::%s()\n", __func__ );
    D_UNIMPLEMENTED();
}

void Path::addRect(const FloatRect& r)
{
     D_DEBUG_AT( WebKit_Path, "Path::%s()\n", __func__ );
    m_path->Add(r);
}

void Path::addEllipse(const FloatRect& r)
{
     D_DEBUG_AT( WebKit_Path, "Path::%s()\n", __func__ );
    D_UNIMPLEMENTED();
}

void Path::clear()
{
     D_DEBUG_AT( WebKit_Path, "Path::%s()\n", __func__ );
    m_path->Reset();
}

bool Path::isEmpty() const
{
     D_DEBUG_AT( WebKit_Path, "Path::%s()\n", __func__ );
    return m_path->empty;
}

void Path::apply(void* info, PathApplierFunction function) const
{
     D_DEBUG_AT( WebKit_Path, "Path::%s()\n", __func__ );
    D_UNIMPLEMENTED();
}

void Path::transform(const AffineTransform& transform)
{
     D_DEBUG_AT( WebKit_Path, "Path::%s()\n", __func__ );
    D_UNIMPLEMENTED();
}

FloatRect Path::strokeBoundingRect(StrokeStyleApplier* applier)
{
     D_DEBUG_AT( WebKit_Path, "Path::%s()\n", __func__ );
    D_UNIMPLEMENTED();
    return FloatRect();
}

} // namespace WebCore

