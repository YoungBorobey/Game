#include <iostream>

#include "entity.h"
#include "game.h"
#include "item.h"
int no_gui(const char* filepath) {
  view_description();
  T_MAP MAP;
  C_PLAYER player(100, 1);
  MAP = create_map_from_file(filepath);
  auto graph = get_graph(MAP, player);
  std::string action = "map size";
  bool is_action_ok = true;
  while (true) {
    Output(action, player, MAP, graph, is_action_ok);
    action = Player_Input();
    Run_action(action, player, MAP, graph, is_action_ok);
  }
  return 0;
}
