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
 * with JSHTMLCollectionCustom.cpp.  any additions to
 * JSHTMLCollectionCustom.cpp will also require the EXACT same additions, here.
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
#include "WebkitBinding.h"
#include "WebkitDOMObject.h"
#include "WebkitDOMObjectPrivate.h"
#include "WebkitHTMLCollectionPrivate.h"
#include "WebkitHTMLOptionsCollectionPrivate.h"

namespace WebKit {

using namespace WebCore;

gpointer toWEBKIT(HTMLCollection* collection)
{
    if (!collection)
        return NULL;

    gpointer gobj = WEBKITObjectCache::getDOMObject(collection);

    if (gobj)
        return gobj;

    gpointer ret;
    switch (collection->type()) {
        case SelectOptions:
            ret = wrapHTMLOptionsCollection(static_cast<HTMLOptionsCollection*>(collection));
            break;
        /* FIXME: rather than delete this code and make it awkward for
         * whomever has to add it back in to work out what is going on,
         * in order to comply with WebKit style coding standards, #if 0
         * is not used here, but a long-winded #ifdef which will never
         * exist is deployed instead.
         * 
         * see JSHTMLCollectionCustom::toJS, that is the code from which
         * this entire function has been verbatim cut/paste.
         */
#ifdef __FIXME_ADD_IN_HTMLCOLLECTION_DOC_ALL_SUPPORT_LATER__
        case HTMLCollection::DocAll:
            ret = wrapHTMLCollection(static_cast<HTMLCollection*>(collection));
            break;
#endif
        default:
            ret = wrapHTMLCollection(static_cast<HTMLCollection*>(collection));
            break;
    }

    return WEBKITObjectCache::putDOMObject(collection, ret);
}


} // namespace WebKit

