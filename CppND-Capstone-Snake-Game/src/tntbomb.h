#ifndef TNTBOMB_H
#define TNTBOMB_H

#include <vector>
#include "SDL.h"

class TntBomb {
    private:
        SDL_Point location;
    public:
        TntBomb(int x, int y) : location(SDL_Point {x, y}) {}
        bool operator<(const TntBomb& rhs) const
        {
            return location.x < rhs.GetLocation().x || location.y < rhs.GetLocation().y;
        }

        SDL_Point GetLocation() const { return location; };    
};

#endif