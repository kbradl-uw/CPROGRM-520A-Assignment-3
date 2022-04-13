#include "Cave.h"
#include "Denizen.h"

namespace HuntTheWumpus
{
    bool CompareThings::operator()(const std::shared_ptr<Denizen>& thing1, const std::shared_ptr<Denizen>& thing2) const
    {
        return thing1->GetPriority() > thing2->GetPriority();
    }

    Cave::Cave(const int caveId, IDungeon& dungeon)
        : m_caveId(caveId),
        m_dungeon(dungeon)
    {
    }

    void Cave::ConnectTo(const std::shared_ptr<Cave>& destination)
    {
        std::weak_ptr<Cave> localCave = destination;

        std::pair<int, std::weak_ptr<Cave>> toInsert;
        toInsert.first = destination->GetCaveId();
        toInsert.second = localCave;

        m_tunnels.insert(toInsert);
    }

    const std::weak_ptr<Cave>& Cave::GetConnectedCave(const int caveId)
    {
        return m_tunnels.at(caveId);
    }

    void Cave::AddDenizen(const std::shared_ptr<Denizen>& newDenizen, const bool observeEntrance)
    {
        m_denizens.insert(newDenizen);

        if (observeEntrance)
        {
            std::set<std::shared_ptr<Denizen>>::const_iterator itr;

            for (itr = m_denizens.begin(); itr != m_denizens.end(); ++itr)
            {
                std::shared_ptr<Denizen> denizen = *itr;

                const auto action = denizen->ObserveCaveEntrance(newDenizen);

                if (action)
                {
                    // Stop if that denizen affected things.
                    break;
                }
            }
        }
    }

    void Cave::RemoveDenizen(const DenizenIdentifier& identifier)
    {
        std::set<std::shared_ptr<Denizen>>::const_iterator itr;

        for (itr = m_denizens.begin(); itr != m_denizens.end(); ++itr)
        {
            std::shared_ptr<Denizen> denizen = *itr;

            const DenizenIdentifier& id = denizen->GetIdentifier();

            if (id.m_category == identifier.m_category &&
                id.m_instance == identifier.m_instance)
            {
                break;
            }
        }

        if (itr != m_denizens.end())
        {
            m_denizens.erase(itr);
        }
    }

    std::vector<int> Cave::GetConnectedIds() const
    {
        std::vector<int> caveIds;

        std::unordered_map<int, std::weak_ptr<Cave>>::const_iterator itr;

        for(itr = m_tunnels.begin(); itr != m_tunnels.end(); ++itr)
        {
            int caveId = (*itr).first;

            caveIds.push_back(caveId);
        }

        return caveIds;
    }

    bool Cave::HasDenizen(const DenizenIdentifier& identifier) const
    {
        std::set<std::shared_ptr<Denizen>>::const_iterator itr;

        for(itr = m_denizens.begin(); itr != m_denizens.end(); ++itr)
        {
            std::shared_ptr<Denizen> denizen = *itr;
            const DenizenIdentifier &id = denizen->GetIdentifier();

            if(id.m_category == identifier.m_category)
            {
                break;
            }
        }

        return itr != m_denizens.end();
    }
}
