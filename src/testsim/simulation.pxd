from libcpp.memory cimport shared_ptr
from libcpp.string cimport string
from libcpp cimport bool

cdef extern from "imports.h":
    pass

cdef extern from "../common.h":
    cdef struct SimulationResult:
        int dmg
        double t, dps, evocated_at, regened_at
        string log

cdef extern from "../simulation.h":
    cdef cppclass Simulation:
        Simulation() except +
        SimulationResult run()
        bool printLog()