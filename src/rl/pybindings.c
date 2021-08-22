#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"

#include "run.h"

typedef struct {
    PyObject_HEAD
    int damage;
    double dps;
} PyRunResult;

static PyMemberDef PyRunResult_members[] = {
    {"damage", T_INT, offsetof(PyRunResult, damage), 0, "Total damage"},
    {"dps", T_DOUBLE, offsetof(PyRunResult, dps), 0, "DPS"},
    {NULL}  /* Sentinel */
};

static int
PyRunResult_Init(PyRunResult *self, PyObject *args, PyObject *kwds)
{
    static char *kwlist[] = {"damage", "dps", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "id", kwlist, &self->damage, &self->dps))
        return -1;

    return 0;
}

static PyTypeObject PyRunResultType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "magsim.RunResult",
    .tp_doc = "MageSim run result",
    .tp_basicsize = sizeof(PyRunResult),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
    .tp_init = (initproc) PyRunResult_Init,
    .tp_members = PyRunResult_members
};

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

   if (PyType_Ready(&PyRunResultType) < 0)
        return NULL;

    m = PyModule_Create(&magesimmodule);
    if (m == NULL)
        return NULL;

    Py_INCREF(&PyRunResultType);
    if (PyModule_AddObject(m, "RunResult", (PyObject *) &PyRunResultType) < 0) {
        Py_DECREF(&PyRunResultType);
        Py_DECREF(m);
        return NULL;
    }

    MageSimError = PyErr_NewException("magsim.error", NULL, NULL);
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
    int print;

    if (!PyArg_ParseTuple(args, "p", &print))
        return NULL;

    RunResult runResult = run(print);
    PyObject* pyRunR = PyObject_CallFunction((PyObject *) &PyRunResultType, "id", runResult.dmg, runResult.dps);

    return pyRunR;
}