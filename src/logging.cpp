#include "logging.hpp"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <raylib.h>
#include <spdlog/common.h>
#include <spdlog/sinks/dist_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/stdout_sinks.h>

#ifdef PLATFORM_DESKTOP
#  if defined(_WIN32) || defined(_WIN64)
#    include <io.h>
#    include <windows.h>
#  elif defined(__APPLE__) || defined(__unix__) || defined(__unix)
#    include <unistd.h>
#  endif
#endif

namespace civitas::logging {
std::shared_ptr<spdlog::sinks::dist_sink_mt> sink = nullptr;
}

static std::string rlcat;

void civitas::logging::callback(int level, const char *text, va_list args) {
  std::string logger = "raylib";
  std::string buffer(256, 0);
  std::vsnprintf(buffer.data(), buffer.size(), text, args);

  std::size_t idx = buffer.find(':');
  if (buffer[0] == ' ') {
    logger += "::" + rlcat;
  } else if (idx != std::string::npos) {
    rlcat.resize(idx);
    std::transform(buffer.begin(), buffer.begin() + static_cast<long>(idx),
                   rlcat.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    logger += "::" + rlcat;
    buffer = buffer.substr(idx + 2);
  }
  switch (level) {
  case LOG_TRACE:
#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_TRACE
    get(logger)->log(spdlog::level::trace, buffer);
#endif
    break;
  case LOG_DEBUG:
#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_DEBUG
    get(logger)->log(spdlog::level::debug, buffer);
#endif
    break;
  case LOG_INFO:
#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_INFO
    get(logger)->log(spdlog::level::info, buffer);
#endif
    break;
  case LOG_WARNING:
#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_WARN
    get(logger)->log(spdlog::level::warn, buffer);
#endif
    break;
  case LOG_ERROR:
#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_ERROR
    get(logger)->log(spdlog::level::err, buffer);
#endif
    break;
  case LOG_FATAL:
#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_CRITICAL
    get(logger)->log(spdlog::level::critical, buffer);
#endif
    break;
  default:
#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_INFO
    get(logger)->log(spdlog::level::info, buffer);
#endif
    break;
  }
}

bool civitas::logging::setup() {
  try {
    logging::sink = std::make_shared<spdlog::sinks::dist_sink_mt>();
    logging::sink->set_pattern(
        "[%Y-%m-%d %H:%M:%S.%e] [%n] [%^%l%$] [%s:%#] <%t> %v");

#ifndef NDEBUG
    {
#  ifdef PLATFORM_DESKTOP
#    if defined(_WIN32) || defined(_WIN64)
      const bool use_color = _isatty(_fileno(stdout));
#    elif defined(__APPLE__) || defined(__unix__) || defined(__unix)
      const bool use_color = ::isatty(fileno(stdout)) != 0;
#    else
      const bool use_color = false;
#    endif
#  else
      const bool use_color = false;
#  endif

      std::shared_ptr<spdlog::sinks::sink> new_sink = nullptr;
      if (use_color)
        new_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
      else
        new_sink = std::make_shared<spdlog::sinks::stdout_sink_mt>();

      new_sink->set_level(spdlog::level::debug);
      if (use_color)
        new_sink->set_pattern(
            "[\033[33m%H:%M:%S.%e\033[0m] [\033[1m%15!n\033[0m] [%^%=7l%$] "
            "[\033[35m%s\033[0m:\033[36m%#\033[0m:\033[90m%!\033[0m] %v");
      else
        new_sink->set_pattern("[%H:%M:%S.%e] [%n] [%^%l%$] [%s:%#:%!] <%t> %v");
      logging::sink->add_sink(new_sink);
    }
#endif

    {
      std::shared_ptr<spdlog::sinks::sink> new_sink =
          std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
              "logs/tinxi.log", 5 * 1024 * 1024, 5);
      new_sink->set_level(spdlog::level::trace);
      new_sink->set_pattern(
          "[%Y-%m-%d %H:%M:%S.%e] [%n] [%^%l%$] [%s:%#] <%t> %v");
      logging::sink->add_sink(new_sink);
    }

  } catch (const spdlog::spdlog_ex &err) {
    std::cerr << "Log initialization failed: " << err.what() << std::endl;
    return false;
  }
  return true;
}
