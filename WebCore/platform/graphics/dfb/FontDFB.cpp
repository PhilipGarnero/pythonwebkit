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
#include "Font.h"

#include "FontData.h"
#include "FontDescription.h"
#include "FontSelector.h"
#include "GraphicsContext.h"
#include "NotImplemented.h"

#include <directfb.h>


#include <direct/debug.h>

D_DEBUG_DOMAIN( Font_DFB, "FontDFB", "FontDFB" );


namespace WebCore {

bool Font::canReturnFallbackFontsForComplexText()
{
    return false;
}

void Font::drawGlyphs(GraphicsContext* graphicsContext, const SimpleFontData* font,
                      const GlyphBuffer& glyphBuffer, int from, int numGlyphs, const FloatPoint& point) const
{
    Color color = graphicsContext->fillColor();
    IDirectFBSurface* surface = graphicsContext->platformContext();
    IDirectFBFont* m_font = font->platformData().font();

    D_DEBUG_AT( Font_DFB, "%s( %p, %p, from %d, num %d, at %.3f,%.3f )\n", __FUNCTION__,
                graphicsContext, font, from, numGlyphs, point.x(), point.y() );

    graphicsContext->setCompositeOperation(CompositeSourceOver);
    surface->SetColor(surface, color.red(), color.green(), color.blue(), color.alpha());
    surface->SetFont(surface, m_font);

    GlyphBufferGlyph* glyphs = const_cast<GlyphBufferGlyph*>(glyphBuffer.glyphs(from));

#if 0
    float offset = point.x();

    for (int i = 0; i < numGlyphs; i++) {
        surface->DrawGlyph(surface, glyphs[i], offset, point.y(), DSTF_LEFT);
        offset += glyphBuffer.advanceAt(from + i);
    }

#else
    char  str[numGlyphs*3];
    char *utf8 = str;

    for (int i = 0; i < numGlyphs; i++) {
        int unicode = 0;

          if( glyphs[i] == 0xfffe || glyphs[i] == 0xfeff ) /* ignore BOM */
               continue;

          if ( (glyphs[i] & 0xfc00) == 0xd800 ) { /* most significant surrogate */
               unicode = glyphs[i] & 0x3ff;
               continue;
          }

          if ( (glyphs[i] & 0xfc00) == 0xdc00 ) { /* least significant surrogate */
               if (i==0) /* no accept half code */
                    continue;

               unicode = 0x10000 + (unicode << 10) | (glyphs[i] & 0x3ff);
          }
          else
               unicode = glyphs[i];

          /* we have Unicode */
          
          if (unicode < 0x80) {
               *utf8++ = unicode;
          } else if (unicode < 0x800) {
               *utf8++ = 0xc0 + (unicode >> 6);
               *utf8++ = 0x80 + (unicode & 0x3f);
          } else if (unicode < 0x10000) {
               *utf8++ = 0xe0 +  (unicode >> 12);
               *utf8++ = 0x80 + ((unicode >>  6) & 0x3f);
               *utf8++ = 0x80 + ( unicode        & 0x3f);
          } else {
               *utf8++ = 0xf0 +  (unicode >> 18);
               *utf8++ = 0x80 + ((unicode >> 12) & 0x3f);
               *utf8++ = 0x80 + ((unicode >>  6) & 0x3f);
               *utf8++ = 0x80 + ( unicode        & 0x3f);
          }
    }

    surface->DrawString(surface, str, utf8 - str, point.x(), point.y(), DSTF_LEFT);
#endif
}

void Font::drawComplexText(GraphicsContext* ctx, const TextRun& run, const FloatPoint& point,
                           int from, int to) const
{
    notImplemented();
}


float Font::floatWidthForComplexText(const TextRun& run, HashSet<const SimpleFontData*>* fallbackFonts, GlyphOverflow* glyphOverflow) const
{
    notImplemented();
    return 0;
}

FloatRect Font::selectionRectForComplexText(const TextRun&, const FloatPoint&, int, int, int) const
{
    notImplemented();
    return FloatRect();
}

int Font::offsetForPositionForComplexText(const TextRun&, float, bool) const
{
    notImplemented();
    return 0;
}

} // namespace WebCore

