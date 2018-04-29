#include "engine.h"
#include <cstdlib>
#include <ctime>
#include <array>
#include <utility>

Engine::Engine() {
    field = std::make_shared<GameField>();
}

void Engine::game() {
    while (true) {
        switch (state) {
        case (GameState::INTRODUCTION):
            interface.show_introduction();
            state = GameState::PLAYERS_REGISTRATION;
            break;
        case (GameState::PLAYERS_REGISTRATION): {
            auto cmd = interface.get_user_cmd();
            switch(cmd->type) {
            case(Cmd_from_user::REGISTER_PLAYER):
                try {
                    join_player(cmd->name);
                } catch (...) {
                    interface.show_error();
                }
                break;
            case (Cmd_from_user::START):
                if (players.size() == 0) {
                    interface.show_error();
                    continue;
                }
                start_game();
                interface.show_field(field);
                state = GameState::PUT_INITIAL_INFRASTRUCTURES;
                break;
            default:
                interface.show_error();
            }
            break;
            }
        case (GameState::PUT_INITIAL_INFRASTRUCTURES):
            for (auto x : players) {
                interface.show_player_info(x);
                initial_registration(x);
                interface.show_field(field);
            }
            for (auto x = players.rbegin(); x != players.rend(); x++) {
                interface.show_player_info(*x);
                auto cross = initial_registration(*x);
                interface.show_field(field);
                get_init_resource(cross, *x);
            }
            state = GameState::STAGE1_DICE;
            break;
        case (GameState::STAGE1_DICE) :
            interface.show_player_info(players[player_num]);
            make_dice();
            interface.show_dice_score(field->dice_score);
            break;
        case (GameState::STAGE1_DROP_RECOURCES):
            interface.show_field(field);
            for (auto x : players) {
                if (drop_resource(x)) {
                    interface
                }
            }
        }
    }
}

cross_pos Engine::initial_registration(const Player &player) {
    cross_pos cross;
    bool road_registered = false;
    bool town_registered = false;
    while (!town_registered || !road_registered) {
        auto cmd = interface.get_user_cmd();
        if (cmd->type != Cmd_from_user::REGISTER_OBJ) {
            interface.show_error();
        }
        switch (cmd->obj_type) {
        case (ObjectType::ROAD) :
            if (road_registered) {
                interface.show_error();
            }
            try {
                register_road(player.id, cmd->road);
                road_registered = true;
            } catch (...) {
                interface.show_error();
            }
            break;
        case (ObjectType::TOWN) :
            if (town_registered) {
                interface.show_error();
            }
            try {
                register_settlement(player.id, cmd->cross, cmd->obj_type);
                town_registered = true;
                cross = cmd->cross;
            } catch (...) {
                interface.show_error();
            }
            break;
        default :
            interface.show_error();
        }
    }
    return cross;
}

void get_init_resource(cross_pos cross, Player &player) {
    auto hexes = neighbour_hexes(cross);
    for (auto x : hexes) {
        if (x.type != Resource::NO_RESOURCE) {
            player.resource_cards[x.type]++;
        }
    }
}
void Engine::join_player(const string &name) {
    if (players.size() < (unsigned long) Limits::MAX_NUM_OF_PLAYERS) {
        for (auto x : players) {
            if (x.name == name) {
                throw;
            }
        }
        players.push_back(Player(name));
    } else {
        throw;//ERROR
    }
}

coord_t gen_coord() {
    int x = rand() % 5 - 2;
    int y = rand() % (5 - abs(x)) - 2 * (x >= 0) - (2 + x) * (x < 0);
    return coord_t(x, y);
}

void Engine::start_game() {
    field = std::make_shared<GameField>();
    std::srand(std::time(nullptr));
    int numbers[11] = {1, 2, 2, 2, 2, 0, 2, 2, 2, 2, 1};
    int res[5] = {3, 3, 3, 3, 3};
    auto coord = gen_coord();//Desert
    field->hexes.insert(std::pair<int, Hex>(7,Hex(coord, Resource::NO_RESOURCE)));
    field->robber_pos = coord;
    int num_it = 0;
    for (int i = -2; i <= 2; i++) {
        int hex_num = 7;
        int res_num;
        for (int j = - 2 * (i >= 0) - (2 + i) * (i < 0);
             j <= (2 - i) * (i >= 0) + 2 * (i < 0); j++) {
            if (coord == coord_t(i, j)) { //NOT DESERT
                continue;
            }
            num_it++;
            while (numbers[hex_num = rand() % 12] == 0) {};
            if (num_it <= 15) {
                while (res[res_num = rand() % int(Resource::NUMBER)] == 0) {}
            } else {
                res_num = rand() % 12;
            }
            numbers[hex_num]--;
            res[res_num]--;
            field->hexes.insert(std::make_pair(hex_num, Hex(coord_t(i, j), Resource(res_num))));
            }
    }
}

void Engine::register_road(int num, road_pos road) {
    if (field->roads.count(road)) {
        throw;
    }
    field->roads.insert(std::make_pair(road, Object(num)));
    players[num].object_cards[ObjectType::ROAD]--;
}

void Engine::register_settlement(int num, const cross_pos &pos, ObjectType type) {
    if (field->settlements.count(pos)) {
        throw ;
    }
    field->settlements.insert(std::pair<cross_pos, Object>(pos, Object(num, ObjectType::TOWN)));
    players[num].object_cards[type]--;
}


