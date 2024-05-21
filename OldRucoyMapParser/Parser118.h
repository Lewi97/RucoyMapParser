#pragma once

#include "Metrics.h"
#include "ByteStream.h"
#include "MusicRegion.h"

/*
* v118 - v122
* new map up to demons
*/
namespace rucoy::v118
{
    constexpr auto map_width = 418;
    constexpr auto map_height = 553;

    /*
    * (TODO) implement zlib compression
    * first four bytes in big endian denote the size of the uncompressed data
    */
    auto get_map_tiles(const std::filesystem::path& chunk_folder, bool is_version_122) -> Layers;
}
