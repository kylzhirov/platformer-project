#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "level_manager.h"
#include "globals.h"



class Graphics {
public:
    static Graphics& get_instance() {
        static Graphics instance;
        return instance;
    }

    Graphics(const Graphics&) = delete;
    Graphics operator=(const Graphics&) = delete;
    Graphics(Graphics&&) = delete;
    Graphics operator=(Graphics&&) = delete;

    void draw_text(Text &text);
    void derive_graphics_metrics_from_loaded_level();
    void draw_game_overlay();
    void draw_level();
    void draw_player();
    void draw_enemies();
    void draw_menu();
    void draw_parallax_background();

    void draw_pause_menu();
    void draw_death_screen();
    void draw_game_over_menu();

    void create_victory_menu_background();
    void animate_victory_menu_background();
    void draw_victory_menu_background();
    void draw_victory_menu();
private:
    Graphics() = default;
    ~Graphics() = default;
};
#endif //GRAPHICS_H
