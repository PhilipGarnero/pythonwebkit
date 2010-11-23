/*
 * Copyright (C) 2006 Apple Computer, Inc.
 * Copyright (C) 2006 Zack Rusin <zack@kde.org>
 * Copyright (C) 2006 Dirk Mueller <mueller@kde.org>
 * Copyright (C) 2006 Nikolas Zimmermann <zimmermann@kde.org>
 * Copyright (C) 2007 Ryan Leavengood <leavengood@gmail.com>
 * Copyright (C) 2009 Maxime Simon <simon.maxime@gmail.com>
 *
 * All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 */

#ifndef FontPlatformData_H
#define FontPlatformData_H

#include "FontDescription.h"
#include "GlyphBuffer.h"

#include <lite/font.h>

namespace WebCore {

    class FontPlatformData {
    public:
        enum FontState { UNINITIALIZED, DELETED, VALID };

        FontPlatformData(WTF::HashTableDeletedValueType)
            : m_font(hashTableDeletedFontValue())
            { }

        ~FontPlatformData();

        FontPlatformData(const FontDescription&, const AtomicString& family);
        FontPlatformData(float size, bool bold, bool italic)
        : m_font(0)
        , m_fontState(UNINITIALIZED)
        {
        }

        FontPlatformData() 
        : m_font(0)
        , m_fontState(UNINITIALIZED)
        {
        }

        IDirectFBFont* font() const { IDirectFBFont *font; lite_font( m_font, &font ); return font; }

        bool isFixedPitch();
        float size() const { return m_size; }
//        bool bold() const { return m_bold; }
//        bool oblique() const { return m_oblique; }

        FontOrientation orientation() const { return Horizontal; } // FIXME: Implement.


        unsigned hash() const {
            switch (m_fontState) {
            case DELETED:
                return -1;
            case UNINITIALIZED:
                return 0;
            case VALID:
                return computeHash();              
            }
        }

        unsigned computeHash() const;

        bool operator==(const FontPlatformData& other) const
        { 
            if (m_font && m_fontState == VALID && other.m_fontState == VALID && other.m_font) {
                LiteFont* thisFont = m_font;//->font();
                LiteFont* otherFont = other.m_font;//->font();
                return thisFont == otherFont;// thisFont->IsOk() && otherFont->IsOk() && thisFont->IsSameAs(*otherFont);
            }
            else
                return m_fontState == other.m_fontState;
        }

        bool isHashTableDeletedValue() const { return m_fontState == DELETED; }

#ifndef NDEBUG
        String description() const;
#endif

        LiteFont* m_font;
//        float m_size;
//        bool m_bold;
//        bool m_oblique;
        FontState m_fontState;

        String          m_family;
        LiteFontStyle   m_style;
        int             m_size;

    private:
        static LiteFont* hashTableDeletedFontValue() { return reinterpret_cast<LiteFont*>(-1); }
    };

} // namespace WebCore

#endif

