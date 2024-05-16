#include "Parser.h"

using namespace rucoy;

static auto add_tiles_from_chunk(Tiles& tiles, MapPosition offset, const MapInfo& info, ByteStream& stream, int tile_layers) -> void
{
    for (auto layer = 0; layer < tile_layers; layer++)
    {
        for (auto x{ offset.x }; x < offset.x + 16; x++)
            for (auto y{ offset.y }; y < offset.y + 10; y++)
            {
                if (stream.read() == 1)
                {
                    auto texture_x = stream.read();
                    auto texture_y = stream.read();
                    tiles[x + y * info.metrics.width].tiles[layer] = { texture_x, texture_y };
                }
            }
    }
}

/*
* Load in all chunk files by bruteforce finding them
*/
static auto get_entire_layer(int layer, const MapInfo& info, const std::filesystem::path& chunk_folder) -> Tiles
{
    constexpr auto map_max_x = 32;
    constexpr auto map_max_y = 32;

    auto tiles = Tiles(info.metrics.width * info.metrics.height);
    auto reader = ByteStream{};

    for (auto x{ 0 }; x < map_max_x; x++)
        for (auto y{ 0 }; y < map_max_y; y++)
        {
            auto chunk_filename = std::format("{}_{}_{}", layer, x, y);
            reader.read_file(chunk_folder / chunk_filename);
            if (reader.valid())
            {
                add_tiles_from_chunk(tiles, { x * info.chunk.width, y * info.chunk.height }, info, reader, info.tile_layers.at(layer));
            }
        }

    return tiles;
}

auto rucoy::get_map_tiles(const MapInfo& map_info, const std::filesystem::path& chunk_folder) -> Layers
{
    auto layers = Layers(map_info.layers);
    auto world_layers = map_info.layers;

    for (auto count{ 0 }; auto& layer : layers)
    {
        layer = get_entire_layer(count, map_info, chunk_folder);
        
        count++;
    }

    return layers;
}

auto rucoy::map_info_from_bytes(ByteStream& stream) -> MapInfo
{
    auto map = MapInfo();
    map.metrics.width = stream.read() * 256 + stream.read();
    map.metrics.height = stream.read() * 256 + stream.read();
    map.layers = stream.read();
    for (auto layer{ 0 }; layer < map.layers; layer++)
    {
        map.tile_layers.push_back(stream.read());
    }
    return map;
}
