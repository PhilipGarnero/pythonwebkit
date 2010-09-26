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
 * with JSCSSRuleCustom.cpp.  any additions to JSCSSRuleCustom.cpp will also
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
#include "WebkitCSSRule.h"
#include "CSSCharsetRule.h"
#include "CSSFontFaceRule.h"
#include "CSSImportRule.h"
#include "CSSMediaRule.h"
#include "CSSPageRule.h"
#include "CSSStyleRule.h"
#include "CSSVariablesRule.h"
#include "WebkitBinding.h"
#include "WebkitCSSCharsetRule.h"
#include "WebkitCSSCharsetRulePrivate.h"
#include "WebkitCSSFontFaceRule.h"
#include "WebkitCSSFontFaceRulePrivate.h"
#include "WebkitCSSImportRule.h"
#include "WebkitCSSImportRulePrivate.h"
#include "WebkitCSSMediaRule.h"
#include "WebkitCSSMediaRulePrivate.h"
#include "WebkitCSSPageRule.h"
#include "WebkitCSSPageRulePrivate.h"
#include "WebkitCSSPrimitiveValuePrivate.h"
#include "WebkitCSSRulePrivate.h"
#include "WebkitCSSStyleRule.h"
#include "WebkitCSSStyleRulePrivate.h"
#include "WebkitCSSVariablesRule.h"
#include "WebkitCSSVariablesRulePrivate.h"
#include "WebkitCSSValueListPrivate.h"
#include "WebkitCSSValuePrivate.h"
#include "WebkitDOMObject.h"
#include "WebkitDOMObjectPrivate.h"

namespace WebKit {

using namespace WebCore;

gpointer toWEBKIT(CSSRule* rule)
{
    if (!rule)
        return NULL;

    gpointer gobj = WEBKITObjectCache::getDOMObject(rule);

    if (gobj)
        return gobj;

    gpointer ret;
    switch (rule->type()) {
        case CSSRule::STYLE_RULE:
            ret = wrapCSSStyleRule(static_cast<CSSStyleRule*>(rule));
            break;
        case CSSRule::MEDIA_RULE:
            ret = wrapCSSMediaRule(static_cast<CSSMediaRule*>(rule));
            break;
        case CSSRule::FONT_FACE_RULE:
            ret = wrapCSSFontFaceRule(static_cast<CSSFontFaceRule*>(rule));
            break;
        case CSSRule::PAGE_RULE:
            ret = wrapCSSPageRule(static_cast<CSSPageRule*>(rule));
            break;
        case CSSRule::IMPORT_RULE:
            ret = wrapCSSImportRule(static_cast<CSSImportRule*>(rule));
            break;
        case CSSRule::CHARSET_RULE:
            ret = wrapCSSCharsetRule(static_cast<CSSCharsetRule*>(rule));
            break;
        case CSSRule::VARIABLES_RULE:
            ret = wrapCSSVariablesRule(static_cast<CSSVariablesRule*>(rule));
            break;
        default:
            ret = wrapCSSRule(rule);
            break;
    }

    WEBKITObjectCache::putDOMObject(rule, ret);
    return ret;
}

} // namespace WebKit

