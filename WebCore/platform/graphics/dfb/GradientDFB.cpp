/*
 * Copyright (C) 2008 Kevin Ollivier <kevino@theolliviers.com>  All rights reserved.
 * Copyright (C) 2009 Maxime Simon <simon.maxime@theolliviers.com>
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
#include "FloatRect.h"
#include "Gradient.h"

#include "CSSParser.h"
#include "NotImplemented.h"



extern "C" {
#include <direct/debug.h>
#include <direct/messages.h>
}


D_DEBUG_DOMAIN( WebKit_Gradient, "WebKit/Gradient", "WebKit Gradient" );

/**********************************************************************************************************************/

namespace WebCore {

void Gradient::platformDestroy()
{
     D_DEBUG_AT( WebKit_Gradient, "Gradient::%s()\n", __func__ );

     D_UNIMPLEMENTED();
}

PlatformGradient Gradient::platformGradient()
{
     D_DEBUG_AT( WebKit_Gradient, "Gradient::%s()\n", __func__ );

     D_UNIMPLEMENTED();

     return 0;
}

void Gradient::fill(GraphicsContext*, const FloatRect& rect)
{
     D_DEBUG_AT( WebKit_Gradient, "Gradient::%s( %7.2f,%7.2f-%7.2fx%7.2f )\n", __func__,
                 rect.x(), rect.y(), rect.width(), rect.height() );

     D_UNIMPLEMENTED();
}

} // namespace WebCore

