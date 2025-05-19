#ifndef IMAGES_H
#define IMAGES_H

#include "raylib.h"
#include "globals.h"

#include <string>
#include <cassert>

class Assets {
public:
    static Assets& get_instance() {
        static Assets instance;
        return instance;
    }

    Assets(const Assets&) = delete;
    Assets operator=(const Assets&) = delete;
    Assets(Assets&&) = delete;
    Assets operator=(Assets&&) = delete;

    void load_fonts();
    void unload_fonts();

    void load_images();
    void unload_images();

    void draw_image(Texture2D image, Vector2 pos, float width, float height);
    void draw_image(Texture2D image, Vector2 pos, float size);

    sprite load_sprite(
            const std::string &file_name_prefix,
            const std::string &file_name_suffix,
            size_t frame_count = 1,
            bool loop = true,
            size_t frames_to_skip = 3
    );

    void unload_sprite(sprite &sprite);
    void draw_sprite(sprite &sprite, Vector2 pos, float width, float height);
    void draw_sprite(sprite &sprite, Vector2 pos, float size);

    void load_sounds();
    void unload_sounds();
private:
    Assets() = default;
    ~Assets() = default;
};

#endif // IMAGES_H