void Engine::get_resources() {
    auto range = field->hexes.equal_range(field->dice_score);
    while (range.first != range.second) {
        auto hex = range.first->second;
        auto crosses_coord = neighbour_crosses(hex.pos);
        for (auto pos : crosses_coord) {
            if (field->settlements.count(pos)) {
                auto settle_it = field->settlements.find(pos);
                if (settle_it->second.type == ObjectType::TOWN) {
                    players[settle_it->second.owner].resource_cards[hex.type] += 1;
                } else { //CITY
                    players[settle_it->second.owner].resource_cards[hex.type] += 2;
                }

            }
        }
    }
}

void Engine::make_dice() {
    field->dice_score = rand() % 6 + 2 + rand() % 6;
    if (field->dice_score == int(Limits::MAGIC_NUM)) {
        state = GameState::STAGE1_DROP_RECOURCES;
    } else {
        get_resources();
        state = GameState::STAGE2;
    }
}

bool Engine::drop_resource(Player &player) {
    auto res_num = player.resource_cards.sum();
    if (res_num > int(Limits::MAGIC_NUM)) {
        res_num /= 2;
        Resource res;
        for (int i = 0; i < res_num; i++) {
            while (player.resource_cards[res = Resource(rand() % 5)] == 0);
            player.resource_cards[res]--;
        }
        return true;
    }
    return false;
}

void Engine::move_robber(coord_t pos) {
    field->robber_pos = pos;
}

void Engine::rob(int robbed_num) {
    if (players[robbed_num].resource_cards.sum() != 0) {
        while (true) {
            int res = rand() % 5;
            if (players[robbed_num].resource_cards[Resource(res)] != 0) {
                players[robbed_num].resource_cards[Resource(res)]--;
                field->bank[Resource(res)]++;
            }
        }
    }
}

void Engine::exchange_with_field(Resource player_resourse, Resource bank_resource) {
    if (players[player_num].resource_cards[player_resourse] < 4) {
        throw; //ERROR
    }
    if (field->bank[bank_resource] < 1) {
        throw; //ERROR
    }
    field->bank[player_resourse] += 4;
    field->bank[bank_resource]--;
    players[player_num].resource_cards[player_resourse] -= 4;
    players[player_num].resource_cards[bank_resource]++;
}



void Engine::end_exchanges() {
    requests.clear();
    state = GameState::STAGE3;
}


template<class T>
bool check_owner(const std::vector<T> positions, const std::map<T, Object> list, int player) {
    for (auto x : positions) {
        auto it = list.find(x);
        if (it != list.end() && it->second.owner == player) {
            return true;
        }
    }
    return false;
}

bool Engine::check_way(cross_pos pos, road_pos prev_road) {
    if (!belong_field(pos)) {
        return false;
    }
    auto roads = neighbour_roads(pos);
    auto crosses = neighbour_crosses(pos);
    if (field->settlements.find(pos)->second.owner == player_num) {
        return true;
    }
    for (auto x : roads) {
        if (prev_road != x) {
            if (check_way(next_cross(pos, x), x)) {
                return true;
            }
        }
    }
    return false;
}

void Engine::build_road(road_pos pos) {
    if (players[player_num].resource_cards < road_request) {
        throw;//ERROR
    }
    if (field->roads.count(pos)) {
        throw;//ERROR
    }
    if (players[player_num].object_cards[ObjectType::ROAD] == 0) {
        throw; //ERROR
    }
    auto crosses = neighbour_crosses(pos);
    if (check_owner(neighbour_roads(pos), field->roads, player_num) ||
            check_owner(crosses, field->settlements, player_num)) {
        register_road(player_num, pos);
        players[player_num].resource_cards -= road_request;
        players[player_num].object_cards[ObjectType::ROAD]--;
        field->bank += road_request;
        return;
    }
    throw;//ERROR
}

void Engine::build_town(cross_pos pos) {
    if (players[player_num].resource_cards < town_request) {
        throw;//ERROR
    }
    if (field->settlements.count(pos)) {
        throw;//ERROR
    }
    if (players[player_num].object_cards[ObjectType::TOWN] == 0) {
        throw; //ERROR
    }
    auto crosses = neighbour_crosses(pos);
    for (auto x : crosses) {
        if (field->settlements.count(x)) {
            throw; //ERROR
        }
    }
    //CHECK_WAYS
    register_settlement(player_num, pos, ObjectType::TOWN);
    players[player_num].resource_cards -= town_request;
    field->bank += town_request;
    players[player_num].object_cards[ObjectType::TOWN]--;
}

void Engine::build_city(cross_pos pos) {
    if (players[player_num].resource_cards < city_request) {
        throw ;//ERROR
    }
    auto it = field->settlements.find(pos);
    if (it == field->settlements.end() || it->second.owner != player_num) {
        throw ;
    }
    it->second.type = ObjectType::CITY;
    players[player_num].resource_cards -= city_request;
    field->bank += city_request;
    players[player_num].object_cards[ObjectType::TOWN]++;
    players[player_num].object_cards[ObjectType::CITY]--;
}

field_ptr Engine::get_field() {
    return field;
}
