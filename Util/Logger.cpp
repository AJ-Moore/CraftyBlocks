#include <Util/Logger.h>
#include <iostream>
#include <cstdarg>

namespace CraftyBlocks
{
	void Logger::Log(LogLevel level, const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		Logger::Log(level, format, args, args);
		va_end(args);
	}

	void Logger::Log(LogLevel level, const char* format, va_list args, ...)
	{
		std::string fString;
		va_list args2;
		va_copy(args2, args);
		fString.resize(vsnprintf(nullptr, 0, format, args2) + 1);
		va_end(args2);
		vsnprintf(fString.data(), fString.size(), format, args);
		FormatLogMessage(level, fString);
	}

	Logger& Logger::Instance()
	{
		static Logger instance;
		return instance;
	}

	void Logger::FormatLogMessage(LogLevel level, const std::string& message)
	{
		std::string logLevelString;
		switch (level)
		{
		case LogLevel::DEBUG:
			logLevelString = "DEBUG";
			break;
		case LogLevel::INFO:
			logLevelString = "INFO";
			break;
		case LogLevel::WARN:
			logLevelString = "WARNING";
			break;
		case LogLevel::ERROR:
			logLevelString = "ERROR";
			break;
		case LogLevel::FATAL:
			logLevelString = "FATAL";
			break;
		}

		std::string formattedMessage = logLevelString + ": " + message;
		std::cout << formattedMessage << std::endl;
	}
}
