/*
 * Copyright (C) 2009 Maxime Simon
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "EventLoop.h"
#include "webkitdefines.h"

extern "C" {
#include <direct/debug.h>
#include <lite/window.h>

LiteWindow *global_mainwin;

WEBKIT_API void webkit_set_lite_global_mainwin(LiteWindow *win)
{
    global_mainwin = win;
}

}


D_DEBUG_DOMAIN( WebKit_EventLoop, "WebKit/EventLoop", "WebKit EventLoop" );

/*************************************************************************/

namespace WebCore {

void EventLoop::cycle()
{
     D_DEBUG_AT( WebKit_EventLoop, "EventLoop::%s()\n", __func__ );

    if (lite_window_event_available() == DFB_OK)
    {
        lite_window_event_loop( global_mainwin, 1 );
    }
    g_main_context_iteration(NULL, FALSE);


     g_main_context_iteration(NULL, FALSE);
//    if (!be_app)
//        m_ended = true;
}

} // namespace WebCore

