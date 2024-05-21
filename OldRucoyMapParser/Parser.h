#pragma once

#include <array>
#include <vector>

#include "ByteStream.h"
#include "Metrics.h"

namespace rucoy
{
    /* Confirmed to support version 106 - v110*/
    namespace v110
    {
        constexpr auto max_layers = 4ull;
        constexpr auto chunk_size = Bounds{ 16, 10 };
        constexpr auto tile_size = 26.f; /* The game reads 24 but i dont feel like doing offsets */

        struct InitFile
        {
            InitFile() = default;

            float tile_size{ ::rucoy::v110::tile_size };

            int layers{ 0 };
            std::vector<int> tile_layers{};

            struct {
                float width;
                float height;
            } map{};

            struct {
                int width{ chunk_size.width };
                int height{ chunk_size.height };
            } chunk{};
        };

        auto map_info_from_bytes(ByteStream& stream) -> InitFile;
        auto get_map_tiles(const InitFile& map_info, const std::filesystem::path& chunk_folder) -> Layers;
    }
}

