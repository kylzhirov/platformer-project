#include "level_manager.h"
#include "graphics.h"
#include "globals.h"
#include "player.h"
#include "assets.h"
#include "utilities.h"

void Graphics::draw_text(Text &text) {
    // Measure the text, center it to the required position, and draw it
    Vector2 dimensions = MeasureTextEx(*text.font, text.str.c_str(), text.size * screen_scale, text.spacing);

    Vector2 pos = {
        (screen_size.x * text.position.x) - (0.5f * dimensions.x),
        (screen_size.y * text.position.y) - (0.5f * dimensions.y)
    };

    DrawTextEx(*text.font, text.str.c_str(), pos, dimensions.y, text.spacing, text.color);
}

void Graphics::derive_graphics_metrics_from_loaded_level() {
    // Level and UI setup
    screen_size.x  = static_cast<float>(GetScreenWidth());
    screen_size.y = static_cast<float>(GetScreenHeight());

    cell_size = screen_size.y / static_cast<float>(LevelManager::get_instance().get_levels()[level_index].get_rows());
    screen_scale = std::min(screen_size.x, screen_size.y) / SCREEN_SCALE_DIVISOR;

    // Parallax background setup
    float larger_screen_side = std::max(screen_size.x, screen_size.y);

    if (screen_size.x > screen_size.y) {
        background_size = {larger_screen_side, larger_screen_side / 16 * 10};
    }
    else {
        background_size = {larger_screen_side / 10 * 16, larger_screen_side};
    }

    background_y_offset = (screen_size.y - background_size.y) * 0.5f;
}

void Graphics::draw_parallax_background() {
    // First uses the player's position
    float initial_offset      = -(Player::get_instance().get_player_pos().x * PARALLAX_PLAYER_SCROLLING_SPEED + game_frame * PARALLAX_IDLE_SCROLLING_SPEED);

    // Calculate offsets for different layers
    float background_offset   = initial_offset;
    float middleground_offset = background_offset * PARALLAX_LAYERED_SPEED_DIFFERENCE;
    float foreground_offset   = middleground_offset * PARALLAX_LAYERED_SPEED_DIFFERENCE;

    // Wrap offsets to create a loop effect
    background_offset   = fmod(background_offset, 1.0f);
    middleground_offset = fmod(middleground_offset, 1.0f);
    foreground_offset   = fmod(foreground_offset, 1.0f);

    // Scale to background size
    background_offset   *= background_size.x;
    middleground_offset *= background_size.x;
    foreground_offset   *= background_size.x;

    // Each layer is drawn twice, side by side, the first starting from its offset, and the other from its offset + background_size
    // This ensures a seamless scrolling effect, because when one copy moves out of sight, the second jumps into its place.
    Assets::get_instance().draw_image(background,   {background_offset + background_size.x, background_y_offset},   background_size.x, background_size.y);
    Assets::get_instance().draw_image(background,   {background_offset,                     background_y_offset},   background_size.x, background_size.y);

    Assets::get_instance().draw_image(middleground, {middleground_offset + background_size.x, background_y_offset}, background_size.x, background_size.y);
    Assets::get_instance().draw_image(middleground, {middleground_offset,                     background_y_offset}, background_size.x, background_size.y);

    Assets::get_instance().draw_image(foreground,   {foreground_offset + background_size.x, background_y_offset},   background_size.x, background_size.y);
    Assets::get_instance().draw_image(foreground,   {foreground_offset,                     background_y_offset},   background_size.x, background_size.y);
}

