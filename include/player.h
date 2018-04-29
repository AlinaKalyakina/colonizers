#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include "gamefield.h"
#include <set>

struct Player
{
    std::string name;
    std::multiset<Resource> recource_cards;
    int score = 0;

    Player(std::string &name);
};

#endif // PLAYER_H
