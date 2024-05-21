#pragma once

#include <array>
#include <vector>

namespace rucoy
{
    template<typename T>
    struct Vec2
    {
        T x{};
        T y{};
    };

    using Vec2f = Vec2<float>;
    using Vec2i = Vec2<int>;

    template<typename T>
    struct Bounds
    {
        T width{};
        T height{};
    };

    using WorldPosition = Vec2<float>;
    using TextureCoord = Vec2i;
    using MapPosition = Vec2<int>;

    struct TextureRegion
    {
        TextureCoord position{};
        Bounds<int> size{};
    };

    struct MultiLayeredTile
    {
        auto add(TextureCoord coord) -> void
        {
            tiles.at(tiles_written_to++) = coord;
        }
    private:
        std::array<TextureCoord, 3> tiles{};
        size_t tiles_written_to{};
    };

    using Tiles = std::vector<MultiLayeredTile>;
    using Layers = std::vector<Tiles>;
}
