#include <cstdio>
using namespace std;

typedef struct _ActionUpdate {
    double damage;
    shared_ptr<State> state;
} ActionUpdate;
