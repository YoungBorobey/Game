#include <iostream>
#include <algorithm>
#include "game.h"
std::vector<std::string> poss_act(
    const C_PLAYER& player, const std::vector<std::vector<C_CELL>>& Map) {
  size_t player_x = player.get_x(), player_y = player.get_y();
  size_t max_x = Map.at(0).size() - 1, max_y = Map.size() - 1;

  std::vector<std::string> actions;

  if (player.battle_status()) {
    actions.push_back("kick enemy");
    return actions;
  }
  if (player_x != 0) {
    actions.push_back("move left");
  }
  if (player_x != max_x) {
    actions.push_back("move right");
  }
  if (player_y != 0) {
    actions.push_back("move down");
  }
  if (player_y != max_y) {
    actions.push_back("move up");
  }
  if (Map[player_y][player_x].is_item_here()) {
    const std::string item_name = Map[player_y][player_x].get_item().get_name();
    if (!player.inv_cont(item_name)) {
      actions.push_back("pick " + item_name);
    }
  }
  std::transform(player.get_inventory().begin(), player.get_inventory().end(), std::back_inserter(actions),
                 [](std::pair<std::string, C_EQUIPMENT> i) -> std::string { return "throw " + i.first; });
  return actions;
}

int battle(C_PLAYER& player, C_ENEMY& enemy) {
    player.hit(enemy);
    if (enemy.is_dead()) {
        return 1;
    }
    enemy.hit(player);
    if (player.is_dead()) {
        return -1;
    }
    return 0;
}
