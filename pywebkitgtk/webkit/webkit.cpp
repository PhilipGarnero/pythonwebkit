#include <Python.h>

#include "config.h"

#include "DOMWindow.h"
#include "Document.h"
#include "XMLHttpRequest.h"

PyObject* toPython(WebCore::Document*);
PyObject* toPython(WebCore::DOMWindow*);
PyObject* toPython(WebCore::XMLHttpRequest*);

PyObject *toPythonFromDocumentPtr(gpointer ptr)
{
    WebCore::Document *doc = static_cast<WebCore::Document*>(ptr);
    return toPython(doc);
}

PyObject *toPythonFromDOMWindowPtr(gpointer ptr)
{
    WebCore::DOMWindow *win = static_cast<WebCore::DOMWindow*>(ptr);
    return toPython(win);
}

PyObject *toPythonFromXMLHttpRequestPtr(gpointer ptr)
{
    WebCore::XMLHttpRequest *xhr = static_cast<WebCore::XMLHttpRequest*>(ptr);
    return toPython(xhr);
}

