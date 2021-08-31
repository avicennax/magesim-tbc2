using namespace std;

// enum ActionType {
//     SPELL,
//     COOLDOWN
// };

// union ActionU {
//     spell::Spell s;
//     cooldown::Cooldown c;
// };

// typedef struct _Action {
//     ActionType type;
//     ActionU action;
// } Action;

/**
 * Policy specifies action for mage to take given
 * the simulation state.
 * 
 * How do we we initialize a Policy?
 * If we're in Python land we just need to provide
 * the ID of spell to select the spell action.
 * 
 */
class Policy
{
public:
    bool use_default;

    Policy()
    {
        use_default = true;
    }

    shared_ptr<spell::Spell> action()
    {
        if (use_default)
            return NULL;
    }

    // int action(shared_ptr<spell::Spell> spell)
    // {
    //     printf("Doing an action!\n");
    //     return 42;
    // }

};