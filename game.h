#ifndef GAME_H
#define GAME_H

class Game {
public:
    static Game& get_instance() {
        static Game instance;
        return instance;
    }

    Game(const Game&) = delete;
    Game operator=(const Game&) = delete;
    Game(Game&&) = delete;
    Game operator=(Game&&) = delete;

    void spawn_enemies();
    void kill_player();
    void spawn_player();

private:
    Game() = default;
    ~Game() = default;
};



#endif //GAME_H
