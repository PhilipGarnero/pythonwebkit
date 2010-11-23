/*
 * Copyright (C) 2007 Ryan Leavengood <leavengood@gmail.com>
 * Copyright (C) 2009 Maxime Simon <simon.maxime@gmail.com>
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
#include "PlatformWheelEvent.h"

#include "Scrollbar.h"

#include <directfb.h>


namespace WebCore {

static const int cScrollbarPixelsPerLineStep = 8;

PlatformWheelEvent::PlatformWheelEvent(const DFBEvent* event)
    : m_granularity(ScrollByPixelWheelEvent)
    , m_isAccepted(false)
    , m_shiftKey(false)
    , m_ctrlKey(false)
    , m_altKey(false)
    , m_metaKey(false)
{
     switch (event->clazz) {
         case DFEC_WINDOW:
             m_position       = IntPoint(event->window.x, event->window.y);
             m_globalPosition = IntPoint(event->window.cx, event->window.cy);

             switch (event->window.type) {
                 case DWET_WHEEL:
                     m_deltaX = 0;
                     m_deltaY = event->window.step;
                     break;

                 default:
                     break;
             }
             break;

        default:
            break;
    }

    m_wheelTicksX = m_deltaX;
    m_wheelTicksY = m_deltaY;

    m_deltaX *= -cScrollbarPixelsPerLineStep;
    m_deltaY *= -cScrollbarPixelsPerLineStep;
}

PlatformWheelEvent::PlatformWheelEvent(int x, int y, int delta)
    : m_granularity(ScrollByPixelWheelEvent)
    , m_isAccepted(false)
    , m_shiftKey(false)
    , m_ctrlKey(false)
    , m_altKey(false)
    , m_metaKey(false)
{
     m_position       = IntPoint(x, y);
     m_globalPosition = IntPoint(x, y); // FIXME

     m_deltaX = 0;
     m_deltaY = delta;

     m_wheelTicksX = m_deltaX;
     m_wheelTicksY = m_deltaY;

     m_deltaX *= -cScrollbarPixelsPerLineStep;
     m_deltaY *= -cScrollbarPixelsPerLineStep;
}

} // namespace WebCore

