#ifndef ENEMY_MANAGER_H
#define ENEMY_MANAGER_H

#include <raylib.h>

#include <vector>
#include "enemy.h"

class EnemyManager {
public:
    static EnemyManager &get_instance()
    {
        static EnemyManager instance;
        return instance;
    }
    EnemyManager(const EnemyManager &) = delete;
    EnemyManager operator=(const EnemyManager &) = delete;
    EnemyManager(EnemyManager &&) = delete;
    EnemyManager operator=(EnemyManager &&) = delete;

    void spawn_enemies();
    void update_enemies();
    bool is_colliding_with_enemies(Vector2 pos) const;
    void remove_colliding_enemy(Vector2 pos);

    [[nodiscard]] const std::vector<Enemy>& get_enemies() const
    {
        return enemies;
    }
private:
    EnemyManager() = default;
    ~EnemyManager() = default;

    std::vector<Enemy> enemies{};
    
};

#endif // ENEMY_MANAGER_H
