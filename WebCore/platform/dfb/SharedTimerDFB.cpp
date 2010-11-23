/*
 * Copyright (C) 2006 Apple Computer, Inc.  All rights reserved.
 * Copyright (C) 2006 Michael Emmel mike.emmel@gmail.com
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
#include "SharedTimer.h"

#include <wtf/Assertions.h>
#include <wtf/CurrentTime.h>
#include <lite/window.h>


extern "C" {
#include <direct/debug.h>
#include <direct/messages.h>
}


D_DEBUG_DOMAIN(WebKit_SharedTimer, "WebKit/SharedTimer", "WebKit SharedTimer");

/*********************************************************************/

namespace WebCore {

static int sharedTimer;
static void (*sharedTimerFiredFunction)();

void setSharedTimerFiredFunction(void (*f)())
{
    D_DEBUG_AT( WebKit_SharedTimer, "%s( %p )\n", __func__, f );
    sharedTimerFiredFunction = f;
}

static DFBResult SharedTimer_timeout_cb(void* p)
{
    D_DEBUG_AT( WebKit_SharedTimer, "%s( %p )\n", __func__, p );
    if (sharedTimerFiredFunction)
        sharedTimerFiredFunction();
    return DFB_OK;
}

void setSharedTimerFireTime(double fireTime)
{
    D_DEBUG_AT( WebKit_SharedTimer, "%s( %.3d )\n", __func__, fireTime );
    ASSERT(sharedTimerFiredFunction);

    double interval = fireTime - currentTime();
    int intervalInMS;
    if (interval < 0)
        intervalInMS = 0;
    else {
        interval *= 1000;
        intervalInMS = (int)interval;
    }

    printf("shared timer time %f %d\n", fireTime, intervalInMS);

    stopSharedTimer();
    lite_enqueue_window_timeout(intervalInMS, SharedTimer_timeout_cb,
                                NULL, &sharedTimer);
}

void stopSharedTimer()
{
    D_DEBUG_AT( WebKit_SharedTimer, "%s()\n", __func__ );
    DFBResult s;
    if (sharedTimer == 0)
        return;

    s = lite_remove_window_timeout(sharedTimer);
    ASSERT(s == DFB_OK);
    sharedTimer = 0;
}

}
