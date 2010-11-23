/*
 * Copyright (C) 2008 Christian Dywan <christian@imendio.com>
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
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "config.h"
#include "webkitrender.h"

#include <ChromeClientDFB.h>
#include <ContextMenuClientDFB.h>
#include <EditorClientDFB.h>
#include <DragClientDFB.h>
#include <InspectorClientDFB.h>
#include <FrameLoaderClientDFB.h>

#include <GraphicsContext.h>
#include <Frame.h>
#include <FrameView.h>
#include <Page.h>

#include <directfb.h>

/**
 * webkit_frame_render:
 *
 * Render...
 *
 * Since: 1.0.0
 */
void webkit_frame_render(IDirectFBSurface *surface)
{
}

