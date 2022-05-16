#include <TestHarness.h>

#include "Dungeon.h"

#include "Cave.h"

#include "Bat.h"
#include "Pit.h"
#include "Wumpus.h"

#include "TestHelperTestEnvironment.h"

namespace TestHuntTheWumpus
{
    TEST(DungeonSuite, TestDungeonInitialize)
    {
        TestEnvironment env;

        // This will ask for 6 random numbers.
        env.m_provider.SetCaveSequence({ 1, 2, 3, 4, 5, 6 });

        HuntTheWumpus::Dungeon dungeon(env.m_context);

        struct TestExpectation
        {
            int m_caveId;
            HuntTheWumpus::DenizenIdentifier m_expectedIdentifier;
        };

        TestExpectation tests[] =
        {
            { 1, { HuntTheWumpus::Category::Bat, 0 } },  
            { 2, { HuntTheWumpus::Category::Bat, 1 } }, 
            { 3, { HuntTheWumpus::Category::Wumpus, 0 } },  
            { 4, { HuntTheWumpus::Category::Pit, 0 } }, 
            { 5, { HuntTheWumpus::Category::Pit, 1 } }, 
            { 6, { HuntTheWumpus::Category::Hunter, 0 } }, 
        };

        for (auto&& test : tests)
        {
            const auto& cave = dungeon.FindCave(test.m_caveId);

            CHECK(cave->HasDenizens());
            CHECK(cave->HasDenizen(test.m_expectedIdentifier));
        }
    }

    TEST(DungeonSuite, TestDenizenPriorities)
    {
        TestEnvironment env;

        HuntTheWumpus::Bat  bat(0, env.m_context);
        HuntTheWumpus::Pit  pit(0, env.m_context);
        HuntTheWumpus::Wumpus wumpus(0, env.m_context);

        CHECK( bat.GetPriority() > pit.GetPriority() );
        CHECK( pit.GetPriority() > wumpus.GetPriority() );
    }}
