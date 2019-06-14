#include <string>
#include <chrono>

#include <stdlib.h>
#include <stdarg.h>

#include "log.hpp"

void logOutput(LogSymbol symbol, const char *format, ...)
{
	va_list args;
	va_start(args, format);

	printf("[%c] ", static_cast<char>(symbol));
	vfprintf(stderr, format, args);

	va_end(args);

	std::cout << std::endl;
}

//std::string printDeviceReceipt(std::string identifier, std::string action)
//{
//	char* output = new char[identifier.length() + action.length() + 16];
//
//	sprintf(output, "Device: %s %s...", identifier.c_str(), action.c_str());
//
//	return output;
//}
