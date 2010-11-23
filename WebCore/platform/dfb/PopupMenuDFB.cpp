/*
 * This file is part of the popup menu implementation for <select> elements in WebCore.
 *
 * Copyright (C) 2007 Ryan Leavengood <leavengood@gmail.com>
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
#include "PopupMenu.h"
#include "PopupMenuDFB.h"

#include "FrameView.h"

#include "NotImplemented.h"



namespace WebCore {

PopupMenuDFB::PopupMenuDFB(PopupMenuClient* client)
    : m_popupClient(client)
{
    // We don't need additional references to the client, since we completely
    // control any sub-objects we create that need it as well.
}

PopupMenuDFB::~PopupMenuDFB()
{
}

void PopupMenuDFB::disconnectClient()
{
    m_popupClient = 0;
}

void PopupMenuDFB::show(const IntRect& rect, FrameView* view, int index)
{
}

void PopupMenuDFB::hide()
{
}

void PopupMenuDFB::updateFromElement()
{
    client()->setTextFromItem(m_popupClient->selectedIndex());
}

} // namespace WebCore

