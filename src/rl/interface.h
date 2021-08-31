#ifdef __cplusplus
extern "C" {  
#endif  

typedef struct RunResult {
    int dmg;
    double dps;
} RunResult;

typedef struct SimState {
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
} SimState;

size_t simulationNames(const char*** p);
RunResult run(const char* name, int print);
SimState step(const char* name, int action);
void del(const char* name);
void init(const char* name);


#ifdef __cplusplus  
} // extern "C"  
#endif