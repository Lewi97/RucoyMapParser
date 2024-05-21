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
    * the output texture coords are locations relative to the tiles;
    * version 1182 and 120 122 all have their tile textures a bit differently so we cant make a universal solution;
    * just keep this in mind, coords are relative to tiles
    */
    auto get_map_tiles(const std::filesystem::path& chunk_folder) -> Layers;
}
