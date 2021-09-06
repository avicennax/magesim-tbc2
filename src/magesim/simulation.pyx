# distutils: language = c++

from magesim.simulation cimport Simulation, setRNGSeed


cdef class Sim:
    cdef Simulation sim
    cdef int _seed

    def __cinit__(self):
        self.sim = Simulation()
        # Same as default in config.h.
        self._seed = 0

    def bootstrap(self):
        self.sim.bootstrapRun()

    def run(self):
        return self.sim.run()

    def step(self, action_id):
        s = self.sim.step(action_id)
        return s

    def print_log(self):
        self.sim.printLog()

    def reset(self):
        return self.sim.envReset()

    def seed(self, _seed):
        setRNGSeed(_seed)
        self._seed = _seed