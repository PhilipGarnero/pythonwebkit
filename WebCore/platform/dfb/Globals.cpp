/*
 * Copyright (C) 2009-2011 Denis Oliver Kropp <dok@directfb.org>
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

#include "Globals.h"

#include <directfb.h>

extern "C" {
#include <direct/debug.h>
}


D_DEBUG_DOMAIN( WebKit_DFB, "WebKit/DFB", "WebKit DFB C++ API" );

/**********************************************************************************************************************/


namespace WebCore {


IDirectFB *Globals::dfb = NULL;


} // namespace WebCore


namespace WebKitDFB {

DFBResult
Initialize( IDirectFB *dfb )
{
     DirectResult ret;

     D_DEBUG_AT( WebKit_DFB, "%s( dfb %p )\n", __func__, dfb );

     ret = dfb->AddRef( dfb );
     if (ret) {
          D_DERROR( ret, "WebKit/DFB: IDirectFB::AddRef() failed!\n" );
          return (DFBResult) ret;
     }

     WebCore::Globals::dfb = dfb;

     return DFB_OK;
}

}

