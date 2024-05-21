#pragma once

#include <string>

namespace rucoy
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
}
