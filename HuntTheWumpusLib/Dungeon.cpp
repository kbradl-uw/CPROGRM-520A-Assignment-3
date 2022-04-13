#include "Dungeon.h"

#include "Cave.h"
#include "Bat.h"
#include "Wumpus.h"
#include "Hunter.h"
#include "Pit.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "Arrow.h"

#include "Context.h"
#include "RandomProvider.h"
#include "GameStateObservation.h"

namespace HuntTheWumpus
{
    Dungeon::Dungeon(Context& providers)
        : m_providers(providers)
    {
        Initialize(providers);
    }

    void Dungeon::Initialize(Context& providers)
    {
        for (auto idx = 1; idx <= 20; ++idx)
        {
            m_caves.emplace(idx, std::make_shared<Cave>(idx, *this));
        }

        MakeTunnels();

        AddDenizen( std::make_shared<Bat>(0, m_providers) );
        AddDenizen( std::make_shared<Bat>(1, m_providers) );
        AddDenizen( std::make_shared<Wumpus>(0, m_providers) );
        AddDenizen( std::make_shared<Pit>(0, m_providers) );
        AddDenizen( std::make_shared<Pit>(1, m_providers) );

        // Put the hunter in a random  empty cave.
        auto hunterPlaced = false;

        while(!hunterPlaced)
        {
            const auto hunterCave = providers.m_random.MakeRandomCave();

            if(m_caves[hunterCave]->HasDenizens())
            {
                continue;
            }

            AddToCave(std::make_shared<Hunter>(providers), hunterCave, true);

            hunterPlaced = true;
        }
    }

    void Dungeon::AddDenizen(const std::shared_ptr<Denizen>& newDenizen)
    {
        auto denizenPlaced = false;

        while (!denizenPlaced)
        {
            const auto denizenCave = m_providers.m_random.MakeRandomCave();

            // Verify that cave does not have a like Denizen in it.
            if (m_caves[denizenCave]->HasDenizen(newDenizen->GetIdentifier()))
            {
                continue;
            }

            AddToCave(newDenizen, denizenCave, false);

            denizenPlaced = true;
        }
    }

    void Dungeon::MakeTunnels()
    {
        const std::vector<std::pair<int, std::vector<int> > > tunnelPairs = {
            { 1, {2, 5, 8} },
            { 2, {1, 3, 10} },
            { 3, {2, 4, 12} },
            { 4, {3, 5, 14} },
            { 5, {1, 4, 6} },
            { 6, { 5, 7, 15} },
            { 7, {6, 8, 17} },
            { 8, {1, 7, 9} },
            { 9, {8, 10, 18} },
            { 10, {2, 9, 11} },
            { 11, {10, 12, 19} },
            { 12, {3, 11, 13} },
            { 13, {12, 14, 20 }  },
            { 14, {4, 13, 15} },
            { 15, {6, 14, 16} },
            { 16, {15, 17, 20} },
            { 17, {7, 16, 18} },
            { 18, {9, 17, 19} },
            { 19, {11, 18, 20} },
            { 20, {13, 16, 19} }
        };

        for (auto&& [caveId, neighborIds] : tunnelPairs)
        {
            const auto cave = m_caves.at(caveId);

            for (auto&& neighborId : neighborIds)
            {
                const auto neighbor = m_caves.at(neighborId);
                cave->ConnectTo(neighbor);
            }
        }
    }

    const std::shared_ptr<Cave>& Dungeon::FindCave(const int caveId)
    {
        const auto caveItr = m_caves.find(caveId);
        return caveItr->second;
    }

    void Dungeon::AddToCave(const std::shared_ptr<Denizen>& denizen, const int caveId, const bool observeEntrance)
    {
        m_caveDenizens.emplace(denizen->GetIdentifier(), denizen);

        const auto cave = m_caves.at(caveId);

        denizen->EnterCave(cave);
        cave->AddDenizen(denizen, observeEntrance);
    }

