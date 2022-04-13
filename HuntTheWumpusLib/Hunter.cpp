#include "Hunter.h"

#include "Arrow.h"

#include "Cave.h"
#include "Context.h"
#include "GameStateObservation.h"

namespace HuntTheWumpus
{
    Hunter::Hunter(Context& providers)
        : Denizen({ Category::Hunter, 0 }, { true, false, false, true, true }, providers)
    {
        int arrowIdx = 0;

        for (arrowIdx = 0; arrowIdx < 5; ++arrowIdx)
        {
            std::shared_ptr<Arrow> arrow;
            arrow = std::shared_ptr<Arrow>(new Arrow(arrowIdx, providers));

            m_arrows.push_back(arrow);
        }
    }

    std::shared_ptr<Arrow> Hunter::GetArrow()
    {
        std::shared_ptr<Arrow> result = m_arrows.back();
        m_arrows.pop_back();

        return result;
    }

    bool Hunter::ObserveCaveEntrance(const std::shared_ptr<Denizen>& trigger)
    {
        if (trigger->Properties().m_fatalToHunter)
        {
            m_providers.m_change.GameOver(false);
            return true;
        }

        return false;
    }

}