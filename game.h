#ifndef GAME_H
#define GAME_H



class Game {
public:
    static Game& get_instance() {
        static Game instance;
        return instance;
    }

private:
    Game() = default;
    ~Game() = default;
};



#endif //GAME_H
