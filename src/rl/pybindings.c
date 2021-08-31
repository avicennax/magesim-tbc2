#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"

#include "interface.h"

#define DOUBLE_TO_STR(x) PyOS_double_to_string(x, 'f', 2, 0, NULL)

/**
 * Get simulation's state attributes. 
 */
typedef struct {
    PyObject_HEAD
    double t;
    double t_gcd;
    double t_mana_spent;
    double mana;
    int dmg;
    int regen_cycle;
    int regen_active; // Boolean
    int innervates;
    int mana_emerald;
    int mana_ruby;
    int combustion;
    double duration;
    double evocated_at;
    double regened_at;
} PySimState;


static PyMemberDef PySimState_members[] = {
    {"t", T_DOUBLE, offsetof(PySimState, t), 0, "Time"},
    {"t_gcd", T_DOUBLE, offsetof(PySimState, t_gcd), 0, "GCD"},
    {"t_mana_spent", T_DOUBLE, offsetof(PySimState, t_mana_spent), 0, "Time Mana Spent"},
    {"mana", T_DOUBLE, offsetof(PySimState, mana), 0, "Mana"},
    {"dmg", T_INT, offsetof(PySimState, dmg), 0, "Total damage"},
    {"regen_cycle", T_INT, offsetof(PySimState, regen_cycle), 0, ""},
    {"regen_active", T_INT, offsetof(PySimState, regen_active), 0, ""},
    {"innervates", T_INT, offsetof(PySimState, innervates), 0, ""},
    {"mana_emerald", T_INT, offsetof(PySimState, mana_emerald), 0, ""},
    {"mana_ruby", T_INT, offsetof(PySimState, mana_ruby), 0, ""},
    {"combustion", T_INT, offsetof(PySimState, combustion), 0, ""},
    {"duration", T_DOUBLE, offsetof(PySimState, duration), 0, ""},
    {"evocated_at", T_DOUBLE, offsetof(PySimState, evocated_at), 0, ""},
    {"regened_at", T_DOUBLE, offsetof(PySimState, regened_at), 0, ""},
    {NULL}  /* Sentinel */
};

static int
PySimState_Init(PySimState *self, PyObject *args, PyObject *kwds)
{
    static char *kwlist[] = {
        "t", "t_gcd","t_mana_spent", "mana",
        "dmg", "regen_cycle", "regen_active", "innervates",
        "mana_emerald", "mana_ruby", "combustion",
        "duration", "evocated_at", "regened_at",
        NULL
    };
    if (!PyArg_ParseTupleAndKeywords(
            args, kwds, "ddddiiiiiiiddd", kwlist,
            &self->t, &self->t_gcd, &self->t_mana_spent, &self->mana,
            &self->dmg, &self->regen_cycle, &self->regen_active, &self->innervates,
            &self->mana_emerald, &self->mana_ruby, &self->combustion,
            &self->duration, &self->evocated_at, &self->regened_at
        )) { return - 1; }

    return 0;
}

static PyObject*
PySimState_Repr(PySimState *self)
{
    return PyUnicode_FromFormat(
        "SimState(t = %s, t_gcd = %s, t_mana_spent = %s, mana = %s, dmg = %d)", 
        DOUBLE_TO_STR(self->t), DOUBLE_TO_STR(self->t_gcd), DOUBLE_TO_STR(self->t_mana_spent),
        DOUBLE_TO_STR(self->mana), self->dmg
    );
        // self->dmg, self->regen_cycle, self->regen_active, self->innervates,
        // self->mana_emerald, self->mana_ruby, self->combustion,
        // self->duration, self->evocated_at, self->regened_at
    // );
}

static PyTypeObject PySimStateType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "magesim.SimState",
    .tp_doc = "MageSim simulation state",
    .tp_basicsize = sizeof(PySimState),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
    .tp_init = (initproc) PySimState_Init,
    .tp_repr = (reprfunc) PySimState_Repr,
    .tp_members = PySimState_members
};

/**
 * Returns result of a simulation run. 
 */
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
    .tp_name = "magesim.RunResult",
    .tp_doc = "MageSim run result",
    .tp_basicsize = sizeof(PyRunResult),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
    .tp_init = (initproc) PyRunResult_Init,
    .tp_members = PyRunResult_members
};

/**
 * Provides access to a simulation instance.
 */
typedef struct {
    PyObject_HEAD
    const char* name;
} PySimulation;

static PyObject * PySimulation_run(PySimulation *self, PyObject *args);
static PyObject * PySimulation_step(PySimulation *self, PyObject *args);
static PyObject * PySimulation_init(PySimulation *self, PyObject *args);

