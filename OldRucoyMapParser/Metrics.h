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
        std::array<TextureCoord, 3> tiles{};
    };

    using Tiles = std::vector<MultiLayeredTile>;
    using Layers = std::vector<Tiles>;
}
