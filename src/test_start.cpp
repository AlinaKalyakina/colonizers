#include <iostream>
#include "engine.h"
using namespace std;

bool check(field_ptr field) {
    if (field->hexes.size() != 19) {
        return false;
    }
    Pack<Resource> pack = {0, 0, 0, 0, 0};
    int numbers[13] ={0};
    for (auto x : field->hexes) {
        if (x.second.type == Resource::NO_RESOURCE) {
            if (field->robber_pos != x.second.pos || x.first != 7) {
                return false;
            }
        } else {
            pack[x.second.type]++;
        }
        numbers[x.first]++;
    }
    //NUMBERS
    if (numbers[2] != 1 || numbers[12] != 1) {
        return false;
    }
    for (int i = 3; i <=11; i++) {
        if (i == 7 && numbers[i] != 1) {
            return false;
        }
        if (numbers[i] != 2 && i != 7) {
            return false;
        }
    }
    //Resourses
    if (!(pack >= Pack<Resource>(3, 3, 3, 3, 3))) {
        return false;
    }
    return true;
}

int main()
{
    for (int i = 0; i < 100000; i++) {
        Engine engine;
        auto field = engine.get_field();
        engine.set_state(GameState::PLAYERS_REGISTRATION);
        engine.insert_player();
        engine.start_game();
        if (!check(field)) {
            std::cout << "!!!";
        }
    }
}
