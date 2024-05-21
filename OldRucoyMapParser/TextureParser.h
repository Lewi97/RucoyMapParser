#pragma once

#include "ByteStream.h"
#include "Metrics.h"

namespace rucoy
{
	namespace v115
	{
		auto load_textures(auto load_texture, auto add_region, ByteStream& stream) -> void
		{
			auto size_texture = stream.read_int(true);
			auto image_data = std::vector<char>(size_texture);
			stream.fill(image_data, size_texture);
			load_texture(image_data);
			auto textures = stream.read_int();
			for (auto i{ 0 }; i < textures; i++)
			{
				auto bytes_read = stream.fill(image_data, stream.read());
				auto texture_x = stream.read_short();
				auto texture_y = stream.read_short();
				auto texture_width = stream.read_short();
				auto texture_height = stream.read_short();
				add_region(std::string_view(image_data.data(), bytes_read), texture_x, texture_y, texture_width, texture_height);
			}
		}
	}

	namespace v118
	{
		/*
		* @param load_texture_: function(char[], TextureRegion)
		* @param stream: bytestream of file c8
		*/
		auto load_texture(auto load_texture_, ByteStream& stream) -> void
		{
			auto size_texture = stream.read_int(true);
			auto image_data = std::vector<char>(size_texture);
			stream.fill(image_data, size_texture);
			auto position = TextureCoord{};
			auto size = Bounds<int>{};
			position.x = stream.read_short();
			position.y = stream.read_short();
			size.width = stream.read_short();
			size.height = stream.read_short();

			load_texture_(image_data, TextureRegion{ position, size });
		}

		/*
		* @param add_tile: function(name, x, y, w, h)
		* @param stream: stream of file c3
		*/
		auto load_tiles(auto add_tile, ByteStream& stream) -> void
		{
			auto name_data = std::vector<char>(64);
			auto tiles = stream.read_int();
			for (auto i{ 0 }; i < tiles; i++)
			{
				auto bytes_read = stream.fill(name_data, stream.read());
				auto texture_x = stream.read_short();
				auto texture_y = stream.read_short();
				auto texture_width = stream.read_short();
				auto texture_height = stream.read_short();
				add_tile(std::string_view(name_data.data(), bytes_read), texture_x, texture_y, texture_width, texture_height);
			}
		}
	}
}
