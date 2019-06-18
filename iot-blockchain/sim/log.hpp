#include <iostream>

enum LogSymbol
{
	LOG_INPUT = '<',
	LOG_OUTPUT = '>',
	LOG_ADD = '+',
	LOG_INFO = '~',
	LOG_ALERT = '!'
};

void logOutput(LogSymbol symbol, const char *format, ...);
