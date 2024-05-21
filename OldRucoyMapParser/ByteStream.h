#pragma once

#include <fstream>
#include <filesystem>

namespace rucoy
{
	template<typename T>
	class Stream
	{
	public:
		auto read_int(bool little_endian = true) -> int
		{
			auto a = read();
			auto b = read();
			auto c = read();
			auto d = read();
			if (little_endian)
			{
				return a << 24 | b << 16 | c << 8 | d;
			}
			else
			{
				return d << 24 | c << 16 | b << 8 | a;
			}
		}

		auto read_short(bool little_endian = true) -> short
		{
			auto a = read();
			auto b = read();
			if (little_endian)
			{
				return a << 8 | b;
			}
			else
			{
				return b << 8 | a;
			}
		}

		auto valid() const -> bool
		{
			return static_cast<T*>(this)->valid();
		}

		/*
		* @param count: bytes to read, -1 will fill up to size
		* @returns Written bytes
		*/
		auto fill(std::ranges::range auto& range, int count = -1) -> int
		{
			auto bytes_to_read = count < 0 ? std::size(range) : count;
			auto bytes_read{ 0 };

			for (auto& byte : range)
			{
				byte = read();
				bytes_read++;
				if (bytes_read == bytes_to_read)
					break;
			}

			return bytes_read;
		}
	protected:
		auto read() -> int
		{
			return static_cast<T*>(this)->read();
		}
	};

	class ByteStream : public Stream<ByteStream>
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

	class ByteStreamVector : public Stream<ByteStreamVector>
	{
	public:
		ByteStreamVector() = default;
		explicit ByteStreamVector(std::vector<char> file)
		{
			take(file);
		}
		
		ByteStreamVector(const ByteStreamVector& other)
		{
			copy(other);
		}

		auto operator=(const ByteStreamVector& other)
		{
			copy(other);
			return *this;
		}

		ByteStreamVector(ByteStreamVector&& other) noexcept
		{
			_data = std::exchange(other._data, std::vector<char>());
			_position = _data.begin();
		}

		auto operator=(ByteStreamVector&& other) noexcept
		{
			_data = std::exchange(other._data, std::vector<char>());
			_position = _data.begin();
			return *this;
		}

		auto copy(const ByteStreamVector& other) -> void
		{
			take(other._data);
		}

		auto take(std::vector<char> data) -> void
		{
			_data = std::move(data);
			_position = _data.begin();
		}

		/* @returns -1 if invalid */
		auto read() -> int
		{
			if (not valid()) return -1;
			auto byte = *_position;
			_position++;
			return static_cast<int>(byte);
		}

		auto position(size_t where) -> void
		{
			if (where >= _data.size())
			{
				_position = _data.end();
			}
			_position = _data.begin() + where;
		}

		auto valid() const -> bool
		{
			return _position != _data.end();
		}

		auto bytes_read() -> int
		{
			return std::distance(_data.begin(), _position);
		}
	protected:
		std::vector<char> _data{};
		std::vector<char>::iterator _position{};
	};

	template<typename Stream_>
	struct BitReader
	{
		int bits_left{};
		char current_byte{};
		Stream_ bytes{};
		char HpzT1UQ5tRgVx7DN7xc = 0;
		int bWRfWkdQTJX4NfxtuEU = 8;

		BitReader() = default;
		explicit BitReader(Stream_ stream)
			: bytes(std::move(stream))
		{}

		int parse(int i) {
			int i2 = 0;
			for (int i3 = 0; i3 < i; i3++) {
				if (bits_left == 0) {
					current_byte = bytes.read();
					bits_left = 8;
				}
				bits_left--;
				i2 += ((((current_byte >> bits_left) << 31) >> 31) * (-1)) << ((i - i3) - 1);
			}
			return i2;
		}
	};

	/*
	* Write a type erased buffer stream object so we dont need to use seperate types for these two
	*/

	using BitReaderFileStream = BitReader<ByteStream>;
	using BitReaderVector = BitReader<ByteStreamVector>;
}

