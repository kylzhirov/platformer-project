#include "raylib.h"

#include "player.h"
#include "level_manager.h"

Vector2 currentPos = Player::get_instance().get_player_pos();

void Player::reset_player_stats() {
    player_lives = MAX_PLAYER_LIVES;
    for (int i = 0; i < LEVEL_COUNT; i++) {
        player_level_scores[i] = 0;
    }
}

void Player::increment_player_score() {
    PlaySound(coin_sound);
    player_level_scores[level_index]++;
}

int Player::get_total_player_score() {
    int sum = 0;

    for (int i = 0; i < LEVEL_COUNT; i++) {
        sum += player_level_scores[i];
    }

    return sum;
}

void Player::spawn_player() {
    player_y_velocity = 0;

    for (size_t row = 0; row < LevelManager::get_instance().get_current_level().get_rows(); ++row) {
        for (size_t column = 0; column < LevelManager::get_instance().get_current_level().get_columns(); ++column) {
            char cell = LevelManager::get_instance().get_current_level().get_level_cell(row, column);;

            if (cell == PLAYER) {
                get_instance().set_player_pos_x(column);
                get_instance().set_player_pos_y(row);
                LevelManager::get_instance().set_level_cell(row, column, AIR);
                return;
            }
        }
    }
}

void Player::kill_player() {
    // Decrement a life and reset all collected coins in the current level
    PlaySound(player_death_sound);
    game_state = DEATH_STATE;
    player_lives--;
    player_level_scores[level_index] = 0;
}

void Player::move_player_horizontally(float delta) {
    // See if the player can move further without touching a wall;
    // otherwise, prevent them from getting into a wall by rounding their position
    float next_x = get_instance().get_player_pos().x + delta;
    if (!LevelManager::get_instance().is_colliding({next_x, get_instance().get_player_pos_y()}, WALL)) {
        get_instance().set_player_pos_x(next_x);
    }
    else {
        get_instance().set_player_pos_x(roundf(get_instance().get_player_pos().x));
        return;
    }

    // For drawing player animations
    get_instance().set_looking_forward(delta > 0);
    if (delta != 0) get_instance().set_moving(true);
}

void Player::update_player_gravity() {
    // Bounce downwards if approaching a ceiling with upwards velocity
    if (LevelManager::get_instance().is_colliding({get_instance().get_player_pos().x, get_instance().get_player_pos().y - 0.1f}, WALL) && player_y_velocity < 0) {
        player_y_velocity = CEILING_BOUNCE_OFF;
    }

    // Add gravity to player's y-position
    get_instance().set_player_pos_y(get_instance().get_player_pos_y() + player_y_velocity);
    player_y_velocity += GRAVITY_FORCE;

    // If the player is on ground, zero player's y-velocity
    // If the player is *in* ground, pull them out by rounding their position
    get_instance().set_player_on_ground(LevelManager::get_instance().is_colliding({get_instance().get_player_pos().x, get_instance().get_player_pos().y + 0.1f}, WALL));
    if (get_instance().is_player_on_ground()) {
        player_y_velocity = 0;
        get_instance().set_player_pos_y(roundf(get_instance().get_instance().get_player_pos().y));
    }
}

void Player::update_player() {
    Player::get_instance().update_player_gravity();

    // Interacting with other level elements
    if (LevelManager::get_instance().is_colliding(get_instance().get_player_pos(), COIN)) {
        LevelManager::get_instance().get_collider(get_instance().get_player_pos(), COIN) = AIR; // Removes the coin
        increment_player_score();
    }

    if (LevelManager::get_instance().is_colliding(get_instance().get_player_pos(), EXIT)) {
        // Reward player for being swift
        if (timer > 0) {
            // For every 9 seconds remaining, award the player 1 coin
            timer -= 25;
            time_to_coin_counter += 5;

            if (time_to_coin_counter / 60 > 1) {
                increment_player_score();
                time_to_coin_counter = 0;
            }
        }
        else {
            // Allow the player to exit after the level timer goes to zero
            LevelManager::get_instance().load_level(1);
            PlaySound(exit_sound);
        }
    }
    else {
        // Decrement the level timer if not at an exit
        if (timer >= 0) timer--;
    }

    // Kill the player if they touch a spike or fall below the level
    if (LevelManager::get_instance().is_colliding(get_instance().get_player_pos(), SPIKE) || get_instance().get_player_pos().y > LevelManager::get_instance().get_current_level().get_rows()) {
        kill_player();
    }

    // Upon colliding with an enemy...
    if (EnemiesManager::getInstance().is_colliding_with_enemies(get_instance().get_player_pos())) {
        // ...check if their velocity is downwards...
        if (player_y_velocity > 0) {
            // ...if yes, award the player and kill the enemy
            EnemiesManager::getInstance().remove_colliding_enemy(get_instance().get_player_pos());
            PlaySound(kill_enemy_sound);

            increment_player_score();
            player_y_velocity = -BOUNCE_OFF_ENEMY * 3;
        }
        else {
            // ...if not, kill the player
            kill_player();
        }
    }
}


void Player::draw_player() {
    horizontal_shift = (screen_size.x - cell_size) / 2;

    // Shift the camera to the center of the screen to allow to see what is in front of the player
    Vector2 pos = {
        horizontal_shift,
        get_instance().get_player_pos().y * cell_size
};

    // Pick an appropriate sprite for the player
    if (game_state == GAME_STATE) {
        if (!get_instance().is_player_on_ground()) {
            draw_image((get_instance().is_looking_forward() ? player_jump_forward_image : player_jump_backwards_image), pos, cell_size);
        }
        else if (get_instance().is_moving()) {
            draw_sprite((get_instance().is_looking_forward() ? player_walk_forward_sprite : player_walk_backwards_sprite), pos, cell_size);
            get_instance().set_moving(false);
        }
        else {
            draw_image((Player::get_instance().is_looking_forward() ? player_stand_forward_image : player_stand_backwards_image), pos, cell_size);
        }
    }
    else {
        draw_image(player_dead_image, pos, cell_size);
    }
}
