#include "Denizen.h"

#include <bit>
#include <functional>
#include <iostream>
#include <limits>

namespace HuntTheWumpus
{
    Denizen::Denizen(const DenizenIdentifier& identifier, DenizenProperties&& properties, Context& providers)
      : m_identifier(identifier),
        m_properties(properties),
        m_providers(providers)
    {
    }

    int Denizen::GetPriority() const
    {
        return 0;
    }

    bool Denizen::ObserveCaveEntrance(const std::shared_ptr<Denizen>&)
    {
        // Default behavior: nothing.
        return false;
    }

    void Denizen::EnterCave(const std::shared_ptr<Cave>& cave)
    {
        m_cave = cave;
    }

    void Denizen::RemoveFromCave()
    {
        m_cave.reset();
    }


    bool DenizenIdentifier::operator==(const DenizenIdentifier& other) const
    {
        return ((*this) <=> other) == std::strong_ordering::equal;
    }

    std::strong_ordering DenizenIdentifier::operator<=>(const DenizenIdentifier& other) const
    {
        if (m_category == other.m_category)
        {
            return m_instance <=> other.m_instance;
        }

        return static_cast<int>(m_category) <=> static_cast<int>(other.m_category);
    }

    // See https://stackoverflow.com/questions/35985960/c-why-is-boosthash-combine-the-best-way-to-combine-hash-values/50978188
    size_t xorshift(const size_t n, const int i)
    {
        return n ^ (n >> i);
    }

    uint32_t distribute(const uint32_t n)
    {
        constexpr uint32_t p = 0x55555555ul; // pattern of alternating 0 and 1
        constexpr uint32_t c = 3423571495ul; // random uneven integer constant;
        return static_cast<uint32_t>(c * xorshift(p * xorshift(n, 16), 16));
    }

    size_t hash_combine(const std::size_t seed, const size_t v)
    {
        return std::rotl(seed, std::numeric_limits<size_t>::digits / 3) ^ distribute(static_cast<uint32_t>(v));
    }

    size_t DenizenIdentifierHasher::operator()(const DenizenIdentifier& id) const
    {
        auto result = 0ull;

        result = hash_combine(result, std::hash<int>()(static_cast<int>(id.m_category)));

        return hash_combine(result, std::hash<int>()(id.m_instance));
    }

    std::ostream& operator<<(std::ostream& out, const Category& value)
    {
        static std::unordered_map<Category, std::string> s_valueMap =
        {
            {Category::Arrow, "Arrow" },
            {Category::Bat, "Bat" },
            {Category::Hunter, "Hunter" },
            {Category::Pit, "Pit" },
            {Category::Wumpus, "Wumpus" }
        };

        out << s_valueMap[value];
        return out;
    }
}
