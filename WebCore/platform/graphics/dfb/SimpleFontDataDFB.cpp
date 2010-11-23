/*
 * Copyright (C) 2006 Apple Computer, Inc.  All rights reserved.
 * Copyright (C) 2009 Maxime Simon <simon.maxime@gmail.com> All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "SimpleFontData.h"

#include "FloatRect.h"
#include "FontCache.h"
#include "FontDescription.h"
#include "NotImplemented.h"
#include <Rect.h>
#include <unicode/uchar.h>
#include <unicode/unorm.h>

#include <direct/debug.h>

D_DEBUG_DOMAIN( SimpleFontData_DFB, "SimpleFontDataDFB", "SimpleFontDataDFB" );


namespace WebCore {

void SimpleFontData::platformInit()
{
    D_DEBUG_AT( SimpleFontData_DFB, "%s()\n", __FUNCTION__ );

    IDirectFBFont* font = m_platformData.font();
    if (!font)
        return;

    int height;
    int ascender;
    int descender;
    font->GetHeight(font, &height);
    font->GetAscender(font, &ascender);
    font->GetDescender(font, &descender);

    m_ascent = ascender;
    m_descent = -descender;
    m_lineSpacing = m_ascent + m_descent;
    m_xHeight = ascender * 0.56f; // Hack taken from the win port.
    m_lineGap = height - m_lineSpacing;
}

void SimpleFontData::platformCharWidthInit()
{
    D_DEBUG_AT( SimpleFontData_DFB, "%s()\n", __FUNCTION__ );

    m_avgCharWidth = 0.f;
    m_maxCharWidth = 0.f;
    initCharWidths();
}

void SimpleFontData::platformDestroy()
{
    D_DEBUG_AT( SimpleFontData_DFB, "%s()\n", __FUNCTION__ );

    delete m_smallCapsFontData;
    m_smallCapsFontData = 0;
}

SimpleFontData* SimpleFontData::smallCapsFontData(const FontDescription& fontDescription) const
{
    D_DEBUG_AT( SimpleFontData_DFB, "%s( %f )\n", __FUNCTION__, fontDescription.computedSize() );

    if (!m_smallCapsFontData) {
        FontDescription desc = FontDescription(fontDescription);
        desc.setComputedSize(0.70f*fontDescription.computedSize());
//        desc.setSpecifiedSize(0.70f * fontDescription.computedSize());
        const FontPlatformData* fontPlatformData = new FontPlatformData(desc, desc.family().family());
        m_smallCapsFontData = new SimpleFontData(*fontPlatformData);
    }
    return m_smallCapsFontData;
}

bool SimpleFontData::containsCharacters(const UChar* characters, int length) const
{
    D_DEBUG_AT( SimpleFontData_DFB, "%s()\n", __FUNCTION__ );

    // FIXME: We will need to implement this to load non-ASCII encoding sites
    return true;
}

void SimpleFontData::determinePitch()
{
    D_DEBUG_AT( SimpleFontData_DFB, "%s()\n", __FUNCTION__ );

    m_treatAsFixedPitch = false; // DFB FIXME: m_platformData.font() && m_platformData.font()->IsFixed();
}

FloatRect SimpleFontData::platformBoundsForGlyph(Glyph) const
{
    return FloatRect();
}

float SimpleFontData::platformWidthForGlyph(Glyph glyph) const
{
    int advance;

    D_DEBUG_AT( SimpleFontData_DFB, "%s()\n", __FUNCTION__ );

    if (m_platformData.font()) {
        m_platformData.font()->GetGlyphExtents(m_platformData.font(), glyph, NULL, &advance);
        return advance;
    }

    return 0;
}

} // namespace WebCore

