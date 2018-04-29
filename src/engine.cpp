#include "engine.h"
#include <cstdlib>
#include <ctime>
#include <array>
#include <utility>

Engine::Engine() {
    field = std::make_shared<GameField>();
}

void Engine::game(const Command &cmd) {
    switch (state) {
    case (GameState::PLAYERS_REGISTRATION):
        switch(cmd.type) {
        case(Cmd_from_user::REGISTER_PLAYER):
            join_player(cmd.name);
            break;
        case (Cmd_from_user::START):
            if (players.size() == 0) {
                throw;
            }
            start_game();
            state = GameState::PUT_INITIAL_INFRASTRUCTURES_DIRECT;
            player_num = 0;
            break;
        default:
            throw;
        }
        break;
    case (GameState::PUT_INITIAL_INFRASTRUCTURES_DIRECT):
        if (cmd.type != Cmd_from_user::REGISTER_OBJ) {
            throw;
        }
        if (field->roads.count(cmd.road)) {
            throw;
        }
        field->roads.insert(std::make_pair(road_pos(cmd.road), Object(player_num)));
        players[player_num].object_cards[ObjectType::ROAD]--;
        try {
            register_town(cmd.cross);
        }
        catch (...) {
            undo_obj_registration(cmd.road);
            throw ;
        }
        player_num++;
        if (player_num == players.size()) {
            state = GameState::PUT_INITIAL_INFRASTRUCTURE_REVERCE;
        }
        break;
    case (GameState::PUT_INITIAL_INFRASTRUCTURE_REVERCE):
        if (cmd.type != Cmd_from_user::REGISTER_OBJ) {
            throw;
        }
        register_road(cmd.road);
        try {
            register_town(cmd.cross);
        } catch (...) {
            undo_obj_registration(cmd.road);
            throw;
        }
        get_init_resource(cmd.cross);
        player_num--;
        if (player_num == -1) {
            state = GameState::STAGE1_DICE;
        }
        break;
    case (GameState::STAGE1_DICE) :
        if (cmd.type != Cmd_from_user::MAKE_DICE) {
            throw;
        }
        make_dice();
        break;
    case (GameState::STAGE1_DROP_RECOURCES):
        for (auto x : players) {
            drop_resource(x);
        }
        state = GameState::STAGE1_MOVE_ROBBER;
        break;
    case (GameState::STAGE1_MOVE_ROBBER) :
        if (cmd.type != Cmd_from_user::MOVE_ROBBER) {
            throw;
        }
        move_robber(cmd.hex);
        break;
    case (GameState::STAGE1_ROBBING) : {
        if (cmd.type != Cmd_from_user::ROB) {
            throw ;
        }
        std::set<int> can_be_robbed;
        auto crosses = neighbour_crosses(field->robber_pos);
        for (auto x : crosses) {
            can_be_robbed.insert(field->settlements.find(x)->second.owner);
        }
        for (auto x : can_be_robbed) {
            if (players[x].name == cmd.name) {
                rob(players[x]);
                state = GameState::STAGE2;
                return;
            }
        }
        throw ;
        }
        break;
    case (GameState::STAGE2) :
        switch (cmd.type) {
        case (Cmd_from_user::EXCHANGE_RES) :
            exchange_with_field(cmd.player_res, cmd.bank_res);
            break;
        case (Cmd_from_user::END_EXCHANGE) :
            state = GameState::STAGE3;
            break;
        default:
            throw ;
        }
    case (GameState::STAGE3) :
        switch (cmd.type) {
        case(Cmd_from_user::BUILD) :
            switch (cmd.obj_type) {
            case (ObjectType::ROAD) :
                build_road(cmd.road);
                break;
            case (ObjectType::TOWN) :
            case (ObjectType::CITY) :
                build_city(cmd.cross);
                break;
            default: throw;
            }
            break;
        case (Cmd_from_user::END_BUILDING) :
            if (!next_player()) {
                return;//WINNER!
            }
            break;
        default:
            throw;
        }
        break;
    default: throw;
    }
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

void Engine::undo_obj_registration(road_pos road) {
    auto it = field->roads.find(road);
    players[it->second.owner].object_cards[ObjectType::ROAD]++;
    field->roads.erase(it);
}

void Engine::get_init_resource(cross_pos cross) {
    auto neigh_hexes = neighbour_hexes(cross);
    for (auto x : field->hexes) {
        if (neigh_hexes.count(x.second.pos)) {
            if (x.second.type != Resource::NO_RESOURCE) {
                players[player_num].resource_cards[x.second.type]++;
            }
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

void Engine::register_road(const road_pos &road) {
    if (field->roads.count(road)) {
        throw;//ERROR
    }
    if (players[player_num].object_cards[ObjectType::ROAD] == 0) {
        throw; //ERROR
    }
    if (check_owner(neighbour_roads(road), field->roads, player_num) ||
            check_owner(neighbour_crosses(road), field->settlements, player_num)) {
        field->roads.insert(std::make_pair(road, Object(player_num)));
        players[player_num].object_cards[ObjectType::ROAD]--;
        return;
    }
    throw;//ERROR
}

void Engine::register_town(const cross_pos &pos) {
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
    auto roads = neighbour_roads(pos);
    if (check_owner(neighbour_roads(pos), field->roads, player_num)) {
        field->settlements.insert(std::pair<cross_pos, Object>(pos, Object(player_num, ObjectType::TOWN)));
        players[player_num].object_cards[ObjectType::TOWN]--;
        players[player_num].score++;
        return;
    }
    throw;
}


void Engine::get_resources() {
    auto range = field->hexes.equal_range(field->dice_score);
    while (range.first != range.second) {
        auto hex = range.first->second;
        auto crosses_coord = neighbour_crosses(hex.pos);
        for (auto pos : crosses_coord) {
            if (!field->settlements.count(pos)) {
                continue;
            }
            auto settle_it = field->settlements.find(pos);
            if (settle_it->second.type == ObjectType::TOWN) {
                players[settle_it->second.owner].resource_cards[hex.type] += 1;
            } else { //CITY
               players[settle_it->second.owner].resource_cards[hex.type] += 2;
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
    auto crosses = neighbour_crosses(pos);
    for (auto it = crosses.begin(); it != crosses.end();) {
       if (!field->settlements.count(*it)) {
                it++;
            } else {
                crosses.erase(it);
            }
    }
    if (crosses.empty()) { //none to be robbed
        state = GameState::STAGE2;
        return;
    }
    state = GameState::STAGE1_ROBBING;
}

bool Engine::rob(Player& robbed) {
    if (robbed.resource_cards.sum() != 0) {
        while (true) {
            int res = rand() % 5;
            if (robbed.resource_cards[Resource(res)] != 0) {
                robbed.resource_cards[Resource(res)]--;
                players[player_num].resource_cards[Resource(res)]++;
                return true;
            }
        }
    }
    return false;
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

void Engine::build_road(road_pos pos) {
    if (players[player_num].resource_cards < road_request) {
        throw;//ERROR
    }
    register_road(pos);
    players[player_num].resource_cards -= road_request;
    field->bank += road_request;
}

void Engine::build_town(cross_pos pos) {
    if (players[player_num].resource_cards < town_request) {
        throw;//ERROR
    }
    register_town(pos);
    players[player_num].resource_cards -= town_request;
    field->bank += town_request;
}

void Engine::build_city(cross_pos pos) {
    if (players[player_num].resource_cards < city_request) {
        throw ;//ERROR
    }
    auto it = field->settlements.find(pos);
    if (it == field->settlements.end() ||
            it->second.owner != player_num || it->second.type == ObjectType::CITY) {
        throw ;
    }
    it->second.type = ObjectType::CITY;
    players[player_num].resource_cards -= city_request;
    field->bank += city_request;
    players[player_num].object_cards[ObjectType::TOWN]++;
    players[player_num].object_cards[ObjectType::CITY]--;
    players[player_num].score++;
}

bool Engine::next_player() {
    if (players[player_num].score >= 10) {
        state = GameState::FINAL;
        return false;
    }
    player_num = (player_num + 1) % players.size();
    return true;
}

field_ptr Engine::get_field() {
    return field;
}
