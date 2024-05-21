#include "Parser118.h"

#include <functional>

using namespace rucoy;
using namespace rucoy::v118;

namespace VertexArrayLoc
{
    enum {
        WorldLocationX = 0,
        WorldLocationY = 1,
        TextureTopLeftX = 8,
        TextureTopLeftY = 9,
    };
}

using VertexArray = std::array<float, 20>;

class MapParser
{
protected:
    inline static constexpr auto identity_array = std::array{ -1, -1, -1, 0, -1, 1, 0, -1, 0, 0, 0, 1, 1, -1, 1, 0, 1, 1 };

    int jSuZ1JmKhBP8P70nQ{};
    int game_width{};
    int maybe_game_size{};
    int fkwCDeQK4UfmC68vP0mN{};
    std::vector<std::vector<int>> tiles{};
    std::vector<BitReaderVector> layers{};

    float timer{};
    int some_kind_of_flip{};
    int UWEBC7ECLQvTNvphZD6G{};
    int umSrD7cb1UsNtfU2HEpw{};
    int drawn_layer{};

    int texture_region_x{ 0 };
    int texture_region_y{ 0 };
    int tile_textures_horizontally{ 39 };
    double inv_texture_width{ 0.0009765625 };
    double inv_texture_height{ 0.0009765625 };
public:
    auto world_layers() const { return layers.size(); }

    /*
    * stream of uncompressed data from file nm1
    */
    auto load(BitReaderFileStream& stream) -> void
    {
        this->jSuZ1JmKhBP8P70nQ = stream.parse(8);
        int game_layers = stream.parse(8);
        this->game_width = stream.parse(16);
        int read_bits = stream.parse(16);
        this->maybe_game_size = stream.parse(8);
        this->fkwCDeQK4UfmC68vP0mN = stream.parse(16);
        this->tiles = std::vector<std::vector<int>>(game_layers);

        for (auto& layer : tiles)
            layer.resize(read_bits * this->game_width);

        auto streams = std::vector<BitReaderVector>(game_layers);
        for (int game_layer = 0; game_layer < game_layers; game_layer++) {
            int read_bits2 = stream.parse(16);
            for (int i2 = 0; i2 < read_bits2; i2++) {
                auto bit72 = stream.parse(7);
                auto bit7 = stream.parse(7);
                auto bit24 = stream.parse(24) + 1;
                this->tiles[game_layer][(bit7 * this->game_width) + bit72] = bit24;
            }
            int read_bits3 = stream.parse(32);
            stream.parse(stream.bits_left);
            auto bArr2 = std::vector<char>(read_bits3);
            stream.bytes.fill(bArr2);
            auto bitstream = BitReaderVector();
            bitstream.bytes = ByteStreamVector(bArr2);
            streams[game_layer] = std::move(bitstream);
        }

        layers = std::move(streams);
    }

    static auto set_sprite_vertices_according_to_1d_2d_coordinate(MapParser& info, int i, VertexArray& array) -> void
    {
        int i2 = info.texture_region_x;
        int i3 = info.tile_textures_horizontally;
        int i4 = ((i % i3) * 26) + i2 + 1;
        int i5 = ((i / i3) * 26) + info.texture_region_y + 1;
        float f = info.inv_texture_width;
        float f2 = i4 * f;
        float f3 = info.inv_texture_height;
        float f4 = i5 * f3;
        float f5 = (i4 + 24) * f;
        float f6 = (i5 + 24) * f3;
        array[3] = f2;
        array[4] = f6;
        array[8] = f2;
        array[9] = f4;
        array[13] = f5;
        array[14] = f4;
        array[18] = f5;
        array[19] = f6;
    }

    auto get_tiles_at(float x, float y, int game_layer, float deltaTime) -> std::vector<VertexArray>
    {
        auto out = std::vector<VertexArray>{};

        int i;
        int i2;
        this->timer += deltaTime;
        int one = 1;
        if (this->timer >= 1.0f) {
            this->timer = 0.0f;
            this->some_kind_of_flip = (this->some_kind_of_flip + 1) % 2;
        }
        float game_size = this->maybe_game_size;
        int i3 = (int)(x / game_size);
        int i4 = (int)(y / game_size);
        //if (this->UWEBC7ECLQvTNvphZD6G == i3 && this->umSrD7cb1UsNtfU2HEpw == i4 && this->drawn_layer == game_layer) {
        //    return out;
        //}
        this->UWEBC7ECLQvTNvphZD6G = i3;
        this->umSrD7cb1UsNtfU2HEpw = i4;
        this->drawn_layer = game_layer;

        char zero = 0;


        auto bitParser = this->layers[game_layer];
        int iter1 = 0;
        while (iter1 < 18) {
            auto iArr = this->identity_array;
            int x2 = iArr[iter1] + i3;
            int y2 = iArr[iter1 + 1] + i4;
            int tile_id = this->tiles[game_layer][(this->game_width * y2) + x2] - one;
            // std::println("{}, {}, {}", x2, y2, tile_id);
            if (tile_id < 0) {
                i = i3;
                i2 = i4;
            }
            else {
                bitParser.bytes.position(tile_id / 8);
                bitParser.parse(tile_id % 8);
                int read_bits = bitParser.parse(3);
                int i5 = this->maybe_game_size;
                int i6 = x2 * i5;
                int i7 = y2 * i5;
                int i8 = 0;
                while (true) {
                    int i9 = this->maybe_game_size;
                    if (i8 >= i9 * i9) {
                        break;
                    }
                    int read_bits2 = bitParser.parse(read_bits);
                    int i10 = this->maybe_game_size;
                    int texture_x = (i8 % i10) + i6;
                    int texture_y = (i8 / i10) + i7;
                    auto array = VertexArray{};
                    float f = texture_x;
                    array[0] = texture_x;
                    float f2 = texture_y;
                    array[1] = f2;
                    array[5] = f;
                    float f3 = texture_y + 1;
                    array[6] = f3;
                    float f4 = texture_x + 1;
                    array[10] = f4;
                    array[11] = f3;
                    array[15] = f4;
                    array[16] = f2;
                    int i11 = 0;
                    while (i11 < read_bits2) {
                        int read_bits3 = bitParser.parse(this->jSuZ1JmKhBP8P70nQ);
                        set_sprite_vertices_according_to_1d_2d_coordinate(*this, read_bits3, array);
                        int i12 = i3;
                        int i13 = i4;

                        out.push_back(array);
                        // For animations, just ignore for now
                        //if (read_bits3 < this->fkwCDeQK4UfmC68vP0mN) {
                        //    set_sprite_vertices_according_to_1d_2d_coordinate(info, read_bits3 + 1, array);
                        //}
                        // out.push_back(array);

                        i11++;
                        i3 = i12;
                        i4 = i13;
                    }
                    //out.push_back(array);
                    one = 1;
                    i8++;
                    zero = 0;
                }
                i = i3;
                i2 = i4;
                bitParser.parse(bitParser.bits_left);
            }
            iter1 += 2;
            i3 = i;
            i4 = i2;
            zero = 0;
        }
        bitParser.parse(bitParser.bits_left);

        //return {};
        return out;
    }
};

