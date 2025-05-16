#ifndef LEVEL_H
#define LEVEL_H

#include <cstddef>

class Level {

public:
    Level() : rows{0}, columns{0}, data(nullptr) {};

    Level(const size_t rows, const size_t columns, char* data)
    : rows(rows), columns(columns), data(data) {};

    static Level &get_instance() {
        static Level instance;
        return instance;
    }

    Level(const Level&) = delete;
    Level operator=(const Level&) = delete;
    Level(Level&&) = delete;
    Level operator=(Level&&) = delete;


    bool is_inside_level(int row, int column);
    bool is_colliding(Vector2 pos, char look_for);
    char& get_collider(Vector2 pos, char look_for);

    void set_current_level(const Level &current_level);
    void set_level_cell(size_t row,  size_t column, char chr);

    void draw_level();
    void load_level(int offset = 0);
    void unload_level();



    void reset_level_index();


    // Getters
    [[nodiscard]] size_t get_rows() const {
        return rows;
    }

    [[nodiscard]] size_t get_columns() const {
        return columns;
    }

    [[nodiscard]] const char* get_data() const {
        return data;
    }

    char& get_level_cell(size_t row, size_t column);

    // Setters
    void set_rows(const size_t rows) {
        this->rows = rows;
    }

    void set_columns(const size_t columns) {
        this->columns = columns;
    }

    void set_data(char *data) {
        this->data = data;
    }

private:
    size_t rows;
    size_t columns;
    char *data = nullptr;
};

#endif //LEVEL_H
