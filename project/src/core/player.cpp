#include <iostream>

#include "game.h"
C_PLAYER::C_PLAYER(int hp, int dmg, int armor, int max_armor)
    : C_ENTITY(hp, dmg, false, armor), x(0), y(0), max_armor(max_armor), battle_check(false) { std::cout << "Player created\n\n"; }

int C_PLAYER::step(const std::string& direction) {
  if (direction == "move up") {
    this->y += 1;
  }
  if (direction == "move down") {
    this->y -= 1;
  }
  if (direction == "move right") {
    this->x += 1;
  }
  if (direction == "move left") {
    this->x -= 1;
  }
  return 0;
}

void C_PLAYER::pick_item(C_EQUIPMENT& item) {
  if (Inventory.find(item.get_name()) == Inventory.end()) {
    armor += item.get_armor();
    Inventory.insert({item.get_name(), item});
  }
}

void C_PLAYER::throw_item(const std::string& item) {
  if (inv_cont(item)) {
    armor -= Inventory.at(item).get_armor();
    Inventory.erase(item);
  }
}
//  inventory contains return TRUE if contains item, either FALSE
bool C_PLAYER::inv_cont(const std::string& item) const {
  return Inventory.find(item) != Inventory.end();
}
size_t C_PLAYER::get_x() const { return this->x; }
size_t C_PLAYER::get_y() const { return this->y; }
int C_PLAYER::get_armor() const { return std::min(max_armor, armor); }
const std::map<std::string, C_EQUIPMENT>& C_PLAYER::get_inventory() const {
  return Inventory;
}

bool& C_PLAYER::battle_status() { return this->battle_check; }
const bool& C_PLAYER::battle_status() const { return this->battle_check; }
