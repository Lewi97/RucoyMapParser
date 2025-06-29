#include "Parser.h"
#include <cstddef>
#include <png++/image.hpp>
#include <png++/png.hpp>
#include <png++/reader.hpp>
#include <png++/rgb_pixel.hpp>
#include <print>

namespace
{
    struct Region
    {
        size_t x{};
        size_t y{};
        size_t width{};
        size_t height{};
    };

    template<typename Pixel>
    struct ImageRegion
    {
        png::image<Pixel>* image{};
        Region region{};
    };

    template<typename Pixel>
    struct Tilesheet
    {
        size_t tile_height{};
        size_t tile_width{};
        png::image<Pixel> _sheet{};

        auto at(size_t x, size_t y) -> ImageRegion<Pixel>
        {
            return ImageRegion<Pixel>{
                &_sheet,
                { x * tile_width, y * tile_height, tile_width, tile_height }
            };
        }
    };

    constexpr auto is_equal(png::rgb_pixel lhs, png::rgb_pixel rhs) noexcept
    {
        return lhs.red == rhs.red && lhs.green == rhs.green &&
               lhs.blue == rhs.blue;
    }

    template<typename Pixel>
    auto blit(const ImageRegion<Pixel>& src,
              png::image<Pixel>& dst,
              size_t x,
              size_t y,
              Pixel ignore)
    {
        for (size_t y_ = src.region.y; y_ < src.region.y + src.region.height;
             ++y_)
        {
            for (size_t x_ = src.region.x; x_ < src.region.x + src.region.width;
                 ++x_)
            {
                const auto pixel = src.image->get_pixel(x_, y_);
                if (is_equal(pixel, ignore))
                {
                    continue;
                }
                dst.set_pixel(
                    x + (x_ - src.region.x), y + (y_ - src.region.y), pixel);
            }
        }
    }

} // namespace

int main()
{
    auto reader = rucoy::ByteStream("maps/init");
    auto map = rucoy::v110::map_info_from_bytes(reader);
    auto tiles = rucoy::v110::get_map_tiles(map, "maps");

    const auto tilesize = static_cast<size_t>(map.tile_size);

    auto sheet = Tilesheet{ tilesize,
                            tilesize,
                            png::image<png::rgb_pixel>("res/ground.png") };

    png::image<png::rgb_pixel> out(map.map.width * tilesize,
                                   map.map.height * tilesize);

    auto current_layer = 0;
    for (auto x = 0ull; x < map.map.width; x++)
    {
        for (auto y = 0ull; y < map.map.height; y++)
        {
            for (auto& [texture_x, texture_y] :
                 tiles[current_layer]
                      [x + map.map.height * ((map.map.height - 1) - y)]
                          .tiles())
            {
                if (texture_x < 0 || texture_y < 0)
                {
                    continue;
                }

                auto tile = sheet.at(texture_x, texture_y);
                blit(tile, out, x * tilesize, y * tilesize, png::rgb_pixel());
            }
        }
    }

    out.write("outfile.png");
}
