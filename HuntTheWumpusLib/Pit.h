#pragma once

#include "Denizen.h"

namespace HuntTheWumpus
{
    class Pit final : public Denizen
    {
    public:
        Pit(int pitInstance, Context& providers);
        ~Pit() override = default;

        int GetPriority() const override { return 5; }

        bool ObserveCaveEntrance(const std::shared_ptr<Denizen>& trigger) override;

        Pit(const Pit&) = delete;
        Pit(Pit&&) = delete;
        Pit& operator=(const Pit&) = delete;
        Pit& operator=(Pit&&) = delete;
    };
}