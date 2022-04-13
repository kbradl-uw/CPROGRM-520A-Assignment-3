#include <TestHarness.h>

#include "Bat.h"

#include "Cave.h"

#include "TestHelperTestEnvironment.h"

namespace TestHuntTheWumpus
{
    TEST(BatSuite, Bat_HasProperAttributes)
    {
        TestEnvironment env;

        const HuntTheWumpus::Bat bat(0, env.m_context);

        const auto &properties = bat.Properties();

        CHECK(!properties.m_carryableByBats);
        CHECK(!properties.m_isEdible);
        CHECK(!properties.m_reportMovement);
        CHECK(!properties.m_fatalToHunter);
        CHECK(!properties.m_fatalToWumpus);

        CHECK_EQUAL(10, bat.GetPriority());

        const auto& id = bat.GetIdentifier();

        CHECK_EQUAL(HuntTheWumpus::Category::Bat, id.m_category);
    }
}
