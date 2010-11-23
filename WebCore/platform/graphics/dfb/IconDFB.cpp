/*
 * Copyright (C) 2007 Ryan Leavengood <leavengood@gmail.com>
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

#include "config.h"
#include "Icon.h"

#include "GraphicsContext.h"
#include "IntRect.h"
#include "NotImplemented.h"
#include "PlatformString.h"



extern "C" {
#include <direct/debug.h>
#include <direct/messages.h>
}


D_DEBUG_DOMAIN( WebKit_Icon, "WebKit/Icon", "WebKit Icon" );

/**********************************************************************************************************************/

namespace WebCore {

Icon::~Icon()
{
     D_DEBUG_AT( WebKit_Icon, "Icon::%s()\n", __func__ );

     D_UNIMPLEMENTED();
}

PassRefPtr<Icon> Icon::createIconForFiles(const Vector<String>& filenames)
{
     D_DEBUG_AT( WebKit_Icon, "Icon::%s()\n", __func__ );

     D_UNIMPLEMENTED();

     return 0;
}

void Icon::paint(GraphicsContext*, const IntRect&)
{
     D_DEBUG_AT( WebKit_Icon, "Icon::%s()\n", __func__ );

     D_UNIMPLEMENTED();
}

} // namespace WebCore

