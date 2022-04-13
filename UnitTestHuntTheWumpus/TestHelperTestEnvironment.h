#pragma once

#include "TestHelperRandomProvider.h"
#include "TestHelperDungeon.h"

#include "Context.h"

namespace TestHuntTheWumpus
{
    struct TestEnvironment
    {
        TestEnvironment();

        TestRandomProvider m_provider;

        HuntTheWumpus::Context m_context { m_provider };

        TestDungeon m_dungeon;
    };
}