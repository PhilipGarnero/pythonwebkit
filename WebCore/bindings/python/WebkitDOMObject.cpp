/*
 * Copyright (C) 2008 Martin Soto <soto@freedesktop.org>
 * Copyright (C) 2008 Alp Toker <alp@atoker.com>
 * Copyright (C) 2008 Apple Inc.
 * Copyright (C) 2010 Free Software Foundation
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

#include "WebkitBinding.h"
#include "WebkitDOMObject.h"
#include "WebkitDOMObjectPrivate.h"

extern "C" {

G_DEFINE_TYPE(WebkitDOMObject, webkit_dom_object, G_TYPE_OBJECT);

static void webkit_dom_object_init(WebkitDOMObject* request)
{
    WebkitDOMObjectPrivate* priv = WEBKIT_DOM_OBJECT_GET_PRIVATE(request);
    request->priv = priv;
}

static void webkit_dom_object_class_init(WebkitDOMObjectClass* requestClass)
{
    g_type_class_add_private(requestClass, sizeof(WebkitDOMObjectPrivate));
}

} /* extern "C" */

