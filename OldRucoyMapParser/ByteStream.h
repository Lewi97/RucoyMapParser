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

		/* 
		* @param count: bytes to read, -1 will fill up to size
		* @returns Written bytes 
		*/
		auto fill(std::ranges::range auto& range, int count = -1) -> int
		{
			auto bytes_to_read = count < 0 ? std::size(range) : count;
			
			for (auto& byte : range)
			{
				byte = read();
				bytes_to_read--;
				if (bytes_to_read == 0) 
					break;
			}

			return bytes_to_read;
		}
	protected:
		std::ifstream _file{};
	};
}