static PyMemberDef PySimulation_members[] = {
    {"name", T_STRING, offsetof(PySimulation, name), 0, "Name of simulation"},
    {NULL}  /* Sentinel */
};

static int
PySimulation_Init(PySimulation *self, PyObject *args, PyObject *kwds)
{
    static char *kwlist[] = {"name", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "s", kwlist, &self->name))
        return -1;

    init(self->name);

    return 0;
}

static void
PySimulation_Dealloc(PySimulation *self)
{
    del(self->name);
    Py_TYPE(self)->tp_free((PyObject *) self);
}

static PyMethodDef PySimulation_methods[] = {
    {"run", (PyCFunction) PySimulation_run, METH_VARARGS, "Run MageSim simulation"},
    {"step", (PyCFunction) PySimulation_step, METH_VARARGS, "Initialize and run a single step of MageSim simulation"},
    {"init", (PyCFunction) PySimulation_init, METH_VARARGS, "Initialize MageSim simulation"},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

static PyTypeObject PySimulationType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "magesim.Simulation",
    .tp_doc = "MageSim Instance",
    .tp_basicsize = sizeof(PySimulation),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
    .tp_dealloc = (destructor) PySimulation_Dealloc,
    .tp_init = (initproc) PySimulation_Init,
    .tp_members = PySimulation_members,
    .tp_methods = PySimulation_methods
};

static PyObject *
PySimulation_init(PySimulation *self, PyObject *args)
{
   init(self->name);
   return Py_None;
}

static PyObject *
PySimulation_run(PySimulation *self, PyObject *args)
{
    int print;

    if (!PyArg_ParseTuple(args, "p", &print))
        return NULL;

    RunResult runResult = run(self->name, print);
    PyObject* pyRunR = PyObject_CallFunction((PyObject *) &PyRunResultType, "id", runResult.dmg, runResult.dps);

    return pyRunR;
}

static PyObject *
PySimulation_step(PySimulation *self, PyObject *args)
{
    int action;

    if (!PyArg_ParseTuple(args, "i", &action))
        return NULL;

    SimState state = step(self->name, action);
    PyObject* pyState = PyObject_CallFunction(
        (PyObject *) &PySimStateType,
            "ddddiiiiiiiddd",
            state.t, state.t_gcd, state.t_mana_spent, state.mana,
            state.dmg, state.regen_cycle, state.regen_active, state.innervates,
            state.mana_emerald, state.mana_ruby, state.combustion,
            state.duration, state.evocated_at, state.regened_at
    );

    return pyState;
}


/**
 * Define module boilerplate.
 */
static PyObject* MageSimError;
static PyObject* magesim_run(PyObject *self, PyObject *args);
static PyObject *magesim_get_simulations(PyObject *self, PyObject *args);

static PyMethodDef MageSimMethods[] = {
    {"run", magesim_run, METH_VARARGS, "Run MageSim simulation"},
    {"get_simulations", magesim_get_simulations, METH_VARARGS, "Get names of simulation instances"},
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

    // --
    if (PyType_Ready(&PySimulationType) < 0)
        return NULL;

    Py_INCREF(&PySimulationType);
    if (PyModule_AddObject(m, "Simulation", (PyObject *) &PySimulationType) < 0) {
        Py_DECREF(&PySimulationType);
        Py_DECREF(m);
        return NULL;
    }

    // --
    if (PyType_Ready(&PyRunResultType) < 0)
        return NULL;

    Py_INCREF(&PyRunResultType);
    if (PyModule_AddObject(m, "RunResult", (PyObject *) &PyRunResultType) < 0) {
        Py_DECREF(&PyRunResultType);
        Py_DECREF(m);
        return NULL;
    }

   // --
    if (PyType_Ready(&PySimStateType) < 0)
        return NULL;

    Py_INCREF(&PySimStateType);
    if (PyModule_AddObject(m, "RunResult", (PyObject *) &PySimStateType) < 0) {
        Py_DECREF(&PySimStateType);
        Py_DECREF(m);
        return NULL;
    }

    // --
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
    const char* name;

    if (!PyArg_ParseTuple(args, "sp", &name, &print))
        return NULL;

    RunResult runResult = run(name, print);
    PyObject* pyRunR = PyObject_CallFunction((PyObject *) &PyRunResultType, "id", runResult.dmg, runResult.dps);

    return pyRunR;
}

static PyObject *
magesim_get_simulations(PyObject *self, PyObject *args)
{
    const char** p;
    size_t size = simulationNames(&p);
    PyObject* tuple = PyTuple_New(size);

    for (size_t i = 0; i < size; i++) {
        PyTuple_SetItem(tuple, i, PyUnicode_FromString(*(p++)));
    }

    return tuple;
}