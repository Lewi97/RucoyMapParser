#pragma once

#include <array>
#include <string>

#include "ByteStream.h"

namespace rucoy::v115
{
	struct MusicRegion
	{
		std::string music{};
		int height{};
		int width{};
		int x{};
		int y{};
		int z{};
	};

	struct InitFile
	{
		int width{};
		int height{};
		int layers{};

		int unknown{};

		std::vector<MusicRegion> music_regions{};
	};

	inline auto parse_init(ByteStream& stream) -> InitFile
	{
		auto file = InitFile{};
		auto music_name_buffer = std::array<char, 64ull>{};

		file.width = stream.read() * 256 + stream.read();
		file.height = stream.read() * 256 + stream.read();
		file.layers = stream.read();

		file.unknown = stream.read() * 256 + stream.read();

		for (auto sprite_layer{ 0 }; sprite_layer < file.layers; sprite_layer++)
		{
			auto music_regions = stream.read();
			for (auto music_region{ 0 }; music_region < music_regions; music_region++)
			{
				auto music_name_size = stream.read();
				stream.fill(music_name_buffer, music_name_size);
				file.music_regions.push_back(
					MusicRegion{
						.music = std::string(music_name_buffer.data(), music_name_size),
						.height = stream.read() * 256 + stream.read(),
						.width = stream.read() * 256 + stream.read(),
						.x = stream.read() * 256 + stream.read(),
						.y = stream.read() * 256 + stream.read(),
						.z = sprite_layer
					}
				);
			}
		}

		return file;
	}
}
