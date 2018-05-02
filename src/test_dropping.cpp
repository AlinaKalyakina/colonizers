#include "interface.h"
#include <iostream>

int main()
{
    Engine engine;
    engine.join_player("ALina");
    engine.start_game(create_field());
    engine.put_initial_infrastructure(cross_pos(coord_t(-1, 1), CrossCoord::TOP),
                                      road_pos(coord_t(-1,1), RoadCoord::UP));
    engine.put_initial_infrastructure(cross_pos(coord_t(-1, 1), CrossCoord::BOTTOM),
                                      road_pos(coord_t(-1,1), RoadCoord::DOWN));
    //wood - 1, claim - 1
    for (auto i = 0; i < 20; i++) {
        engine.make_dice(5);
        engine.end_exchanges();
        engine.next_player();
    }
    //wood - 19 , claim - 1
    engine.make_dice(7);
    if (engine.get_cur_player().resource_cards.sum() != 10) {
        std::cout << "Error in dropping resource";
    }
}

