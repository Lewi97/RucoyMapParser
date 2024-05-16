#pragma once

#include <fstream>
#include <filesystem>

namespace rucoy
{
	class ByteStream
	{
	public:
		ByteStream() = default;
		explicit ByteStream(const std::filesystem::path& file)
		{
			read_file(file);
		}

		auto read_file(const std::filesystem::path& file) -> void
		{
			_file = std::ifstream(file, std::ios_base::binary);
		}

		/* @returns -1 if invalid */
		auto read() -> int
		{
			if (auto b = _file.get();
				b >= 0 and b <= 255)
				return b;
			return -1;
		}

		auto valid() const -> bool
		{
			return not _file.fail();
		}
	protected:
		std::ifstream _file{};
	};
}

