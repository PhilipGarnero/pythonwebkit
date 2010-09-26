/*
 *  Copyright (C) 1999-2001 Harri Porten (porten@kde.org)
 *  Copyright (C) 2004, 2005, 2006, 2007, 2008 Apple Inc. All rights reserved.
 *  Copyright (C) 2007 Samuel Weinig <sam@webkit.org>
 *  Copyright (C) 2008 Martin Soto <soto@freedesktop.org>
 *  Copyright (C) 2010 Free Software Foundation
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

/* PLEASE NOTE: this file needs to be kept up-to-date in EXACT accordance
 * with JSElementCustom.cpp.  any additions to JSElementCustom.cpp will also
 * require the EXACT same additions, here.
 *
 * FIXME: there should have been no need to duplicate the functionality behind
 * JSDOMBinding.cpp and call it WEBKITBinding.cpp in the first place, and
 * there should be no need for this file; the functionality should be
 * merged into common code, as it does exactly the same thing.
 */

#ifndef gpointer
#define gpointer unsigned long
#endif

#include "config.h"

#include "CString.h"
#include "Event.h"
#include "WebkitBinding.h"
#include "WebkitDOMObject.h"
#include "WebkitDOMObjectPrivate.h"
#include "WebkitElement.h"
#include "WebkitElementPrivate.h"
#include "WebkitHTMLElementWrapperFactory.h"
#include "HTMLElement.h"

#if ENABLE(SVG)
#ifdef __TODO_BUG_20586__ /* TODO - see #20586 */
#include "WebkitSVGElementWrapperFactoryPrivate.h"
#include "WebkitSVGElementWrapperFactory.h"
#include "SVGElement.h"
#endif
#endif

namespace WebKit {

using namespace WebCore;

gpointer toWEBKIT(Element* element)
{
    if (!element)
        return NULL;

    // shouldn't be one? ASSERT(!ScriptInterpreter::getDOMObject(element));
    gpointer gobj = WEBKITObjectCache::getDOMObject(element);
    if (gobj)
        return gobj;

    gpointer ret;
    Document* doc = element->document();

    if (element->isHTMLElement())
        ret = createWebkitHTMLElementWrapper(static_cast<HTMLElement*>(element));
#if ENABLE(SVG)
    else if (element->isSVGElement())
    {
        return NULL; /* TODO - see #20586 */
#ifdef __TODO_BUG_20586__ /* TODO - see #20586 */
        return createWebkitSVGElementWrapper(static_cast<SVGElement*>(element));
#endif
    }
#endif
    else
        ret = wrapElement(element);

    return WEBKITObjectCache::putDOMObject(element, ret);
}

} // namespace WebKit

