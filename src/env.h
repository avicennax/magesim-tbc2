#ifndef ENV_H
#define ENV_H

namespace env
{
    // Cython will automatically cast structs to dictionaries for us.
    // http://docs.cython.org/en/latest/src/userguide/external_C_code.html?highlight=struct
    typedef struct State
    {
        public:
            double t;
            double gcd;
            double mana;
            double duration;
            Spec spec;
            int dmg;
            int mana_emerald;
            int mana_ruby;
    } State;
}

#endif