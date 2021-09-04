# distutils: language = c++

from testsim.simulation cimport Simulation 
from libcpp.memory cimport shared_ptr, make_shared
from cython.operator cimport dereference as deref

cdef class Sim:
    cdef Simulation sim

    def __cinit__(self):
        self.sim = Simulation()

    def run(self):
        return self.sim.run()

    def print_log(self):
        self.sim.printLog()

    def step(self):
        pass