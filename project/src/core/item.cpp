#include "item.h"

C_ITEM::C_ITEM(const std::string& name = "none", int weight = 1)
    : name(name), weight(weight) {}

void C_ITEM::set_weight(int new_weight) { weight = new_weight; }

std::string C_ITEM::get_name() const { return name; }


C_EQUIPMENT::C_EQUIPMENT(const std::string& str) : C_ITEM(str) {
  if (str == "armor") {
    set_weight(3);
    armor = 3;
    return;
  }
  if (str == "helmet") {
    set_weight(2);
    armor = 3;
    return;
  }
  if (str == "shield") {
    set_weight(7);
    armor = 5;
    return;
  }
  if (str == "pants") {
    set_weight(1);
    armor = 1;
    return;
  }
  if (str == "T-Shirt") {
    set_weight(1);
    armor = 1;
    return;
  }
}

int C_EQUIPMENT::get_armor() const { return armor; }
