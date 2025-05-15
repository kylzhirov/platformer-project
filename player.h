#ifndef PLAYER_H
#define PLAYER_H

#include "globals.h"
#include "level.h"
#include "enemy_manager.h"

class Player {
public:

    static Player& get_instance() {
        static Player instance;
        return instance;
    }

    Player(const Player&) = delete;
    Player operator=(const Player&) = delete;
    Player(Player&&) = delete;
    Player operator=(Player&&) = delete;


    void reset_player_stats() {
        player_lives = MAX_PLAYER_LIVES;
        for (int i = 0; i < LEVEL_COUNT; i++) {
            player_level_scores[i] = 0;
        }
    }

    void increment_player_score() {
        PlaySound(coin_sound);
        player_level_scores[level_index]++;
    }

    int get_total_player_score() {
        int sum = 0;

        for (int i = 0; i < LEVEL_COUNT; i++) {
            sum += player_level_scores[i];
        }

        return sum;
    }

    void spawn_player() {
        player_y_velocity = 0;

        for (size_t row = 0; row < Level::get_instance().get_rows(); ++row) {
            for (size_t column = 0; column < Level::get_instance().get_columns(); ++column) {
                char cell = Level::get_instance().get_level_cell(row, column);;

                if (cell == PLAYER) {
                    player_pos.x = column;
                    player_pos.y = row;
                    Level::get_instance().set_level_cell(row, column, AIR);
                    return;
                }
            }
        }
    }

    void kill_player() {
        // Decrement a life and reset all collected coins in the current level
        PlaySound(player_death_sound);
        game_state = DEATH_STATE;
        player_lives--;
        player_level_scores[level_index] = 0;
    }

    void move_player_horizontally(float delta) {
        // See if the player can move further without touching a wall;
        // otherwise, prevent them from getting into a wall by rounding their position
        float next_x = player_pos.x + delta;
        if (!Level::get_instance().is_colliding({next_x, player_pos.y}, WALL)) {
            player_pos.x = next_x;
        }
        else {
            player_pos.x = roundf(player_pos.x);
            return;
        }

        // For drawing player animations
        looking_forward = delta > 0;
        if (delta != 0) moving = true;
    }

    void update_player_gravity() {
        // Bounce downwards if approaching a ceiling with upwards velocity
        if (Level::get_instance().is_colliding({player_pos.x, player_pos.y - 0.1f}, WALL) && player_y_velocity < 0) {
            player_y_velocity = CEILING_BOUNCE_OFF;
        }

        // Add gravity to player's y-position
        player_pos.y += player_y_velocity;
        player_y_velocity += GRAVITY_FORCE;

        // If the player is on ground, zero player's y-velocity
        // If the player is *in* ground, pull them out by rounding their position
        player_on_ground = Level::get_instance().is_colliding({player_pos.x, player_pos.y + 0.1f}, WALL);
        if (player_on_ground) {
            player_y_velocity = 0;
            player_pos.y = roundf(player_pos.y);
        }
    }

    void update_player() {
        update_player_gravity();

        // Interacting with other level elements
        if (Level::get_instance().is_colliding(player_pos, COIN)) {
            Level::get_instance().get_collider(player_pos, COIN) = AIR; // Removes the coin
            increment_player_score();
        }

        if (Level::get_instance().is_colliding(player_pos, EXIT)) {
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
                Level::get_instance().load_level(1);
                PlaySound(exit_sound);
            }
        }
        else {
            // Decrement the level timer if not at an exit
            if (timer >= 0) timer--;
        }

        // Kill the player if they touch a spike or fall below the level
        if (Level::get_instance().is_colliding(player_pos, SPIKE) || player_pos.y > Level::get_instance().get_rows()) {
            kill_player();
        }

        // Upon colliding with an enemy...
        if (EnemiesManager::getInstance().is_colliding_with_enemies(player_pos)) {
            // ...check if their velocity is downwards...
            if (player_y_velocity > 0) {
                // ...if yes, award the player and kill the enemy
                EnemiesManager::getInstance().remove_colliding_enemy(player_pos);
                PlaySound(kill_enemy_sound);

                increment_player_score();
                player_y_velocity = -BOUNCE_OFF_ENEMY;
            }
            else {
                // ...if not, kill the player
                kill_player();
            }
        }
    }

    [[nodiscard]] Vector2 get_player_pos() const {
        return player_pos;
    }

    [[nodiscard]] bool is_player_on_ground() const {
        return player_on_ground;
    }

    [[nodiscard]] bool is_looking_forward() const {
        return looking_forward;
    }

    [[nodiscard]] bool is_moving() const {
        return moving;
    }

    void set_player_pos(const Vector2 &player_pos) {
        this->player_pos = player_pos;
    }

    void set_player_on_ground(bool player_on_ground) {
        this->player_on_ground = player_on_ground;
    }

    void set_looking_forward(bool looking_forward) {
        this->looking_forward = looking_forward;
    }

    void set_moving(bool moving) {
        this->moving = moving;
    }



private:
    Vector2 player_pos;

    bool player_on_ground;
    bool looking_forward;
    bool moving;

    Player() = default;
    ~Player() = default;
};



#endif //PLAYER_H
