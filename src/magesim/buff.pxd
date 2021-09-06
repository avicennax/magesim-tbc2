from libcpp cimport bool
from libcpp.string cimport string

# TODO: use for more detail state 
cdef extern from "../buff.h" namespace "buff":
    cdef enum ID:
        NONE = 0
        POTION = 1
        CONJURED = 2
        TRINKET1 = 3
        TRINKET2 = 4
        EVOCATION = 12051
        COLD_SNAP = 11958
        BERSERKING = 20554
        ICY_VEINS = 12472
        COMBUSTION = 29977
        ARCANE_POWER = 12042
        PRESENCE_OF_MIND = 12043
        POWER_INFUSION = 10060
        SYMBOL_OF_HOPE = 32548
        QUAGMIRRANS_EYE = 33370
        UNSTABLE_CURRENTS = 38348
        ETERNAL_SAGE = 35084
        SPELL_BLASTING = 25906
        DRUMS = 35476
        LIGHTNING_CAPACITOR = 37657
        INSIGHTFUL_EARTHSTORM = 27521
        MYSTICAL_SKYFIRE = 32837
        MARK_OF_DEFIANCE = 33511
        CALL_OF_THE_NEXUS = 34320
        FORGOTTEN_KNOWLEDGE = 38319
        POWER_OF_ARCANAGOS = 34598

    cdef struct BuffStruct:
        ID id
        string name
        double duration
        int stacks
        int max_stacks
        bool hidden
