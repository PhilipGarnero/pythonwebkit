#include <Python.h>

#include "config.h"

#include "DOMWindow.h"
#include "Document.h"
#include "XMLHttpRequest.h"

namespace WebKit {
PyObject* toPython(WebCore::Document*);
PyObject* toPython(WebCore::DOMWindow*);
PyObject* toPython(WebCore::XMLHttpRequest*);
}

extern "C" {
PyObject *toPythonFromDocumentPtr(gpointer ptr)
{
    WebCore::Document *doc = static_cast<WebCore::Document*>(ptr);
    return WebKit::toPython(doc);
}

PyObject *toPythonFromDOMWindowPtr(gpointer ptr)
{
    WebCore::DOMWindow *win = static_cast<WebCore::DOMWindow*>(ptr);
    return WebKit::toPython(win);
}

PyObject *toPythonFromXMLHttpRequestPtr(gpointer ptr)
{
    WebCore::XMLHttpRequest *xhr = static_cast<WebCore::XMLHttpRequest*>(ptr);
    return WebKit::toPython(xhr);
}
};

