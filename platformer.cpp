#include <iostream>

#include "raylib.h"

#include "globals.h"
#include "level.h"
#include "player.h"
#include "enemy.h"
#include "graphics.h"
#include "assets.h"
#include "utilities.h"
#include "enemy_manager.h"
#include "level_manager.h"

namespace std {
    class runtime_error;
}

void update_game() {
    game_frame++;

    switch (game_state) {
        case MENU_STATE:
            if (IsKeyPressed(KEY_ENTER)) {
                SetExitKey(0);
                game_state = GAME_STATE;
                LevelManager::get_instance().load_level(0);
            }
            break;

        case GAME_STATE:
            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
                Player::get_instance().move_player_horizontally(PLAYER_MOVEMENT_SPEED);
            }

            if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
                Player::get_instance().move_player_horizontally(-PLAYER_MOVEMENT_SPEED);
            }

            // Calculating collisions to decide whether the player is allowed to jump
            Player::get_instance().set_player_on_ground(LevelManager::get_instance().is_colliding({ Player::get_instance().get_player_pos().x, Player::get_instance().get_player_pos().y + 0.1f}, WALL));
            if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_W) || IsKeyDown(KEY_SPACE)) && Player::get_instance().is_player_on_ground()) {
                player_y_velocity = -JUMP_STRENGTH;
            }

            Player::get_instance().update_player();
            EnemyManager::get_instance().update_enemies();

            if (IsKeyPressed(KEY_ESCAPE)) {
                game_state = PAUSED_STATE;
            }
            break;

        case PAUSED_STATE:
            if (IsKeyPressed(KEY_ESCAPE)) {
                game_state = GAME_STATE;
            }
            break;

        case DEATH_STATE:
            Player::get_instance().update_player_gravity();

            if (IsKeyPressed(KEY_ENTER)) {
                if (player_lives > 0) {
                    LevelManager::get_instance().load_level(0);
                    game_state = GAME_STATE;
                }
                else {
                    game_state = GAME_OVER_STATE;
                    PlaySound(game_over_sound);
                }
            }
            break;

        case GAME_OVER_STATE:
            if (IsKeyPressed(KEY_ENTER)) {
                LevelManager::get_instance().reset_level_index();
                Player::get_instance().reset_player_stats();
                game_state = GAME_STATE;
                LevelManager::get_instance().load_level();
            }
            break;

        case VICTORY_STATE:
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE)) {
                LevelManager::get_instance().reset_level_index();
                Player::get_instance().reset_player_stats();
                game_state = MENU_STATE;
                SetExitKey(KEY_ESCAPE);
            }
            break;
    }
}

void draw_game() {
    switch(game_state) {
        case MENU_STATE:
            ClearBackground(BLACK);
            Graphics::get_instance().draw_menu();
            break;

        case GAME_STATE:
            ClearBackground(BLACK);
            Graphics::get_instance().draw_parallax_background();
            LevelManager::get_instance().draw_level();
            Graphics::get_instance().draw_game_overlay();
            break;

        case DEATH_STATE:
            ClearBackground(BLACK);
            Graphics::get_instance().draw_death_screen();
            break;

        case GAME_OVER_STATE:
            ClearBackground(BLACK);
            Graphics::get_instance().draw_game_over_menu();
            break;

        case PAUSED_STATE:
            ClearBackground(BLACK);
            Graphics::get_instance().draw_pause_menu();
            break;

        case VICTORY_STATE:
            Graphics::get_instance().draw_victory_menu();
            break;
    }
}

int main() {
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(1024, 480, "Platformer");
    SetTargetFPS(60);
    HideCursor();

    Assets::get_instance().load_fonts();
    Assets::get_instance().load_images();
    Assets::get_instance().load_sounds();

    try {
        LevelManager::get_instance().rle_load("data/levels.rll");
        LevelManager::get_instance().load_level();
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }



    while (!WindowShouldClose()) {
        BeginDrawing();

        update_game();
        draw_game();

        EndDrawing();
    }


    LevelManager::get_instance().unload_level();
    Assets::get_instance().unload_sounds();
    Assets::get_instance().unload_images();
    Assets::get_instance().unload_fonts();

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
