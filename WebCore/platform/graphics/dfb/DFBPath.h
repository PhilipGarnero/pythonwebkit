/*
    Copyright (C) 2007 Alp Toker <alp.toker@collabora.co.uk>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    aint with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef DFBPath_h
#define DFBPath_h

#include <directfb.h>

namespace WebCore {

    struct DFBPath {
        bool      empty;
        DFBRegion region;

        DFBPath()
        {
            empty = true;
        }

        ~DFBPath()
        {
        }

        bool Contains(const FloatPoint& point)
        {
             if (empty)
                 return false;

             if (point.x() < region.x1)
                 return false;

             if (point.y() < region.y1)
                 return false;

             if (point.x() > region.x2)
                 return false;

             if (point.y() > region.y2)
                 return false;

             return true;
        }

        FloatRect Bounds()
        {
            if (empty)
                return FloatRect(0,0,0,0);
                
            return FloatRect(region.x1, region.y1, region.x2-region.x1+1, region.y2-region.y1+1);
        }

        void Reset()
        {
            empty = true;
        }

        void Add(const FloatRect& r)
        {
            if (empty) {
                region.x1 = r.x();
                region.y1 = r.y();
                region.x2 = region.x1 + r.width() - 1;
                region.y2 = region.y1 + r.height() - 1;

                empty = false;
            }
            else
                notImplemented();
        }
    };

} // namespace WebCore

#endif // DFBPath_h
