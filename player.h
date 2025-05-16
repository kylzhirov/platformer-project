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


    void reset_player_stats();

    void increment_player_score();

    int get_total_player_score();

    void spawn_player();

    void kill_player();

    void move_player_horizontally(float delta);

    void update_player_gravity();

    void update_player();

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

    void set_player_pos_x(const float x) {
        this->player_pos.x = x;
    }

    void set_player_pos_y(const float y) {
        this->player_pos.y = y;
    }

    float get_player_pos_y() {
        return player_pos.y;
    }

    float get_player_pos_x() {
        return player_pos.x;
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
