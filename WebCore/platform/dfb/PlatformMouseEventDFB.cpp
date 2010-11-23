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
#include "PlatformMouseEvent.h"

#include <directfb.h>


namespace WebCore {

PlatformMouseEvent::PlatformMouseEvent(const DFBEvent* event)
    : m_shiftKey(false)
    , m_ctrlKey(false)
    , m_altKey(false)
    , m_metaKey(false)
{
     switch (event->clazz) {
         case DFEC_WINDOW:
             m_timestamp      = event->window.timestamp.tv_sec * 1000000 + event->window.timestamp.tv_usec;
             m_position       = IntPoint(event->window.x, event->window.y);
             m_globalPosition = IntPoint(event->window.cx, event->window.cy);

             switch (event->window.type) {
                 case DWET_BUTTONDOWN:
                     m_clickCount = 1;
                     m_eventType  = MouseEventPressed;
                     break;

                 case DWET_BUTTONUP:
                     m_clickCount = 0;
                     m_eventType  = MouseEventReleased;
                     break;

                 case DWET_MOTION:
                     m_clickCount = 0;
                     m_eventType  = MouseEventMoved;
                     break;

                 default:
                     break;
             }

             switch (event->window.button) {
                 case DIBI_LEFT:
                     m_button = LeftButton;
                     break;

                 case DIBI_RIGHT:
                     m_button = RightButton;
                     break;

                 case DIBI_MIDDLE:
                     m_button = MiddleButton;
                     break;

                 default:
                     m_button = NoButton;
                     break;
             }
             break;

        default:
            break;
    }
}

PlatformMouseEvent::PlatformMouseEvent(int x, int y,
                                       DFBInputDeviceButtonIdentifier button,
                                       bool down)
    : m_shiftKey(false)
    , m_ctrlKey(false)
    , m_altKey(false)
    , m_metaKey(false)
{
     m_timestamp      = 0;              // FIXME
     m_position       = IntPoint(x, y);
     m_globalPosition = IntPoint(x, y); // FIXME

     if (down) {
          m_clickCount = 1;
          m_eventType  = MouseEventPressed;
     }
     else {
          m_clickCount = 0;
          m_eventType  = MouseEventReleased;
     }

     switch (button) {
         case DIBI_LEFT:
             m_button = LeftButton;
             break;

         case DIBI_RIGHT:
             m_button = RightButton;
             break;

         case DIBI_MIDDLE:
             m_button = MiddleButton;
             break;

         default:
             m_button = NoButton;
             break;
     }
}

PlatformMouseEvent::PlatformMouseEvent(int x, int y,
                                       DFBInputDeviceButtonMask buttons)
    : m_shiftKey(false)
    , m_ctrlKey(false)
    , m_altKey(false)
    , m_metaKey(false)
{
     m_timestamp      = 0;              // FIXME
     m_position       = IntPoint(x, y);
     m_globalPosition = IntPoint(x, y); // FIXME

     m_clickCount = 0;
     m_eventType  = MouseEventMoved;
}

} // namespace WebCore

