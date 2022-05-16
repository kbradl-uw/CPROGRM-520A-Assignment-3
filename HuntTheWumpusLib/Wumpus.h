#pragma once

#include "Denizen.h"

namespace HuntTheWumpus
{
    class Wumpus final : public Denizen
    {
    public:
        explicit Wumpus(int instance, Context& providers);
        ~Wumpus() override = default;

        int GetPriority() const override { return 4; }

        bool ObserveCaveEntrance(const std::shared_ptr<Denizen>& trigger) override;

        Wumpus(const Wumpus&) = delete;
        Wumpus(Wumpus&&) = delete;
        Wumpus& operator=(const Wumpus&) = delete;
        Wumpus& operator=(Wumpus&&) = delete;
    };
}