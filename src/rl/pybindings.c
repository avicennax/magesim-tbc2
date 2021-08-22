#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "run.h"

static PyObject *MageSimError;
static PyObject *magesim_run(PyObject *self, PyObject *args);

static PyMethodDef MageSimMethods[] = {
    {"run", magesim_run, METH_VARARGS, "Run MageSim Simulation"},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

static struct PyModuleDef magesimmodule = {
    PyModuleDef_HEAD_INIT,
    "magesim",   /* name of module */
    NULL, /* module documentation, may be NULL */
    -1,       /* size of per-interpreter state of the module,
                 or -1 if the module keeps state in global variables. */
    MageSimMethods
};

PyMODINIT_FUNC
PyInit_magesim(void)
{
    PyObject *m;

    m = PyModule_Create(&magesimmodule);
    if (m == NULL)
        return NULL;

    MageSimError = PyErr_NewException("spam.error", NULL, NULL);
    Py_XINCREF(MageSimError);
    if (PyModule_AddObject(m, "error", MageSimError) < 0) {
        Py_XDECREF(MageSimError);
        Py_CLEAR(MageSimError);
        Py_DECREF(m);
        return NULL;
    }

    return m;
}

static PyObject *
magesim_run(PyObject *self, PyObject *args)
{
    const char *command;
    int sts;

    if (!PyArg_ParseTuple(args, "s", &command))
        return NULL;
    sts = run();
    if (sts < 0) {
        PyErr_SetString(MageSimError, "System command failed");
        return NULL;
    }
    return PyLong_FromLong(sts);
}