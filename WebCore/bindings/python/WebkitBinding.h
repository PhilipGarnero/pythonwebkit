/*
 *  Copyright (C) 1999-2001 Harri Porten (porten@kde.org)
 *  Copyright (C) 2003, 2004, 2005, 2006, 2008 Apple Inc. All rights reserved.
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

#ifndef WebkitDOMBinding_h
#define WebkitDOMBinding_h

#ifndef gpointer
#define gpointer unsigned long
#endif

#include "CSSRule.h"
#include "CSSValue.h"
#include "Element.h"
#include "Event.h"
#include "EventTarget.h"
#include "WebkitNode.h"
#include "WebkitNodePrivate.h"
#include "HTMLCollection.h"
#include "Node.h"
#include "StyleSheet.h"
#include "Text.h"
#include <wtf/Noncopyable.h>

namespace WebCore {

    class AtomicString;
    class Document;
    class Event;
    class Frame;
    class KURL;
    class Node;
    class String;

#if ENABLE(SVG)
    class SVGElement;
#endif
} // namespace WebCore

namespace WebKit {
    gpointer toWEBKIT(WebCore::CSSRule*);
    gpointer toWEBKIT(WebCore::CSSValue*);
    gpointer toWEBKIT(WebCore::Document*);
    gpointer toWEBKIT(WebCore::Element*);
    gpointer toWEBKIT(WebCore::Event*);
    gpointer toWEBKIT(WebCore::EventTarget*);
    gpointer toWEBKIT(WebCore::HTMLCollection*);
    gpointer toWEBKIT(WebCore::Node*);
    gpointer toWEBKIT(WebCore::StyleSheet*);
    gpointer toWEBKIT(WebCore::Text*);

    class WEBKITObjectCache {
    public:
        static gpointer getDOMObject(void *);
        static gpointer putDOMObject(void *, gpointer);
        static void forgetDOMObject(void *);
    };
} // namespace WebKit

#endif // WebkitDOMBinding_h
