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

#define DIRECT_ENABLE_DEBUG

#include "config.h"
#include "GraphicsContext.h"

#include "CString.h"
#include "Color.h"
#include "Font.h"
#include "FontData.h"
#include "NotImplemented.h"
#include "Path.h"
#include "Pen.h"
#include "TransformationMatrix.h"
#include <directfb.h>
#include <stdio.h>


#include <direct/debug.h>

D_DEBUG_DOMAIN( GraphicsContext_DFB, "WebKit/GraphicsContext", "GraphicsContextDFB" );


namespace WebCore {


class GraphicsStateDFB {
public:
    static const DFBColor defaultColor;

    GraphicsStateDFB() {
        D_DEBUG_AT( GraphicsContext_DFB, "GraphicsContext::%s()\n", __FUNCTION__ );

        m_clip_set     = false;
        m_color_fill   = defaultColor;
        m_color_stroke = defaultColor;
        m_porterduff   = DSPD_SRC;

        m_matrix[0] = 0x10000; m_matrix[1] = 0x00000; m_matrix[2] = 0x00000;
        m_matrix[3] = 0x00000; m_matrix[4] = 0x10000; m_matrix[5] = 0x00000;
        m_matrix[6] = 0x00000; m_matrix[7] = 0x00000; m_matrix[8] = 0x10000;
    }

    ~GraphicsStateDFB() {
        D_DEBUG_AT( GraphicsContext_DFB, "GraphicsContext::%s()\n", __FUNCTION__ );

    }

    void apply( IDirectFBSurface *surface ) {
        applyClip( surface );
        applyPorterBlit( surface );
        applyMatrix( surface );
    }

    void applyClip( IDirectFBSurface *surface ) {
        DFBResult ret;

        ret = surface->SetClip(surface, m_clip_set ? &m_clip : NULL);
        if (ret) {
            D_DERROR( (DirectResult) ret, "GraphicsContextDFB: SetClip( %d,%d-%d,%d ) failed!\n",
                      m_clip.x1, m_clip.y1, m_clip.x2, m_clip.y2 );
        }
    }

    void applyPorterBlit( IDirectFBSurface *surface ) {
        surface->SetPorterDuff(surface, m_porterduff);

        if (m_porterduff == DSPD_SRC)
            surface->SetBlittingFlags( surface, DSBLIT_NOFX );
        else
            surface->SetBlittingFlags( surface, DSBLIT_BLEND_ALPHACHANNEL );
    }

    void applyPorterDraw( IDirectFBSurface *surface, u8 alpha ) {
        if (m_porterduff == DSPD_SRC)
            surface->SetDrawingFlags( surface, DSDRAW_NOFX );
        else
            surface->SetDrawingFlags( surface, alpha != 255 ? DSDRAW_BLEND : DSDRAW_NOFX );
    }

    void applyMatrix( IDirectFBSurface *surface ) {
        if (nonIdentMatrix()) {
            surface->SetMatrix(surface, m_matrix);
            surface->SetRenderOptions(surface, DSRO_MATRIX);
        }
        else
            surface->SetRenderOptions(surface, DSRO_NONE);
    }

    bool nonIdentMatrix() {
        return
            m_matrix[0] != 0x10000 || m_matrix[1] != 0x00000 || m_matrix[2] != 0x00000 ||
            m_matrix[3] != 0x00000 || m_matrix[4] != 0x10000 || m_matrix[5] != 0x00000 ||
            m_matrix[6] != 0x00000 || m_matrix[7] != 0x00000 || m_matrix[8] != 0x10000;
    }

    DFBRegion                m_clip;
    bool                     m_clip_set;

    DFBColor                 m_color_fill;
    DFBColor                 m_color_stroke;

    DFBSurfacePorterDuffRule m_porterduff;

    int                      m_matrix[9];
};

const DFBColor GraphicsStateDFB::defaultColor = { 0, 0, 0, 0 };


class GraphicsContextPlatformPrivate {
public:
    GraphicsContextPlatformPrivate(IDirectFBSurface *surface);
    ~GraphicsContextPlatformPrivate();

    IDirectFBSurface *m_surface;


    GraphicsStateDFB  m_state;

