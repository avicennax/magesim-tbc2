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
#include "player.h"
#include "simulation.h"
#include "bindings.h"

using namespace std;

extern "C" int run()
{
    int runs = 1;
    bool log_mana = false;

    shared_ptr<Config> config(new Config());
    shared_ptr<Player> player(new Player(config));
    player->quickReady();
    shared_ptr<Simulation> sim(new Simulation(config, player));

    auto start = chrono::high_resolution_clock::now();
    
    sim->logging = true;
    SimulationResult result = sim->run();
    sim->printLog();

    printf("Damage: %d\n", result.dmg);
    printf("DPS: %.2f\n", result.dps);

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    printf("Exec time: %ldms\n", duration.count());

    return 0;
}