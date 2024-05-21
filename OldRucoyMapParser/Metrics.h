#pragma once

#include <array>
#include <vector>
#include <compare>

namespace rucoy
{
    template<typename T>
    struct Vec2
    {
        T x{};
        T y{};

        constexpr auto operator<=>(const Vec2<T>&) const = default;
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
    constexpr auto invalid_texture_coord = TextureCoord{ -1,-1 };
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
            if (_tiles_written_to < _tiles.size())
                _tiles.at(_tiles_written_to++) = coord;
        }

        auto tiles(this auto&& self) { return std::forward<decltype(self)>(self)._tiles; }
    private:
        std::array<TextureCoord, 6> _tiles{ invalid_texture_coord , invalid_texture_coord , invalid_texture_coord, invalid_texture_coord, invalid_texture_coord, invalid_texture_coord };
        size_t _tiles_written_to{};
    };

    using Tiles = std::vector<MultiLayeredTile>;
    using Layers = std::vector<Tiles>;
}
