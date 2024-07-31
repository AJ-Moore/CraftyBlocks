#pragma once

#include <string>
#include <cstdarg>
#include <stdexcept>

namespace CraftyBlocks
{
	class Logger
	{
	public:
		enum LogLevel
		{
			VERBOSE,
			DEBUG,
			INFO,
			WARN,
			ERROR,
			FATAL
		};

		static void Log(LogLevel level, const char* format, ...);
		static void SetLogLevel(LogLevel level) { Instance().m_logLevel = level; }
		static Logger& Instance();

	private:
		void FormatLogMessage(LogLevel level, const std::string& message);
		LogLevel m_logLevel = LogLevel::DEBUG;
	};

	inline void Log(Logger::LogLevel level, const char* format, ...) {
		va_list args;
		va_start(args, format);
		Logger::Log(level, format, args);
		va_end(args);
	}

	class GLException : public std::runtime_error {
	public:
		explicit GLException(const std::string& message)
			: std::runtime_error(message) {}
	};

	inline bool LogGraphicsErrors() {
#ifdef RENDERER_OPENGL
		int code = glGetError();

		if (code == 0)
		{
			return code;
		}

		Log(Logger::ERROR, "GL Error [%d]", code);

#if not defined(IGNORE_GL_EXCEPTIONS)
		throw(GLException("GL Exception"));
#endif
#elif RENDERER_VULKAN
#elif RENDERER_PS5
#elif RENDERER_DX12
#endif
		return false;
	}

}
