/*
 * Copyright (C) 2006-2007, Red Hat, Inc.
 * Copyright (C) 2009 Jan Michael Alonzo <jmalonzo@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */
extern "C" {
#include <Python.h>

#include <glib.h>
/* include this first, before NO_IMPORT_PYGOBJECT is defined */
#include <pygobject.h>
#include <pygtk/pygtk.h>

#include "pywebkit.h"

extern PyMethodDef pywebkit_functions[];

void pywebkit_register_classes (PyObject *d);
void webkit_init_pywebkit(PyObject *m, struct pyjoinapi *api_fns);

struct pyjoinapi pywebkit_api_fns;

DL_EXPORT(void)
initpywebkit(void)
{
    PyObject *m, *d;

    if (!pygobject_init(-1, -1, -1)) {
        PyErr_Print();
        Py_FatalError ("can't initialise module gobject");
    }

    init_pygtk();

    /* webkit module */
    m = Py_InitModule ("pywebkit", pywebkit_functions);
    d = PyModule_GetDict (m);
    pywebkit_register_classes (d);
    webkit_init_pywebkit(m, &pywebkit_api_fns);

    if (PyErr_Occurred ()) {
        PyErr_Print();
        Py_FatalError ("can't initialise module webkit.gjs");
    }
}
}; // extern "C"
