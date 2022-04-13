#include "Arrow.h"

namespace HuntTheWumpus
{
    Arrow::Arrow(const int arrowInstance, Context &providers)
        : Denizen(DenizenIdentifier{ Category::Arrow, arrowInstance }, { false, true, true, false, false }, providers)
    {       
    }
}