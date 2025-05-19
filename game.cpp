#include "game.h"

#include "level_manager.h"
#include "player.h"


void Game::kill_player() {
    // Decrement a life and reset all collected coins in the current level
    PlaySound(player_death_sound);
    game_state = DEATH_STATE;
    Player::get_instance().set_player_lives(player_lives--);
    Player::get_instance().reset_level_score(level_index);
}

void Game::spawn_player() {
    player_y_velocity = 0;

    for (size_t row = 0; row < LevelManager::get_instance().get_current_level().get_rows(); ++row) {
        for (size_t column = 0; column < LevelManager::get_instance().get_current_level().get_columns(); ++column) {
            char cell = LevelManager::get_instance().get_current_level().get_level_cell(row, column);

            if (cell == PLAYER) {
                Player::get_instance().set_player_pos_x(column);
                Player::get_instance().set_player_pos_y(row);
                LevelManager::get_instance().set_level_cell(row, column, AIR);
                return;
            }
        }
    }
}