    void Dungeon::Move(const DenizenIdentifier& identifier, const int destinationCave)
    {
        std::unordered_map<DenizenIdentifier, std::shared_ptr<Denizen>, DenizenIdentifierHasher>::mapped_type &thing = m_caveDenizens[identifier];

        Cave *pStartCave = thing->GetCurrentCave().lock().get();

        thing->RemoveFromCave();
        pStartCave->RemoveDenizen(identifier);

        std::unordered_map<int, std::shared_ptr<Cave>>::mapped_type &destCave = m_caves[destinationCave];

        thing->EnterCave(destCave);
        destCave->AddDenizen(thing, true);
    }

    bool Dungeon::LegalMove(const std::shared_ptr<Denizen>& denizen, const int destinationCave) const
    {
        Cave *pDenizenCave = denizen->GetCurrentCave().lock().get();
        std::vector<int> tunnelIds = pDenizenCave->GetConnectedIds();

        int idx = 0;

        for(idx = 0; idx < tunnelIds.size(); ++idx)
        {
            if(tunnelIds[idx] == destinationCave)
            {
                break;
            }
        }

        return idx < tunnelIds.size();
    }

    void Dungeon::MakeMove(const DungeonMove operation, const std::vector<int>& destinationIds)
    {
        // First, find the hunter.
        DenizenIdentifier hunterId;
        hunterId.m_category = Category::Hunter;
        hunterId.m_instance = 0;

        std::shared_ptr<Denizen> hunterDenizen = m_caveDenizens.at(hunterId);

        Hunter *pHunter = dynamic_cast<Hunter *>(hunterDenizen.get());

        if (operation == DungeonMove::Move)
        {
            // Check move validity.
            if (LegalMove(hunterDenizen, destinationIds.front()))
            {
                Move(hunterId, destinationIds.front());
            }
        }

        if (operation == DungeonMove::Shoot)
        {
            // Retrieve an arrow.
            std::shared_ptr<Arrow> arrow = pHunter->GetArrow();

            Cave *pStartCave = pHunter->GetCurrentCave().lock().get();

            AddToCave(arrow, pStartCave->GetCaveId(), false);

            int currentStep = 0;

            for(currentStep = 0; currentStep < destinationIds.size(); ++currentStep)
            {
                int caveId = destinationIds[currentStep];

                if (!LegalMove(arrow, caveId))
                {
                    MoveDenizenRandomly(arrow);
                }
                else
                {
                    Move(arrow->GetIdentifier(), caveId);
                }

                if (!m_providers.m_change.IsPlaying())
                {
                    break;
                }
            }

            // Once the arrow is done, remove it.
            Cave *pCurrentArrowCave = arrow->GetCurrentCave().lock().get();

            arrow->RemoveFromCave();
            pCurrentArrowCave->RemoveDenizen(arrow->GetIdentifier());
            m_caveDenizens.erase(arrow->GetIdentifier());

            if (m_providers.m_change.IsPlaying())
            {
                // The game didn't end, meaning the Wumpus wasn't shot,
                // so we need to wake up the Wumpus and move it.
                DenizenIdentifier wumpusId;
                wumpusId.m_category = Category::Wumpus;
                wumpusId.m_instance = 0;

                std::shared_ptr<Denizen> wumpus = m_caveDenizens.at(wumpusId);

                MoveDenizenRandomly(wumpus);
            }

            // The wumpus move could have ended the game, so check again.
            if (m_providers.m_change.IsPlaying() && pHunter->OutOfArrows())
            {
                m_providers.m_change.GameOver(false);
            }
        }
    }

    void Dungeon::MoveDenizenRandomly(const DenizenIdentifier& identifier)
    {
        std::shared_ptr<Denizen> denizen = m_caveDenizens.at(identifier);
        MoveDenizenRandomly(denizen);
    }

    void Dungeon::MoveDenizenRandomly(const std::shared_ptr<Denizen>& denizen)
    {
        Cave *pDenizenCave = denizen->GetCurrentCave().lock().get();
        std::vector<int> caveTunnels;

        caveTunnels = pDenizenCave->GetConnectedIds();

        int newRandomId = m_providers.m_random.MakeRandomTunnel();

        Move(denizen->GetIdentifier(), caveTunnels[newRandomId]);
    }
}