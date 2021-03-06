#pragma once

#include "Denizen.h"

namespace HuntTheWumpus
{
    class Bat final : public Denizen
    {
    public:
        Bat(int batInstance, Context &providers);
        ~Bat() override = default;

        bool ObserveCaveEntrance(const std::shared_ptr<Denizen>& trigger) override;

        int GetPriority() const override { return 10; }

        Bat(const Bat &) = delete;
        Bat(Bat &&) = delete;
        Bat &operator=(const Bat &) = delete;
        Bat &operator=(Bat &&) = delete;
    };
}