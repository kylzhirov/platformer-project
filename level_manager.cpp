#include "level_manager.h"
#include "enemy_manager.h"
#include "globals.h"

bool LevelManager::is_inside_level(int row, int column) {
    if (row < 0 || row >= get_instance().get_current_level().get_rows()) return false;
    if (column < 0 || column >= get_instance().get_current_level().get_columns()) return false;
    return true;
}

bool LevelManager::is_colliding(Vector2 pos, char look_for) {
    Rectangle entity_hitbox = {pos.x, pos.y, 1.0f, 1.0f};

    // Scan the adjacent area in the level to look for a match in collision
    for (int row = pos.y - 1; row < pos.y + 1; ++row) {
        for (int column = pos.x - 1; column < pos.x + 1; ++column) {
            // Check if the cell is out-of-bounds
            if (!get_instance().is_inside_level(row, column)) continue;
            if (Level::get_level_cell(row, column) == look_for) {
                Rectangle block_hitbox = {(float) column, (float) row, 1.0f, 1.0f};
                if (CheckCollisionRecs(entity_hitbox, block_hitbox)) {
                    return true;
                }
            }
        }
    }
    return false;
}

char& LevelManager::get_collider(Vector2 pos, char look_for) {
    // Like is_colliding(), except returns a reference to the colliding object
    Rectangle player_hitbox = {pos.x, pos.y, 1.0f, 1.0f};

    for (int row = pos.y - 1; row < pos.y + 1; ++row) {
        for (int column = pos.x - 1; column < pos.x + 1; ++column) {
            // Check if the cell is out-of-bounds
            if (!get_instance().is_inside_level(row, column)) continue;
            if (Level::get_level_cell(row, column) == look_for) {
                Rectangle block_hitbox = {(float) column, (float) row, 1.0f, 1.0f};
                if (CheckCollisionRecs(player_hitbox, block_hitbox)) {
                    return Level::get_level_cell(row, column);
                }
            }
        }
    }

    // If failed, get an approximation
    return Level::get_level_cell(pos.x, pos.y);
}

void LevelManager::reset_level_index() {
    level_index = 0;
}

void LevelManager::load_level(const int offset) {
    level_index += offset;

    // Win logic
    if (level_index >= LEVEL_COUNT) {
        game_state = VICTORY_STATE;
        create_victory_menu_background();
        level_index = 0;
        return;
    }

    // Level duplication
    const size_t rows = LEVELS[level_index].get_rows();
    const size_t columns = LEVELS[level_index].get_columns();
    current_level_data = new char[rows * columns];

    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            const char* source_data = LEVELS[level_index].get_data(); // Use the getter
            current_level_data[row * columns + column] = source_data[row * columns + column];
        }
    }

    get_instance().set_current_level(Level{rows, columns, current_level_data});
    // Instantiate entities
    spawn_player();
    EnemiesManager::getInstance().spawn_enemies();

    // Calculate positioning and sizes
    derive_graphics_metrics_from_loaded_level();

    // Reset the timer
    timer = MAX_LEVEL_TIME;
}

void LevelManager::unload_level() {
    delete[] get_instance().get_current_level_data();
}

// Getters and setters
void LevelManager::set_level_cell(size_t row,size_t column, char chr) {
    Level::get_level_cell(row, column) = chr;
}
char& Level::get_level_cell(size_t row, size_t column) {
    return LevelManager::get_instance().get_current_level().data[row * LevelManager::get_instance().get_current_level().get_columns() + column];
}
void LevelManager::set_current_level(const Level &current_level) {
    this->current_level = current_level;
}
