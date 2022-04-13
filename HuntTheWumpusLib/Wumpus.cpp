#include "Wumpus.h"

#include "Cave.h"
#include "Context.h"
#include "Dungeon.h"
#include "GameStateObservation.h"
#include "RandomProvider.h"

namespace HuntTheWumpus
{
    Wumpus::Wumpus(const int instance, Context& providers)
        : Denizen({ Category::Wumpus, instance }, { false, false, true, false, false }, providers)
    {
    }

    bool Wumpus::ObserveCaveEntrance(const std::shared_ptr<Denizen>& trigger)
    {
        // Is this the player coming into the Wumpus' room?
        if (trigger->Properties().m_isEdible)
        {
            float moveProb = m_providers.m_random.MakeRandomNumber();

            if (moveProb >= 0.25f)
            {
                Cave *pCave = m_cave.lock().get();
                pCave->GetDungeon().MoveDenizenRandomly(m_identifier);
                return true;
            }

            // Otherwise it stays put.
            m_providers.m_change.GameOver(false);

            return true;
        }

        // Is this an arrow that shot us?
        if (trigger->Properties().m_fatalToWumpus)
        {
            m_providers.m_change.GameOver(true);

            return true;
        }

        return false;
    }
}