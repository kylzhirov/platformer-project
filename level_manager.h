#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H
#include "level.h"
#include "raylib.h"

class LevelManager {
public:
    Level &get_current_level() {
        return current_level;
    }

    [[nodiscard]] char *get_current_level_data() const {
        return current_level_data;
    }

    static LevelManager &get_instance() {
        static LevelManager instance;
        return instance;
    }

    LevelManager(const LevelManager&) = delete;
    LevelManager operator=(const LevelManager&) = delete;
    LevelManager(LevelManager&&) = delete;
    LevelManager operator=(LevelManager&&) = delete;


    bool is_inside_level(int row, int column);
    bool is_colliding(Vector2 pos, char look_for);
    char& get_collider(Vector2 pos, char look_for);
    static void reset_level_index();
    void set_current_level(const Level &current_level);

    void load_level(int offset = 0);

    static void unload_level();
    void set_level_cell(size_t row,  size_t column, char chr);


private:
    LevelManager() = default;
    ~LevelManager() = default;
    Level current_level;
    char* current_level_data;
};


#endif //LEVEL_MANAGER_H
