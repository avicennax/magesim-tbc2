#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <memory>
#include <map>
#include <vector>
#include <string>
#include <list>
#include <math.h>
#include <random>
#include <sstream>
#include <iomanip>

#include "common.h"
#include "config.h"
#include "stats.h"
#include "talents.h"
#include "buff.h"
#include "debuff.h"
#include "dot.h"
#include "cooldown.h"
#include "spell.h"
#include "state.h"
#include "policy.h"
#include "player.h"
#include "simulation.h"
#include "bindings.h"
#include "interface.h"

using namespace std;

class SimulationNotFound: public exception
{
    virtual const char* msg() const throw()
    {
        return "Simulation not found.";
    }
} simNotFound;

// Can't use static members in mixed C/C++ code.
// https://www.geeksforgeeks.org/static-variables-in-c/
map<string, shared_ptr<Simulation>> SIMULATIONS;
class SimulationManager
{
public:
    shared_ptr<Simulation> createSimulation(string name)
    {
        shared_ptr<Config> config(new Config());
        shared_ptr<Player> player(new Player(config));
        shared_ptr<Policy> policy(new Policy());
        player->quickReady();
        shared_ptr<Simulation> sim(new Simulation(config, player, policy));

        SIMULATIONS[name] = sim;
        printf("Creating Sim: %s\n", name.c_str());
        return sim;
    }

    shared_ptr<Simulation> getSimulation(string name, bool createIfNotFound = true)
    {
        if (SIMULATIONS.find(name) == SIMULATIONS.end())
            if (createIfNotFound)
                return createSimulation(name);
            else
                throw simNotFound;
        else
            return SIMULATIONS[name];
    }
};
SimulationManager SIM_MANAGER;
vector<const char*> SIM_NAMES;

extern "C" size_t simulationNames(const char*** p)
{
    SIM_NAMES.clear();
    for (auto const& smap: SIMULATIONS) {
        SIM_NAMES.push_back(smap.first.c_str());
    }

    *p = SIM_NAMES.data();
    return SIM_NAMES.size();
}

extern "C" void init(const char* name)
{
    auto n = string(name);
    shared_ptr<Simulation> sim(SIM_MANAGER.getSimulation(n));
    sim->initRun();
}

extern "C" void del(const char* name)
{
    SIMULATIONS.erase(string(name));
    printf("Simulation '%s' removed.\n", name);
}

extern "C" SimState step(const char* name, int action)
{
    auto n = string(name);
    shared_ptr<Simulation> sim(SIM_MANAGER.getSimulation(n, false));
    
    shared_ptr<State> state = sim->step(action);

    printf("state->t: %f\n", state->t);

    return SimState {
        state->t,
        state->t_gcd,
        state->t_mana_spent,
        state->mana,
        state->dmg,
        state->regen_cycle,
        (int) state->regen_active,
        state->innervates,
        state->mana_emerald,
        state->mana_ruby,
        state->combustion,
        state->duration,
        state->evocated_at,
        state->regened_at
    };
}

extern "C" RunResult run(const char* name, int print)
{
    int runs = 1;
    bool log_mana = false;

    string n = string(name);
    shared_ptr<Simulation> sim(SIM_MANAGER.getSimulation(n));
    auto start = chrono::high_resolution_clock::now();
    
    sim->logging = true;
    SimulationResult result = sim->run();
    if (print)
        sim->printLog();

    printf("Damage: %d\n", result.dmg);
    printf("DPS: %.2f\n", result.dps);

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    printf("Exec time: %ldms\n", duration.count());

    RunResult runR {result.dmg, result.dps};

    return runR;
}