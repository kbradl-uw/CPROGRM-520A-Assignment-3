#include <TestHarness.h>

#include "Wumpus.h"

#include "Arrow.h"
#include "Bat.h"
#include "Cave.h"
#include "Hunter.h"

#include "TestHelperTestEnvironment.h"

namespace TestHuntTheWumpus
{
    TEST(WumpusSuite, Wumpus_HasProperAttributes)
    {
        TestEnvironment env;

        const HuntTheWumpus::Wumpus wumpus(0, env.m_context);

        const auto &properties = wumpus.Properties();

        CHECK(!properties.m_carryableByBats);
        CHECK(!properties.m_isEdible);
        CHECK(!properties.m_reportMovement);
        CHECK(properties.m_fatalToHunter);
        CHECK(!properties.m_fatalToWumpus);

        CHECK_EQUAL(4, wumpus.GetPriority());

        const auto& id = wumpus.GetIdentifier();

        CHECK_EQUAL(HuntTheWumpus::Category::Wumpus, id.m_category);
    }
}
