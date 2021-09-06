from libcpp.string cimport string
from libcpp.map cimport map
from libcpp cimport bool


cdef extern from "imports.h":
    pass


cdef extern from "../common.h":
    cdef struct SimulationResult:
        int dmg
        double t, dps, evocated_at, regened_at
        string log

    cdef enum Spec:
        SPEC_ARCANE
        SPEC_FIRE
        SPEC_FROST

    void setRNGSeed(int seed)


cdef extern from "../env.h" namespace "env":
    cdef struct State:
        double t, gcd, mana, duration
        int dmg, mana_emerald, mana_ruby
        Spec spec


cdef extern from "../simulation.h":
    cdef cppclass Simulation:
        Simulation() except +
        void bootstrapRun()
        State envReset()
        SimulationResult run()
        State step(int)
        bool printLog()