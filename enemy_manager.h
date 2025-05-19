#ifndef ENEMIES_MANAGER_H
#define ENEMIES_MANAGER_H

#include <vector>
#include <raylib.h>
#include "enemy.h"

class EnemyManager {
public:
    static EnemyManager &get_instance() {
        static EnemyManager instance;
        return instance;
    }

    EnemyManager(const EnemyManager&) = delete;
    EnemyManager operator=(const EnemyManager&) = delete;
    EnemyManager(EnemyManager&&) = delete;
    EnemyManager operator=(EnemyManager&&) = delete;

    void spawn_enemies();
    void update_enemies();
    void remove_colliding_enemy(Vector2 pos);
    void draw_enemies();

    [[nodiscard]] bool is_colliding_with_enemies(Vector2 pos) const;
    [[nodiscard]] std::vector<Enemy>& get_enemies() {
        return enemies;
    }


    private:
    EnemyManager() = default;
    ~EnemyManager() = default;
    std::vector<Enemy> enemies{};
};
#endif //ENEMIES_MANAGER_H
