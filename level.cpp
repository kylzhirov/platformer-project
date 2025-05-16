#include <raylib.h>

#include "level.h"

#include <fstream>
#include <sstream>

#include "globals.h"
#include "player.h"

bool Level::is_inside_level(int row, int column) {
    if (row < 0 || row >= get_instance().get_rows()) return false;
    if (column < 0 || column >= get_instance().get_columns()) return false;
    return true;
}

bool Level::is_colliding(Vector2 pos, char look_for) {
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

char& Level::get_collider(Vector2 pos, char look_for) {
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


void Level::load_level(const int offset) {
    level_index += offset;

    // Win logic
    if (level_index >= LEVEL_COUNT) {
        game_state = VICTORY_STATE;
        create_victory_menu_background();
        level_index = 0;
        return;
    }

    // Level duplication
    const size_t rows = LEVELS[level_index]->get_rows();
    const size_t columns = LEVELS[level_index]->get_columns();
    get_instance().set_data(new char[rows * columns]);

    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            const char* source_data = LEVELS[level_index]->get_data();
            get_instance().data[row * columns + column] = source_data[row * columns + column];
        }
    }

    //  current_level = {rows, columns, current_level_data};

    Level& level = Level::get_instance();
    level.set_rows(rows);
    level.set_columns(columns);
    // data is already set

    // Instantiate entities
    Player::get_instance().spawn_player();
    EnemiesManager::getInstance().spawn_enemies();

    // Calculate positioning and sizes
    derive_graphics_metrics_from_loaded_level();

    // Reset the timer
    timer = MAX_LEVEL_TIME;
}

void Level::unload_level() {
    delete[] get_instance().data;
}

void Level::reset_level_index() {
    level_index = 0;
}

void Level::set_level_cell(size_t row,size_t column, char chr) {
    get_instance().get_level_cell(row, column) = chr;
}


char& Level::get_level_cell(size_t row, size_t column) {
    return get_instance().data[row * get_instance().get_columns() + column];
}

void Level::draw_level() {
    // Move the x-axis' center to the middle of the screen
    horizontal_shift = (screen_size.x - cell_size) / 2;

    for (size_t row = 0; row < Level::get_instance().get_rows(); ++row) {
        for (size_t column = 0; column < Level::get_instance().get_columns(); ++column) {

            Vector2 pos = {
                // Move the level to the left as the player advances to the right,
                // shifting to the left to allow the player to be centered later
                (static_cast<float>(column) - Player::get_instance().get_player_pos().x) * cell_size + horizontal_shift,
                static_cast<float>(row) * cell_size
        };

            // Draw the level itself
            char cell = Level::get_instance().get_level_cell(row, column);
            switch (cell) {
                case WALL:
                    draw_image(wall_image, pos, cell_size);
                    break;
                case WALL_DARK:
                    draw_image(wall_dark_image, pos, cell_size);
                    break;
                case SPIKE:
                    draw_image(spike_image, pos, cell_size);
                    break;
                case COIN:
                    draw_sprite(coin_sprite, pos, cell_size);
                    break;
                case EXIT:
                    draw_image(exit_image, pos, cell_size);
                    break;
                default:
                    break;
            }
        }
    }

    Player::get_instance().draw_player();
    EnemiesManager::getInstance().draw_enemies();
}


// void Level::set_current_level(const Level &current_level) {
//     this->current_level = current_level;
// }



