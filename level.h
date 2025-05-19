#ifndef LEVEL_H
#define LEVEL_H

#include <cstddef>
#include <raylib.h>

class Level {

public:
    Level() : rows{0}, columns{0}, data(nullptr) {};

    Level(const size_t rows, const size_t columns, char* data)
    : rows(rows), columns(columns), data(data) {};

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

    char& get_level_cell(size_t row, size_t column) {
        return data[row * get_columns() + column];
    }

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
