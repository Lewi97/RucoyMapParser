#pragma once

#include <array>
#include <vector>

#include "ByteStream.h"

/*
* Most of the types defined within this header deserve their own header but i'm lazy
*/

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
    using MapPosition = Vec2<int>;

    constexpr auto max_layers = 4ull;
    constexpr auto chunk_size = Bounds{ 16, 10 };
    constexpr auto tile_size = 26.f; /* The game reads 24 but i dont feel like doing offsets */

    struct MapInfo
    {
        MapInfo() = default;

        float tile_size{ ::rucoy::tile_size };

        int layers{ 0 };
        std::vector<int> tile_layers{};

        struct {
            float width;
            float height;
        } metrics{};

        struct {
            int width{ chunk_size.width };
            int height{ chunk_size.height };
        } chunk{};
    };

    using TextureCoord = Vec2i;
    struct MultiLayeredTile
    {
        std::array<TextureCoord, 3> tiles{};
    };

    using Tiles = std::vector<MultiLayeredTile>;
    using Layers = std::vector<Tiles>;

    auto map_info_from_bytes(ByteStream& stream) -> MapInfo;
    auto get_map_tiles(const MapInfo& map_info, const std::filesystem::path& chunk_folder) -> Layers;
}

