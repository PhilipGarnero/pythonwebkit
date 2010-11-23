/*
 * Copyright (C) 2007 Kevin Ollivier <kevino@theolliviers.com>
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
#include "FontPlatformData.h"

#include "FontDescription.h"
#include "PlatformString.h"

#include <lite/font.h>

#include <direct/debug.h>

D_DEBUG_DOMAIN( FontPlatformData_DFB, "FontPlatformDataDFB", "FontPlatformDataDFB" );

namespace WebCore {

FontPlatformData::FontPlatformData(const FontDescription& desc, const AtomicString& family)
{
    DFBResult ret;

    m_family = family.string();
    m_style  = LITE_FONT_PLAIN;

    if (desc.italic())
        m_style = (LiteFontStyle)( m_style | LITE_FONT_ITALIC );

    if (desc.weight() >= FontWeight600)
        m_style = (LiteFontStyle)( m_style | LITE_FONT_BOLD );

    m_size = desc.computedPixelSize();

    if (m_size < 8) {
        abort();
         m_size = 8;
    }

    D_DEBUG_AT( FontPlatformData_DFB, "%s( %s, %d, style %d )\n", __FUNCTION__,
                (char*) m_family.characters(), m_size, m_style );

    ret = lite_get_font( (char*) m_family.characters(), m_style, m_size, DFFA_NONE, &m_font );
    if (ret) {
        D_DERROR( (DirectResult) ret, "FontPlatformData::FontPlatformData: Could not get font (%s, size %d, style %d)!\n",
                  (char*) m_family.characters(), m_size, m_style );
        return;
    }

    m_fontState = VALID;
}

unsigned FontPlatformData::computeHash() const
{
    ASSERT(m_font);
    
    // make a hash that is unique for this font, but not globally unique - that is,
    // a font whose properties are equal should generate the same hash
    uintptr_t hashCodes[3] = { StringImpl::computeHash((char*) m_family.characters()), m_style, m_size };
    
    return StringImpl::computeHash(reinterpret_cast<UChar*>(hashCodes), sizeof(hashCodes) / sizeof(UChar));
}

FontPlatformData::~FontPlatformData()
{
    if (m_font)
        lite_release_font( m_font );

    m_fontState = UNINITIALIZED;
    m_font = 0;
}

#ifndef NDEBUG
String FontPlatformData::description() const
{
    return String();
}
#endif

}