static auto parse_music(BitReaderFileStream& stream) -> std::vector<MusicRegion>
{
    auto regions = std::vector<MusicRegion>();

    int read_bits4 = stream.parse(8);
    for (int i3 = 0; i3 < read_bits4; i3++) {
        int read_bits5 = stream.parse(8);

        auto bArr3 = std::vector<char>(read_bits5);
        for (int i4 = 0; i4 < read_bits5; i4++) {
            bArr3[i4] = stream.parse(8);
        }

        auto str = std::string_view(bArr3.data(), bArr3.size());
        int x = stream.parse(10);
        int y = stream.parse(10);
        int layer = stream.parse(4);
        int width = stream.parse(10);
        int height = stream.parse(10);

        auto music_region = MusicRegion{
            .music = std::string(str),
            .height = height, .width = width,
            .x = x, .y = y, .z = layer,
        };
        regions.push_back(std::move(music_region));

        //rn6n3NQJ8Ft3eMs0yQe rn6n3nqj8ft3ems0yqe = JVQNpNt4duCxPPa6UF26.HbeQDSUrkKkwRz20L3Dv;
        //PcNW4q8MpFTLpVCMAxQ2 pcNW4q8MpFTLpVCMAxQ2 = rn6n3nqj8ft3ems0yqe.music.get(str);
        if (false /*pcNW4q8MpFTLpVCMAxQ2 != null*/) {
            //rn6n3NQJ8Ft3eMs0yQe.Pmd9wDWSa5H2KTjH3nyk pmd9wDWSa5H2KTjH3nyk = new rn6n3NQJ8Ft3eMs0yQe.Pmd9wDWSa5H2KTjH3nyk(rn6n3nqj8ft3ems0yqe, read_bits6, read_bits7, read_bits8, read_bits9, read_bits10, pcNW4q8MpFTLpVCMAxQ2);
            //rn6n3nqj8ft3ems0yqe.umSrD7cb1UsNtfU2HEpw.lock();
            //rn6n3nqj8ft3ems0yqe.Dm7ewSwq4ES2r2hY2GL.add(pmd9wDWSa5H2KTjH3nyk);
            //rn6n3nqj8ft3ems0yqe.umSrD7cb1UsNtfU2HEpw.unlock();
        }
        else {
            // std::println("Music track missing: {}", str);
        }
    }

    return regions;
}

auto rucoy::v118::get_map_tiles(const std::filesystem::path& path) -> Layers
{
    constexpr auto offset_x = 41;
    constexpr auto offset_y = 225;

    auto parser = MapParser();
    auto filestream = BitReaderFileStream();
    filestream.bytes = ByteStream(path / "assets/nm1");
    
    if (not filestream.bytes.valid()) return {};

    // auto uncompressed_size = reader.read_int();
    // zlib::decompress(buffer, file, uncompressed_size)
    parser.load(filestream);
    
    auto layers = Layers(parser.world_layers());
    for (auto layer{ 0 }; layer < layers.size(); layer++)
    {
        layers.at(layer).resize(map_width * map_height + map_width);
        auto& tiles = layers.at(layer);
        for (auto x{ 2 }; x < 64; x++)
        {
            for (auto y{ 2 }; y < 64; y++)
            {
                for (const auto& tile : parser.get_tiles_at(x * 10, y * 10, layer, 0.f))
                {
                    auto tile_x = tile[VertexArrayLoc::WorldLocationX] - offset_x;
                    auto tile_y = tile[VertexArrayLoc::WorldLocationY];// -offset_y;
                    if (tile_x < 0 or tile_y < 0) 
                        continue;
                    auto texture_pos = TextureCoord{
                        static_cast<int>(tile[VertexArrayLoc::TextureTopLeftX] * 1024),
                        static_cast<int>(tile[VertexArrayLoc::TextureTopLeftY] * 1024)
                    };

                    tiles.at(tile_x + tile_y * map_width)
                        .add(texture_pos);
                }
            }
        }
    }
    return layers;
}
