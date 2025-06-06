#ifndef GLOBALS_H
#define GLOBALS_H

#include "raylib.h"
#include "level.h"
#include <vector>
#include <string>
#include <cstddef>
#include <cmath>

/* Game Elements */

inline const char WALL      = '#',
                  WALL_DARK = '=',
                  AIR       = '-',
                  SPIKE     = '^',
                  PLAYER    = '@',
                  ENEMY     = '&',
                  COIN      = '*',
                  EXIT      = 'E';

inline int level_index = 0;
inline const int LEVEL_COUNT = 3;

/* Timer-mechanic related */
inline const int MAX_LEVEL_TIME = 50 * 60;
inline int timer = MAX_LEVEL_TIME;
inline int time_to_coin_counter = 0;

/* Physics constants */

inline const float PLAYER_MOVEMENT_SPEED = 0.1f;
inline const float JUMP_STRENGTH         = 0.3f;
inline const float CEILING_BOUNCE_OFF    = 0.05f;
inline const float ENEMY_MOVEMENT_SPEED  = 0.07f;
inline const float BOUNCE_OFF_ENEMY      = 0.1f;
inline const float GRAVITY_FORCE         = 0.01f;

/* Player data */

inline float player_y_velocity = 0;
inline int player_level_scores[LEVEL_COUNT];
inline const int MAX_PLAYER_LIVES = 3;
inline int player_lives = MAX_PLAYER_LIVES;

/* Graphic Metrics */

// UI
inline const float SCREEN_SCALE_DIVISOR = 700.0f; // The divisor was found through experimentation
                                                  // to scale things accordingly to look pleasant.
inline Vector2 screen_size;
inline float screen_scale; // Used to scale str/UI components size and displacements based on the screen_size size
inline float cell_size;
inline float horizontal_shift;

// Parallax background scrolling
inline Vector2 background_size;
inline float background_y_offset;

inline const float PARALLAX_PLAYER_SCROLLING_SPEED = 0.003f;
inline const float PARALLAX_IDLE_SCROLLING_SPEED = 0.00005f;
inline const float PARALLAX_LAYERED_SPEED_DIFFERENCE = 3.0f;

/* Fonts */

inline Font menu_font;



/* Display Text Parameters */

struct Text {
    std::string str;
    Vector2 position = {0.50f, 0.50f};
    float size = 32.0f;
    Color color = WHITE;
    float spacing = 4.0f;
    Font* font = &menu_font;
};

inline Text game_title = {
    "Platformer",
    {0.50f, 0.50f},
    100.0f,
    RED
};

inline Text game_subtitle = {
    "Press Enter to Start",
    {0.50f, 0.65f}
};

inline Text game_paused = {
    "Press Escape to Resume"
};

inline Text death_title = {
    "You Died!",
    {0.50f, 0.50f},
    80.0f,
    RED
};

inline Text death_subtitle = {
    "Press Enter to Try Again",
    {0.50f, 0.65f}
};

inline Text game_over_title = {
    "Game Over",
    {0.50f, 0.50f},
    120.0f,
    RED
};

inline Text game_over_subtitle = {
    "Press Enter to Restart",
    {0.50f, 0.675f}
};

inline Text victory_title = {
    "You Won!",
    {0.50f, 0.50f},
    100.0f,
    RED
};

inline Text victory_subtitle = {
    "Press Enter to go back to menu",
    {0.50f, 0.65f}
};


/* Images and Sprites */

struct sprite {
    size_t frame_count    = 0;
    size_t frames_to_skip = 3;
    size_t frames_skipped = 0;
    size_t frame_index    = 0;
    bool loop = true;
    size_t prev_game_frame = 0;
    Texture2D *frames = nullptr;
};

// Level Elements
inline Texture2D wall_image;
inline Texture2D wall_dark_image;
inline Texture2D spike_image;
inline Texture2D exit_image;
inline sprite coin_sprite;


// UI Elements
inline Texture2D heart_image;

// Player
inline Texture2D player_stand_forward_image;
inline Texture2D player_stand_backwards_image;
inline Texture2D player_jump_forward_image;
inline Texture2D player_jump_backwards_image;
inline Texture2D player_dead_image;
inline sprite player_walk_forward_sprite;
inline sprite player_walk_backwards_sprite;

// Enemy
inline sprite enemy_walk;

// Background Elements
inline Texture2D background;
inline Texture2D middleground;
inline Texture2D foreground;

/* Sounds */

inline Sound coin_sound;
inline Sound exit_sound;
inline Sound player_death_sound;
inline Sound kill_enemy_sound;
inline Sound game_over_sound;

/* Victory Menu Background */

struct victory_ball {
    float x, y;
    float dx, dy;
    float radius;
};

inline const size_t VICTORY_BALL_COUNT     = 2000;
inline const float VICTORY_BALL_MAX_SPEED  = 2.0f;
inline const float VICTORY_BALL_MIN_RADIUS = 2.0f;
inline const float VICTORY_BALL_MAX_RADIUS = 3.0f;
inline const Color VICTORY_BALL_COLOR      = { 180, 180, 180, 255 };
inline const unsigned char VICTORY_BALL_TRAIL_TRANSPARENCY = 10;
inline victory_ball victory_balls[VICTORY_BALL_COUNT];

/* Frame Counter */

inline size_t game_frame = 0;

/* Game States */

enum game_state {
    MENU_STATE,
    GAME_STATE,
    PAUSED_STATE,
    DEATH_STATE,
    GAME_OVER_STATE,
    VICTORY_STATE
};
inline game_state game_state = MENU_STATE;

/* Forward Declarations */

// UTILITIES_H

// float rand_from_to(float from, float to);
// float rand_up_to(float to);

#endif // GLOBALS_H