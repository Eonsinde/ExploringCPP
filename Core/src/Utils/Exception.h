#pragma once

#include <stdexcept>


namespace {
	class CustomException : public std::exception
	{
	private:
		std::string message;

	public:
		CustomException(std::string text): message{ text } {}

		const char* what() const noexcept override
		{
			return message.c_str();
		}
	};
}