void Graphics::draw_game_overlay() {
    const float ICON_SIZE = 48.0f * screen_scale;

    float slight_vertical_offset = 8.0f;
    slight_vertical_offset *= screen_scale;

    // Hearts
    for (int i = 0; i < player_lives; i++) {
        const float SPACE_BETWEEN_HEARTS = 4.0f * screen_scale;
        Assets::get_instance().draw_image(heart_image, {ICON_SIZE * i + SPACE_BETWEEN_HEARTS, slight_vertical_offset}, ICON_SIZE);
    }

    // Timer
    Vector2 timer_dimensions = MeasureTextEx(menu_font, std::to_string(timer / 60).c_str(), ICON_SIZE, 2.0f);
    Vector2 timer_position = {(GetRenderWidth() - timer_dimensions.x) * 0.5f, slight_vertical_offset};
    DrawTextEx(menu_font, std::to_string(timer / 60).c_str(), timer_position, ICON_SIZE, 2.0f, WHITE);

    // Score
    Vector2 score_dimensions = MeasureTextEx(menu_font, std::to_string(Player::get_instance().get_total_player_score()).c_str(), ICON_SIZE, 2.0f);
    Vector2 score_position = {GetRenderWidth() - score_dimensions.x - ICON_SIZE, slight_vertical_offset};
    DrawTextEx(menu_font, std::to_string(Player::get_instance().get_total_player_score()).c_str(), score_position, ICON_SIZE, 2.0f, WHITE);
    Assets::get_instance().draw_sprite(coin_sprite, {GetRenderWidth() - ICON_SIZE, slight_vertical_offset}, ICON_SIZE);
}

// void draw_level();

// void draw_player();


// Menus
void Graphics::draw_menu() {
    Graphics::get_instance().draw_text(game_title);
    Graphics::get_instance().draw_text(game_subtitle);
}

void Graphics::draw_pause_menu() {
    Graphics::get_instance().draw_text(game_paused);
}

void Graphics::draw_death_screen() {
    Graphics::get_instance().draw_parallax_background();
    LevelManager::get_instance().draw_level();
    Graphics::get_instance().draw_game_overlay();
    DrawRectangle(0, 0, GetRenderWidth(), GetRenderHeight(), {0, 0, 0, 100});
    Graphics::get_instance().draw_text(death_title);
    Graphics::get_instance().draw_text(death_subtitle);
}

void Graphics::draw_game_over_menu() {
    Graphics::get_instance().draw_text(game_over_title);
    Graphics::get_instance().draw_text(game_over_subtitle);
}

void Graphics::create_victory_menu_background() {
    for (auto &ball : victory_balls) {
        ball.x  = rand_up_to(screen_size.x);
        ball.y  = rand_up_to(screen_size.y);
        ball.dx = rand_from_to(-VICTORY_BALL_MAX_SPEED, VICTORY_BALL_MAX_SPEED);
        ball.dx *= screen_scale;
        if (abs(ball.dx) < 0E-1) ball.dx = 1.0f;
        ball.dy = rand_from_to(-VICTORY_BALL_MAX_SPEED, VICTORY_BALL_MAX_SPEED);
        ball.dy *= screen_scale;
        if (abs(ball.dy) < 0E-1) ball.dy = 1.0f;
        ball.radius = rand_from_to(VICTORY_BALL_MIN_RADIUS, VICTORY_BALL_MAX_RADIUS);
        ball.radius *= screen_scale;
    }

    /* Clear both the front buffer and the back buffer to avoid ghosting of the game graphics. */
    ClearBackground(BLACK);
    EndDrawing();
    BeginDrawing();
    ClearBackground(BLACK);
    EndDrawing();
    BeginDrawing();
}

void Graphics::animate_victory_menu_background() {
    for (auto &ball : victory_balls) {
        ball.x += ball.dx;
        if (ball.x - ball.radius < 0 ||
            ball.x + ball.radius >= screen_size.x) {
            ball.dx = -ball.dx;
        }
        ball.y += ball.dy;
        if (ball.y - ball.radius < 0 ||
            ball.y + ball.radius >= screen_size.y) {
            ball.dy = -ball.dy;
        }
    }
}

void Graphics::draw_victory_menu_background() {
    for (auto &ball : victory_balls) {
        DrawCircleV({ ball.x, ball.y }, ball.radius, VICTORY_BALL_COLOR);
    }
}

void Graphics::draw_victory_menu() {
    DrawRectangle(
        0, 0,
        static_cast<int>(screen_size.x), static_cast<int>(screen_size.y),
        { 0, 0, 0, VICTORY_BALL_TRAIL_TRANSPARENCY }
    );

    animate_victory_menu_background();
    draw_victory_menu_background();

    Graphics::get_instance().draw_text(victory_title);
    Graphics::get_instance().draw_text(victory_subtitle);
}