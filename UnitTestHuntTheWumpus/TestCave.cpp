#include "Cave.h"
#include "Dungeon.h"
#include "Context.h"

#include <TestHarness.h>

#include "TestHelperTestEnvironment.h"

namespace TestHuntTheWumpus
{
    TEST(CaveSuite, CaveConstruction)
    {
        TestEnvironment env;

        const HuntTheWumpus::Cave cave(57, env.m_dungeon);

        CHECK_EQUAL(57, cave.GetCaveId());
        CHECK_EQUAL(&env.m_dungeon, &cave.GetDungeon());

        CHECK(cave.GetConnectedIds().empty());
    }

    class TestDenizen final : public HuntTheWumpus::Denizen
    {
    public:
        explicit TestDenizen(HuntTheWumpus::Context& context)
            : Denizen({ HuntTheWumpus::Category::Hunter, 1 }, { false, false, false, false, true }, context)
        {
        }

        ~TestDenizen() override = default;

        int GetPriority() const override
        {
            return m_priority;
        }

        int m_priority = 0;

        TestDenizen(const TestDenizen&) = delete;
        TestDenizen(TestDenizen&&) = delete;
        TestDenizen& operator=(const TestDenizen&) = delete;
        TestDenizen& operator=(TestDenizen&&) = delete;
    };

    TEST(CaveSuite, CaveDenizen_AddDenizen_AddsProperly)
    {
        TestEnvironment env;

        HuntTheWumpus::Cave cave(57, env.m_dungeon);

        const auto testDenizen = std::make_shared<TestDenizen>(env.m_context);

        cave.AddDenizen(testDenizen);

        CHECK(cave.HasDenizens());
        CHECK(cave.HasDenizen( { HuntTheWumpus::Category::Hunter, 1 } ) );
    }

    TEST(CaveSuite, CaveTunnels_AddTunnel_ReportsConnections)
    {
        TestEnvironment env;

        HuntTheWumpus::Cave cave(57, env.m_dungeon);

        const auto cave1 = std::make_shared<HuntTheWumpus::Cave>(58, env.m_dungeon);
        const auto cave2 = std::make_shared<HuntTheWumpus::Cave>(59, env.m_dungeon);
        const auto cave3 = std::make_shared<HuntTheWumpus::Cave>(60, env.m_dungeon);

        cave.ConnectTo(cave1);
        cave.ConnectTo(cave2);
        cave.ConnectTo(cave3);

        // Get the set of connected cave IDs.
        auto caveIds = cave.GetConnectedIds();

        CHECK_EQUAL(static_cast<size_t>(3), caveIds.size());

        // Note: since the map is unordered, the cave IDs come out in
        // any order. Test that the ones we want are present.
        CHECK(std::find(caveIds.begin(), caveIds.end(), 58) != caveIds.end());
        CHECK(std::find(caveIds.begin(), caveIds.end(), 59) != caveIds.end());
        CHECK(std::find(caveIds.begin(), caveIds.end(), 60) != caveIds.end());

        // Get a particular cave.
        auto caveTrial = cave.GetConnectedCave(59).lock();

        CHECK_EQUAL(59, caveTrial->GetCaveId());

        // Show these are actually the same cave.
        CHECK_EQUAL(cave2.get(), caveTrial.get());
    }
}
