using namespace std;

enum ActionType {
    SPELL,
    COOLDOWN
};

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
        use_default = false;
    }

    shared_ptr<spell::Spell> action(int spellId)
    {
        if (use_default)
            return NULL;

        auto sp = spell::ID(spellId);

        switch(sp) {
            case spell::ID::ARCANE_BLAST:
                return make_shared<spell::ArcaneBlast>();
            case spell::ID::ARCANE_MISSILES:
                return make_shared<spell::ArcaneMissiles>();
            case spell::ID::FROSTBOLT:
                return make_shared<spell::Frostbolt>();
            case spell::ID::FIREBALL:
                return make_shared<spell::Fireball>();
            case spell::ID::SCORCH:
                return make_shared<spell::Scorch>();
            case spell::ID::PYROBLAST:
                return make_shared<spell::Pyroblast>();
            default:
                throw exception();
        }
    }
};