#include <exception>
#include <fstream>
#include <cstring>

#include "enemy_manager.h"
#include "game.h"
#include "level.h"
#include "raylib.h"
#include "globals.h"
#include "player.h"
#include "level_manager.h"
#include "graphics.h"

void LevelManager::draw_level() {
    // Move the x-axis' center to the middle of the screen
    horizontal_shift = (screen_size.x - cell_size) / 2;

    for (size_t row = 0; row < get_instance().get_current_level().get_rows(); ++row) {
        for (size_t column = 0; column < get_instance().get_current_level().get_columns(); ++column) {

            Vector2 pos = {
                // Move the level to the left as the player advances to the right,
                // shifting to the left to allow the player to be centered later
                (static_cast<float>(column) - Player::get_instance().get_player_pos_x()) * cell_size + horizontal_shift,
                static_cast<float>(row) * cell_size
        };

            // Draw the level itself
            char cell = get_instance().get_current_level().get_level_cell(row, column);
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
    EnemyManager::get_instance().draw_enemies();
}

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
            if (get_instance().get_current_level().get_level_cell(row, column) == look_for) {
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
            if (get_instance().get_current_level().get_level_cell(row, column) == look_for) {
                Rectangle block_hitbox = {(float) column, (float) row, 1.0f, 1.0f};
                if (CheckCollisionRecs(player_hitbox, block_hitbox)) {
                    return get_instance().get_current_level().get_level_cell(row, column);
                }
            }
        }
    }

    // If failed, get an approximation
    return get_instance().get_current_level().get_level_cell(pos.x, pos.y);
}

void LevelManager::reset_level_index() {
    level_index = 0;
}

void LevelManager::load_level(int offset) {
    level_index += offset;

    // Win logic
    if (level_index >= LEVEL_COUNT) {
        game_state = VICTORY_STATE;
        Graphics::get_instance().create_victory_menu_background();
        level_index = 0;
        return;
    }

    // Level duplication
    size_t rows = levels[level_index].get_rows();
    size_t columns = levels[level_index].get_columns();
    current_level_data = new char[rows * columns];



    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            const char* source_data = levels[level_index].get_data();
            current_level_data[row * columns + column] = source_data[row * columns + column];
        }
    }
    get_instance().set_current_level(Level{rows, columns, current_level_data});
    Game::get_instance().spawn_player();
    EnemyManager::get_instance().spawn_enemies();

    Graphics::get_instance().derive_graphics_metrics_from_loaded_level();

    // Reset the timer
    timer = MAX_LEVEL_TIME;
}

void LevelManager::unload_level() {
    delete[] get_instance().get_current_level_data();
}


std::vector<Level> LevelManager::rle_load(const std::string& file_path) {
    std::ifstream level_file_stream(file_path);

    if (!level_file_stream) {
        throw ("Failed to access level file");
    }

    std::string content_line;

    while (std::getline(level_file_stream, content_line)) {
        if (content_line[0] == ';' || content_line.empty()) {
            continue;
        }
        levels.push_back(parse_level(content_line));
    }

    return levels;
}

Level LevelManager::parse_level(const std::string& rle_data_line) {

    std::string counter;
    std::vector<std::string> level_lines;
    std::string current_line_buffer;


    for (size_t i = 0; i < rle_data_line.size(); i++) {

        char current_char = rle_data_line[i];

        if (current_char == '|') {
            level_lines.push_back(current_line_buffer);
            current_line_buffer = "";
            counter = "";
        } else if (current_char == ';') {
            if (!current_line_buffer.empty()) {
                level_lines.push_back(current_line_buffer);
            }
            break;
        } else if (current_char >= '0' && current_char <= '9') {
            counter += current_char;
        } else {
            int count = 1;
            if (!counter.empty()) {
                count = std::stoi(counter);
                counter.clear();
            }

            const std::string valid_elements = "#=-@*^&E";
            if (valid_elements.find(current_char) == std::string::npos) {
                throw std::runtime_error("Encountered invalid level element");
            }
            for (int j = 0; j < count; j++) {
                current_line_buffer += current_char;
            }
        }
    }
    if (!current_line_buffer.empty()) level_lines.push_back(current_line_buffer);

    if (level_lines.empty()) throw std::runtime_error("Level data is empty");

    size_t line_length = level_lines[0].length();
    for (const auto& line : level_lines) {
        if (line.length() != line_length) {
            throw std::runtime_error("Level contains rows of inconsistent length");
        }
    }
    size_t height = level_lines.size();
    size_t width = line_length;
    char* level_data = new char[height * width];

    for (size_t y = 0; y < height; y++) {
        memcpy(&level_data[y * width], level_lines[y].data(), width);
    }

    return {height, width, level_data};
}

void LevelManager::set_level_cell(size_t row, size_t column, char chr) {
    get_instance().get_current_level().get_level_cell(row, column) = chr;
}

void LevelManager::set_current_level(const Level &current_level) {
    this->current_level = current_level;
}