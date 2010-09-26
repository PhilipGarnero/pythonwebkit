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
 * with JSCSSValueCustom.cpp.  any additions to JSCSSValueCustom.cpp will also
 * require the EXACT same additions, here.
 *
 * FIXME: there should have been no need to duplicate the functionality behind
 * JSDOMBinding.cpp and call it PythonBinding.cpp in the first place, and
 * there should be no need for this file; the functionality should be
 * merged into common code, as it does exactly the same thing.
 */

#include "config.h"

#include <Python.h>

#include "CString.h"
#include "PythonBinding.h"
#include "WebkitCSSValue.h"
#include "WebkitCSSValueList.h"
#include "WebkitCSSPrimitiveValue.h"

#if ENABLE(SVG)
#ifdef __TODO_BUG_20586__ /* TODO - see #20586 */
#include "WebkitSVGColor.h"
#include "WebkitSVGPaint.h"
#endif
#endif

namespace WebKit {

using namespace WebCore;

PyObject* toPython(CSSValue* value)
{
    if (!value)
        return NULL;

    PyObject* pobj = PythonObjectCache::getDOMObject(value);

    if (pobj)
        return pobj;

    PyObject* ret;

    if (value->isValueList())
        ret = wrapCSSValueList(static_cast<CSSValueList*>(value));
#if ENABLE(SVG)
    else if (value->isSVGPaint())
    {
        return NULL; /* TODO - see #20586 */
#ifdef __TODO_BUG_20586__ /* TODO - see #20586 */
        ret = wrapSVGPaint(static_cast<SVGPaint*>(value));
#endif
    }
    else if (value->isSVGColor())
    {
        return NULL; /* TODO - see #20586 */
#ifdef __TODO_BUG_20586__ /* TODO - see #20586 */
        ret = wrapSVGColor(static_cast<SVGColor*>(value));
#endif
    }
#endif
    else if (value->isPrimitiveValue())
        ret = wrapCSSPrimitiveValue(static_cast<CSSPrimitiveValue*>(value));
    else
        ret = wrapCSSValue(value);

    return PythonObjectCache::putDOMObject(value, ret);
}


} // namespace WebKit

