#include "game.h"

C_CELL::C_CELL(const C_ENEMY& enemy_here) : enemy(enemy_here) { ++enemy_quant;
 }

C_CELL::C_CELL(const C_EQUIPMENT& eq) : equipment(eq) {}

C_CELL::C_CELL() {}

void C_CELL::set_enemy(const C_ENEMY& an_enemy) {
    enemy.emplace(an_enemy);
    ++enemy_quant;
}

void C_CELL::set_equipment(const C_EQUIPMENT& item) { equipment.emplace(item); }

bool C_CELL::is_enemy_here() const { return enemy.has_value(); }

bool C_CELL::is_item_here() const { return equipment.has_value(); }

void C_CELL::enemy_dies() {
     enemy.reset();
     --enemy_quant;
}

void C_CELL::item_left() { equipment.reset(); }

C_ENEMY& C_CELL::get_enemy() { return enemy.value(); }

const C_ENEMY& C_CELL::get_enemy() const { return enemy.value(); }

C_EQUIPMENT& C_CELL::get_item() { return equipment.value(); }

const C_EQUIPMENT& C_CELL::get_item() const { return equipment.value(); }

int C_CELL::enemy_quant = 0;
