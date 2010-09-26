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

/* 
 * FIXME: there should have been no need to duplicate the functionality behind
 * JSDOMBinding.cpp and call it PythonBinding.cpp in the first place.
 * There should be no need for this file; the functionality should be
 * merged into common code, as it does exactly the same thing.
 *
 * however, it is being added near-verbatim-cut-paste because a) that is
 * easier b) it's exactly what the objective-c bindings do
 *
 */

// gcc 3.x can't handle including the HashMap pointer specialization
// in this file
#if defined __GNUC__ && !defined __GLIBCXX__ // less than gcc 3.4
#define HASH_MAP_PTR_SPEC_WORKAROUND 1
#endif

#include "config.h"

#include "CString.h"
#include "PythonBinding.h"
#include "WebkitAttrPrivate.h"
#include "WebkitCDATASectionPrivate.h"
#include "WebkitCommentPrivate.h"
#include "WebkitDocumentPrivate.h"
#include "WebkitDocumentTypePrivate.h"
#include "WebkitDocumentFragmentPrivate.h"
#include "WebkitDOMObject.h"
#include "WebkitDOMObjectPrivate.h"
#include "WebkitHTMLElementWrapperFactory.h"
#include "WebkitElementPrivate.h"
#include "WebkitEntityPrivate.h"
#include "WebkitEntityReferencePrivate.h"
#include "WebkitNodePrivate.h"
#include "WebkitNotationPrivate.h"
#include "WebkitProcessingInstructionPrivate.h"
#include "WebkitTextPrivate.h"
#include "HTMLElement.h"

namespace WebKit {

using namespace WebCore;

typedef HashMap<void*, PyObject*> DOMObjectMap;

static DOMObjectMap& domObjects()
{
    static DOMObjectMap staticDOMObjects;
    return staticDOMObjects;
}

PyObject* PythonObjectCache::getDOMObject(void* objectHandle)
{
    PyObject* ret = domObjects().get(objectHandle);
    return ret;
}

PyObject* PythonObjectCache::putDOMObject(void* objectHandle, PyObject* wrapper)
{
    domObjects().set(objectHandle, wrapper);
    return wrapper;
}

void PythonObjectCache::forgetDOMObject(void* objectHandle)
{
    domObjects().take(objectHandle);
}

static PyObject* createWrapper(Node* node);

PyObject* toPythonNewlyCreated(Node* node)
{
    if (!node)
        return NULL;

    return createWrapper(node);
}

PyObject* toPython(Node* node)
{
    if (!node)
        return NULL;

    PyObject* ret = PythonObjectCache::getDOMObject(node);
    if (ret)
        return ret;

    return createWrapper(node);
}

static ALWAYS_INLINE PyObject* createWrapper(Node* node)
{
    ASSERT(node);
    ASSERT(!ScriptInterpreter::getDOMObject(node));

    Document* doc = node->document();
    PyObject* ret = NULL;

    switch (node->nodeType()) {
        case Node::ELEMENT_NODE:
            if (node->isHTMLElement())
                ret = createPythonHTMLElementWrapper(
                                        static_cast<HTMLElement*>(node));
#if ENABLE(SVG)
            else if (node->isSVGElement())
                return NULL; /* XXX TODO - see #20586 */
#ifdef __TODO_BUG_20586__ /* XXX TODO - see #20586 */
                ret = createPythonSVGElementWrapper(
                                        static_cast<SVGElement*>(node));
#endif
#endif
            else
                ret = wrapElement(static_cast<Element*>(node));
            break;
        case Node::ATTRIBUTE_NODE:
            ret = wrapAttr(static_cast<Attr*>(node));
            break;
        case Node::TEXT_NODE:
            ret = wrapText(static_cast<Text*>(node));
            break;
        case Node::CDATA_SECTION_NODE:
            ret = wrapCDATASection(static_cast<CDATASection*>(node));
            break;
        case Node::ENTITY_NODE:
            ret = wrapEntity(static_cast<Entity*>(node));
            break;
        case Node::PROCESSING_INSTRUCTION_NODE:
            ret = wrapProcessingInstruction(
                                    static_cast<ProcessingInstruction*>(node));
            break;
        case Node::COMMENT_NODE:
            ret = wrapComment(static_cast<Comment*>(node));
            break;
        case Node::DOCUMENT_NODE:
            // we don't want to cache the document itself in
            // the per-document dictionary
            return toPython(static_cast<Document*>(node));
        case Node::DOCUMENT_TYPE_NODE:
            ret = wrapDocumentType(static_cast<DocumentType*>(node));
            break;
        case Node::NOTATION_NODE:
            ret = wrapNotation(static_cast<Notation*>(node));
            break;
        case Node::DOCUMENT_FRAGMENT_NODE:
            ret = wrapDocumentFragment(static_cast<DocumentFragment*>(node));
            break;
        case Node::ENTITY_REFERENCE_NODE:
            ret = wrapEntityReference(static_cast<EntityReference*>(node));
            break;
        default:
            ret = wrapNode(node);
    }

    return PythonObjectCache::putDOMObject(node, ret);
}


} // namespace WebKit