    WTF::Vector<GraphicsStateDFB*> m_saved;
};




GraphicsContextPlatformPrivate::GraphicsContextPlatformPrivate(IDirectFBSurface *surface)
    : m_surface(surface)
{
    if (m_surface != NULL) {
        m_surface->AddRef( m_surface );

        m_state.apply( m_surface );
    }
}

GraphicsContextPlatformPrivate::~GraphicsContextPlatformPrivate()
{
    if (m_surface != NULL)
        m_surface->Release( m_surface );
}

GraphicsContext::GraphicsContext(PlatformGraphicsContext* context)
    : m_common(createGraphicsContextPrivate())
    , m_data(new GraphicsContextPlatformPrivate(context))
{
    D_DEBUG_AT( GraphicsContext_DFB, "GraphicsContext::%s()\n", __FUNCTION__ );

    setPaintingDisabled(!context);
    if (context) {
        // Make sure the context starts in sync with our state.
        setPlatformFillColor(fillColor(), ColorSpaceDeviceRGB);
        setPlatformStrokeColor(strokeColor(), ColorSpaceDeviceRGB);
    }
}

GraphicsContext::~GraphicsContext()
{
    D_DEBUG_AT( GraphicsContext_DFB, "GraphicsContext::%s()\n", __FUNCTION__ );

    destroyGraphicsContextPrivate(m_common);
    delete m_data;
}

PlatformGraphicsContext* GraphicsContext::platformContext() const
{
    D_DEBUG_AT( GraphicsContext_DFB, "GraphicsContext::%s()\n", __FUNCTION__ );
    return m_data->m_surface;
}

void GraphicsContext::savePlatformState()
{
    D_DEBUG_AT( GraphicsContext_DFB, "GraphicsContext::%s()\n", __FUNCTION__ );

    if (paintingDisabled())
        return;

    GraphicsStateDFB *saved = new GraphicsStateDFB( m_data->m_state );

    m_data->m_saved.append( saved );
}

void GraphicsContext::restorePlatformState()
{
    D_DEBUG_AT( GraphicsContext_DFB, "GraphicsContext::%s()\n", __FUNCTION__ );

    if (paintingDisabled())
        return;

    if (m_data->m_saved.size() > 0) {
        GraphicsStateDFB *saved = m_data->m_saved.last();

        m_data->m_saved.removeLast();

        m_data->m_state = *saved;

        delete saved;

        m_data->m_state.apply( m_data->m_surface );
    }
}

// Draws a filled rectangle with a stroked border.
void GraphicsContext::drawRect(const IntRect& rect)
{
    D_DEBUG_AT( GraphicsContext_DFB, "%s( %4d,%4d-%4dx%4d )\n", __FUNCTION__,
                rect.x(), rect.y(), rect.width(), rect.height() );

    if (paintingDisabled())
        return;

    m_data->m_state.applyPorterDraw( m_data->m_surface, m_data->m_state.m_color_fill.a );

    m_data->m_surface->SetColor(m_data->m_surface, m_data->m_state.m_color_fill.r, m_data->m_state.m_color_fill.g, m_data->m_state.m_color_fill.b, m_data->m_state.m_color_fill.a);

    m_data->m_surface->FillRectangle(m_data->m_surface, rect.x(), rect.y(), rect.width(), rect.height());


    if (strokeStyle() != NoStroke) {
         m_data->m_state.applyPorterDraw( m_data->m_surface, m_data->m_state.m_color_stroke.a );

         m_data->m_surface->SetColor(m_data->m_surface, m_data->m_state.m_color_stroke.r, m_data->m_state.m_color_stroke.g, m_data->m_state.m_color_stroke.b, m_data->m_state.m_color_stroke.a);

         m_data->m_surface->DrawRectangle(m_data->m_surface, rect.x(), rect.y(), rect.width(), rect.height());
    }
}

// This is only used to draw borders.
void GraphicsContext::drawLine(const IntPoint& point1, const IntPoint& point2)
{
    D_DEBUG_AT( GraphicsContext_DFB, "%s()\n", __FUNCTION__ );

    if (paintingDisabled())
        return;

    if (strokeStyle() == NoStroke)
        return;

    m_data->m_state.applyPorterDraw( m_data->m_surface, m_data->m_state.m_color_stroke.a );

    m_data->m_surface->SetColor(m_data->m_surface, m_data->m_state.m_color_stroke.r, m_data->m_state.m_color_stroke.g, m_data->m_state.m_color_stroke.b, m_data->m_state.m_color_stroke.a);

    m_data->m_surface->DrawLine(m_data->m_surface, point1.x(), point1.y(), point2.x(), point2.y());
}

// This method is only used to draw the little circles used in lists.
void GraphicsContext::drawEllipse(const IntRect& rect)
{
    D_DEBUG_AT( GraphicsContext_DFB, "%s()\n", __FUNCTION__ );

    D_UNIMPLEMENTED();
}

void GraphicsContext::strokeArc(const IntRect& rect, int startAngle, int angleSpan)
{
    D_DEBUG_AT( GraphicsContext_DFB, "%s()\n", __FUNCTION__ );

    D_UNIMPLEMENTED();
}

void GraphicsContext::strokePath()
{
    D_DEBUG_AT( GraphicsContext_DFB, "%s()\n", __FUNCTION__ );

    D_UNIMPLEMENTED();
}

void GraphicsContext::drawConvexPolygon(size_t pointsLength, const FloatPoint* points, bool shouldAntialias)
{
    D_DEBUG_AT( GraphicsContext_DFB, "%s()\n", __FUNCTION__ );

    D_UNIMPLEMENTED();
}

void GraphicsContext::clipConvexPolygon(size_t numPoints, const FloatPoint* points, bool antialiased)
{
    D_DEBUG_AT( GraphicsContext_DFB, "GraphicsContext::%s()\n", __FUNCTION__ );

    D_UNIMPLEMENTED();

    if (paintingDisabled())
        return;

    if (numPoints <= 1)
        return;
    
    // FIXME: IMPLEMENT!!
}

void GraphicsContext::fillRect(const FloatRect& rect, const Color& color, ColorSpace colorSpace)
{
     D_DEBUG_AT( GraphicsContext_DFB, "%s( %4d,%4d-%4dx%4d <= %02x %02x %02x %02x )\n", __FUNCTION__,
                 (int) rect.x(), (int) rect.y(), (int) rect.width(), (int) rect.height(),
                 color.alpha(), color.red(), color.green(), color.blue() );

    if (paintingDisabled())
        return;

    m_data->m_state.applyPorterDraw( m_data->m_surface, color.alpha() );

    m_data->m_surface->SetColor(m_data->m_surface, color.red(), color.green(), color.blue(), color.alpha());

    m_data->m_surface->FillRectangle(m_data->m_surface, rect.x(), rect.y(), rect.width(), rect.height());
}

void GraphicsContext::fillRect(const FloatRect& rect)
{
    D_DEBUG_AT( GraphicsContext_DFB, "%s()\n", __FUNCTION__ );
    if (paintingDisabled())
        return;

    D_UNIMPLEMENTED();
}

void GraphicsContext::fillRoundedRect(const IntRect& rect, const IntSize& topLeft, const IntSize& topRight, const IntSize& bottomLeft, const IntSize& bottomRight, const Color& color, ColorSpace colorSpace)
{
    D_DEBUG_AT( GraphicsContext_DFB, "%s()\n", __FUNCTION__ );
    if (paintingDisabled() || !color.alpha())
        return;

    D_UNIMPLEMENTED();
}

void GraphicsContext::fillPath()
{
    D_DEBUG_AT( GraphicsContext_DFB, "%s()\n", __FUNCTION__ );
    D_UNIMPLEMENTED();
}

void GraphicsContext::beginPath()
{
    D_DEBUG_AT( GraphicsContext_DFB, "%s()\n", __FUNCTION__ );
    D_UNIMPLEMENTED();
}

void GraphicsContext::addPath(const Path& path)
{
    D_DEBUG_AT( GraphicsContext_DFB, "%s()\n", __FUNCTION__ );
    D_UNIMPLEMENTED();
}

void GraphicsContext::clip(const FloatRect& rect)
{
    D_DEBUG_AT( GraphicsContext_DFB, "%s( %4d,%4d-%4dx%4d )\n", __FUNCTION__,
                (int) rect.x(), (int) rect.y(), (int) rect.width(), (int) rect.height() );

    DFBRegion region = {
         m_data->m_state.m_matrix[2]/65536 + rect.x(),
         m_data->m_state.m_matrix[5]/65536 + rect.y(),
         m_data->m_state.m_matrix[2]/65536 + rect.x()+rect.width()-1,
         m_data->m_state.m_matrix[5]/65536 + rect.y()+rect.height()-1
    };

    if (paintingDisabled())
        return;

    D_DEBUG_AT( GraphicsContext_DFB, "  -> %4d,%4d-%4d,%4d\n",
                region.x1, region.y1, region.x2, region.y2 );

    m_data->m_state.m_clip     = region;
    m_data->m_state.m_clip_set = true;

    m_data->m_state.applyClip( m_data->m_surface );
}

void GraphicsContext::drawFocusRing(const Vector<Path>& paths, int width, int offset, const Color& color)
{
    D_DEBUG_AT( GraphicsContext_DFB, "%s()\n", __FUNCTION__ );
     D_UNIMPLEMENTED();
}

void GraphicsContext::drawFocusRing(const Vector<IntRect>& rects, int /* width */, int /* offset */, const Color& color)
{
    D_DEBUG_AT( GraphicsContext_DFB, "%s()\n", __FUNCTION__ );
     D_UNIMPLEMENTED();
}

void GraphicsContext::drawLineForText(const IntPoint& origin, int width, bool printing)
{
    D_DEBUG_AT( GraphicsContext_DFB, "%s()\n", __FUNCTION__ );
     D_UNIMPLEMENTED();
}

void GraphicsContext::drawLineForTextChecking(const IntPoint&, int width, TextCheckingLineStyle)
{
    D_DEBUG_AT( GraphicsContext_DFB, "GraphicsContext::%s()\n", __FUNCTION__ );

    if (paintingDisabled())
        return;

    D_UNIMPLEMENTED();
}

FloatRect GraphicsContext::roundToDevicePixels(const FloatRect& rect)
{
    D_DEBUG_AT( GraphicsContext_DFB, "%s()\n", __FUNCTION__ );
    D_UNIMPLEMENTED();
    return rect;
}

void GraphicsContext::beginTransparencyLayer(float opacity)
{
    D_DEBUG_AT( GraphicsContext_DFB, "%s()\n", __FUNCTION__ );
    if (paintingDisabled())
        return;

    D_UNIMPLEMENTED();
}

void GraphicsContext::endTransparencyLayer()
{
    D_DEBUG_AT( GraphicsContext_DFB, "%s()\n", __FUNCTION__ );
    if (paintingDisabled())
        return;

    D_UNIMPLEMENTED();
}

void GraphicsContext::clearRect(const FloatRect& rect)
{
    D_DEBUG_AT( GraphicsContext_DFB, "%s()\n", __FUNCTION__ );
    if (paintingDisabled())
        return;

    D_UNIMPLEMENTED();
}

void GraphicsContext::strokeRect(const FloatRect& rect, float width)
{
    D_DEBUG_AT( GraphicsContext_DFB, "%s()\n", __FUNCTION__ );
    if (paintingDisabled())
        return;

    m_data->m_state.applyPorterDraw( m_data->m_surface, m_data->m_state.m_color_stroke.a );

    m_data->m_surface->SetColor(m_data->m_surface, m_data->m_state.m_color_stroke.r, m_data->m_state.m_color_stroke.g, m_data->m_state.m_color_stroke.b, m_data->m_state.m_color_stroke.a);

    m_data->m_surface->DrawRectangle(m_data->m_surface, rect.x(), rect.y(), rect.width(), rect.height());
}

void GraphicsContext::setLineCap(LineCap lineCap)
{
    D_DEBUG_AT( GraphicsContext_DFB, "%s()\n", __FUNCTION__ );
    if (paintingDisabled())
        return;

    D_UNIMPLEMENTED();
}

void GraphicsContext::setLineDash(const DashArray& dashes, float dashOffset)
{
    D_DEBUG_AT( GraphicsContext_DFB, "%s()\n", __FUNCTION__ );
     if (paintingDisabled())
         return;

     D_UNIMPLEMENTED();
//    CGContextSetLineDash(platformContext(), dashOffset, dashes.data(), dashes.size());
}

void GraphicsContext::setLineJoin(LineJoin lineJoin)
{
    D_DEBUG_AT( GraphicsContext_DFB, "%s()\n", __FUNCTION__ );
    if (paintingDisabled())
        return;

    D_UNIMPLEMENTED();
}

void GraphicsContext::setMiterLimit(float limit)
{
    D_DEBUG_AT( GraphicsContext_DFB, "%s()\n", __FUNCTION__ );
    if (paintingDisabled())
        return;

    D_UNIMPLEMENTED();
}

void GraphicsContext::setAlpha(float opacity)
{
    D_DEBUG_AT( GraphicsContext_DFB, "%s()\n", __FUNCTION__ );
    if (paintingDisabled())
        return;

    D_UNIMPLEMENTED();
}

void GraphicsContext::setCompositeOperation(CompositeOperator op)
{
    D_DEBUG_AT( GraphicsContext_DFB, "%s()\n", __FUNCTION__ );
    if (paintingDisabled())
        return;

    DFBSurfacePorterDuffRule rule = DSPD_SRC;
    switch (op) {
    case CompositeClear:
    case CompositeCopy:
        // Use the default above
        break;
    case CompositeSourceOver:
        rule = DSPD_SRC_OVER;
        break;
    default:
        printf("GraphicsContext::setCompositeOperation: Unsupported composite operation %s\n",
                compositeOperatorName(op).utf8().data());
    }

    m_data->m_state.m_porterduff = rule;

    m_data->m_state.applyPorterBlit( m_data->m_surface );
}

void GraphicsContext::clip(const Path& path)
{
    D_DEBUG_AT( GraphicsContext_DFB, "%s()\n", __FUNCTION__ );
    if (paintingDisabled())
        return;

    D_UNIMPLEMENTED();

    FloatRect bounds = path.boundingRect();

    clip( bounds );
}

void GraphicsContext::clipOut(const Path& path)
{
    D_DEBUG_AT( GraphicsContext_DFB, "%s()\n", __FUNCTION__ );
    if (paintingDisabled())
        return;

    D_UNIMPLEMENTED();
}

void GraphicsContext::canvasClip(const Path& path)
{
    D_DEBUG_AT( GraphicsContext_DFB, "GraphicsContext::%s()\n", __FUNCTION__ );

    clip(path);
}

AffineTransform GraphicsContext::getCTM() const
{
    D_DEBUG_AT( GraphicsContext_DFB, "%s()\n", __FUNCTION__ );
    D_UNIMPLEMENTED();
    return AffineTransform();
}

void GraphicsContext::translate(float x, float y)
{
    D_DEBUG_AT( GraphicsContext_DFB, "%s( %8.3f, %8.3f )\n", __FUNCTION__, x, y );

    if (paintingDisabled())
        return;

    // FIXME
    m_data->m_state.m_matrix[2] += 0x10000 * x;
    m_data->m_state.m_matrix[5] += 0x10000 * y;

    m_data->m_state.applyMatrix( m_data->m_surface );
}

void GraphicsContext::rotate(float radians)
{
    D_DEBUG_AT( GraphicsContext_DFB, "%s( %8.3f )\n", __FUNCTION__, radians );

    if (paintingDisabled())
        return;

    D_UNIMPLEMENTED();
}

void GraphicsContext::scale(const FloatSize& size)
{
    D_DEBUG_AT( GraphicsContext_DFB, "%s( %8.3f, %8.3f )\n", __FUNCTION__, size.width(), size.height() );

    if (paintingDisabled())
        return;

    // FIXME
    m_data->m_state.m_matrix[0] *= size.width();
    m_data->m_state.m_matrix[4] *= size.height();

    m_data->m_state.applyMatrix( m_data->m_surface );
}

void GraphicsContext::clipOut(const IntRect& rect)
{
    D_DEBUG_AT( GraphicsContext_DFB, "%s()\n", __FUNCTION__ );
    if (paintingDisabled())
        return;

    D_UNIMPLEMENTED();
}

void GraphicsContext::clipPath(WindRule clipRule)
{
    D_DEBUG_AT( GraphicsContext_DFB, "%s()\n", __FUNCTION__ );
    if (paintingDisabled())
        return;

    D_UNIMPLEMENTED();
/*
    CGContextRef context = platformContext();

    if (!CGContextIsPathEmpty(context)) {
        if (clipRule == RULE_EVENODD)
            CGContextEOClip(context);
        else
            CGContextClip(context);
    }
*/
}

void GraphicsContext::addInnerRoundedRectClip(const IntRect& rect, int thickness)
{
    D_DEBUG_AT( GraphicsContext_DFB, "%s()\n", __FUNCTION__ );
    if (paintingDisabled())
        return;

    D_UNIMPLEMENTED();
}

void GraphicsContext::concatCTM(const AffineTransform& transform)
{
    D_DEBUG_AT( GraphicsContext_DFB, "%s()\n", __FUNCTION__ );
    if (paintingDisabled())
        return;

    D_UNIMPLEMENTED();
}

void GraphicsContext::setPlatformShouldAntialias(bool enable)
{
    D_DEBUG_AT( GraphicsContext_DFB, "%s()\n", __FUNCTION__ );
    if (paintingDisabled())
        return;

    D_UNIMPLEMENTED();
}

void GraphicsContext::setImageInterpolationQuality(InterpolationQuality)
{
    D_DEBUG_AT( GraphicsContext_DFB, "%s()\n", __FUNCTION__ );
}

InterpolationQuality GraphicsContext::imageInterpolationQuality() const
{
    D_DEBUG_AT( GraphicsContext_DFB, "GraphicsContext::%s()\n", __FUNCTION__ );

    D_UNIMPLEMENTED();
    return InterpolationDefault;
}

void GraphicsContext::setURLForRect(const KURL& link, const IntRect& destRect)
{
    D_DEBUG_AT( GraphicsContext_DFB, "%s()\n", __FUNCTION__ );
    D_UNIMPLEMENTED();
}

void GraphicsContext::setPlatformFont(const Font& font)
{
    D_DEBUG_AT( GraphicsContext_DFB, "%s()\n", __FUNCTION__ );
    m_data->m_surface->SetFont(m_data->m_surface, font.primaryFont()->platformData().font());
}

void GraphicsContext::setPlatformStrokeColor(const Color& color, ColorSpace colorSpace)
{
    D_DEBUG_AT( GraphicsContext_DFB, "%s()\n", __FUNCTION__ );
    if (paintingDisabled())
        return;

    m_data->m_state.m_color_stroke.a = color.alpha();
    m_data->m_state.m_color_stroke.r = color.red();
    m_data->m_state.m_color_stroke.g = color.green();
    m_data->m_state.m_color_stroke.b = color.blue();
}

void GraphicsContext::setPlatformStrokeStyle(const StrokeStyle& strokeStyle)
{
    D_DEBUG_AT( GraphicsContext_DFB, "%s()\n", __FUNCTION__ );
    D_UNIMPLEMENTED();
}

void GraphicsContext::setPlatformStrokeThickness(float thickness)
{
    D_DEBUG_AT( GraphicsContext_DFB, "%s()\n", __FUNCTION__ );
    if (paintingDisabled())
        return;

    D_UNIMPLEMENTED();
}

void GraphicsContext::setPlatformFillColor(const Color& color, ColorSpace colorSpace)
{
    D_DEBUG_AT( GraphicsContext_DFB, "%s()\n", __FUNCTION__ );
    if (paintingDisabled())
        return;

    m_data->m_state.m_color_fill.a = color.alpha();
    m_data->m_state.m_color_fill.r = color.red();
    m_data->m_state.m_color_fill.g = color.green();
    m_data->m_state.m_color_fill.b = color.blue();
}

void GraphicsContext::clearPlatformShadow()
{
    D_DEBUG_AT( GraphicsContext_DFB, "%s()\n", __FUNCTION__ );
    D_UNIMPLEMENTED();
}

void GraphicsContext::setPlatformShadow(const FloatSize&, float, Color const&, ColorSpace) 
{
    D_DEBUG_AT( GraphicsContext_DFB, "%s()\n", __FUNCTION__ );
    D_UNIMPLEMENTED();
}

} // namespace WebCore

