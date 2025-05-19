#include "enemy_manager.h"
#include "globals.h"
#include "level.h"
#include "level_manager.h"
#include "player.h"



void EnemyManager::update_enemies() {
    for (auto &enemy : enemies) {
        // Find the enemy's next x
        float next_x = enemy.get_pos().x;
        next_x += enemy.is_looking_right() ? ENEMY_MOVEMENT_SPEED : -ENEMY_MOVEMENT_SPEED;

        // If its next position collides with a wall, turn around
        if (LevelManager::get_instance().is_colliding({next_x, enemy.get_pos().y}, WALL)) {
            enemy.set_looking_right(!enemy.is_looking_right());
        }
        // Otherwise, keep moving
        else {
            enemy.set_pos(Vector2{next_x, enemy.get_pos().y});
        }
    }
}

// Custom is_colliding function for enemies
bool EnemyManager::is_colliding_with_enemies(const Vector2 pos) const {
    Rectangle entity_hitbox = {pos.x, pos.y, 1.0f, 1.0f};

    for (auto &enemy : enemies) {
        const Rectangle enemy_hitbox = { enemy.get_pos().x, enemy.get_pos().y, 1.0f, 1.0f};
        if (CheckCollisionRecs(entity_hitbox, enemy_hitbox)) {
            return true;
        }
    }
    return false;
}

void EnemyManager::remove_colliding_enemy(const Vector2 pos) {
    const Rectangle entity_hitbox = {pos.x, pos.y, 1.0f, 1.0f};

    for (auto it = enemies.begin(); it != enemies.end(); ++it) {
        // Erase a colliding enemy
        if (const Rectangle enemy_hitbox = {(float) it->get_pos().x, (float) it->get_pos().y, 1.0f, 1.0f}; CheckCollisionRecs(entity_hitbox, enemy_hitbox)) {
            enemies.erase(it);

            // Call the function again to remove any remaining colliding enemies
            // Cannot continue as calling erase on a vector invalidates current iterators
            remove_colliding_enemy(pos);
            return;
        }
    }
}
void EnemyManager::draw_enemies() {
    // Go over all enemies and draw them, once again accounting to the player's movement and horizontal shift
    for (auto &enemy : get_instance().get_enemies()) {
        horizontal_shift = (screen_size.x - cell_size) / 2;

        Vector2 pos = {
            (enemy.get_pos().x - Player::get_instance().get_player_pos().x) * cell_size + horizontal_shift,
            enemy.get_pos().y * cell_size
    };

        draw_sprite(enemy_walk, pos, cell_size);
    }
}
