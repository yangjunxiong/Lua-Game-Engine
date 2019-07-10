#pragma once

#include <iostream>

namespace DirectX
{
	struct XMFLOAT4X4;
}

namespace GameEngine
{
	class OutputStreamHelper final
	{
	public:
		OutputStreamHelper(std::ostream& stream);
		OutputStreamHelper(const OutputStreamHelper&) = delete;
		OutputStreamHelper& operator=(const OutputStreamHelper&) = delete;

		std::ostream& Stream();

		OutputStreamHelper& operator<<(int32_t value);
		OutputStreamHelper& operator<<(int64_t value);
		OutputStreamHelper& operator<<(uint32_t value);
		OutputStreamHelper& operator<<(uint64_t value);
		OutputStreamHelper& operator<<(float value);
		OutputStreamHelper& operator<<(const std::string& value);
		OutputStreamHelper& operator<<(const DirectX::XMFLOAT4X4& value);
		OutputStreamHelper& operator<<(bool value);
		
	private:
		template <typename T>
		void WriteObject(std::ostream& stream, T value);

		std::ostream& mStream;
	};

	class InputStreamHelper final
	{
	public:
		InputStreamHelper(std::istream& stream);
		InputStreamHelper(const InputStreamHelper&) = delete;
		InputStreamHelper& operator=(const InputStreamHelper&) = delete;

		std::istream& Stream();

		InputStreamHelper& operator>>(int32_t& value);
		InputStreamHelper& operator>>(int64_t& value);
		InputStreamHelper& operator>>(uint32_t& value);
		InputStreamHelper& operator>>(uint64_t& value);
		InputStreamHelper& operator>>(float& value);
		InputStreamHelper& operator>>(std::string& value);
		InputStreamHelper& operator>>(DirectX::XMFLOAT4X4& value);
		InputStreamHelper& operator>>(bool& value);
		
	private:
		template <typename T>
		void ReadObject(std::istream& stream, T& value);

		std::istream& mStream;
	};
}