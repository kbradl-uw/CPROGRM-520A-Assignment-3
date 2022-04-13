#pragma once

namespace HuntTheWumpus
{
    class IRandomProvider;
    class IGameStateChange;

    struct Context
    {
        IRandomProvider &m_random;
        IGameStateChange &m_change;
    };
}