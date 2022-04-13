#include "Bat.h"

#include "Cave.h"
#include "Context.h"
#include "Dungeon.h"
#include "RandomProvider.h"

namespace HuntTheWumpus
{
    Bat::Bat(const int batInstance, Context& providers)
        : Denizen(DenizenIdentifier{ Category::Bat, batInstance }, { false, false, false, false, false }, providers)
    {
    }

    bool Bat::ObserveCaveEntrance(const std::shared_ptr<Denizen>& trigger)
    {
        if (trigger->Properties().m_carryableByBats)
        {
            Cave *pCave = m_cave.lock().get();

            // Carry to another spot.
            int caveId = pCave->GetCaveId();

            bool newCaveFound = false;
            int newCaveId = 0;

            while (!newCaveFound)
            {
                newCaveId = m_providers.m_random.MakeRandomCave();
                newCaveFound = (newCaveId != caveId);
            }

            pCave->GetDungeon().Move(trigger->GetIdentifier(), newCaveId);
            return true;
        }

        return false;
    }
}