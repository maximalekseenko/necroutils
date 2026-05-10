/// Copyright 2026 maxim (necromax) alekseenko

#ifndef INCLUDE_NECROUTILS_LOGGER_LOGGER_H_
#define INCLUDE_NECROUTILS_LOGGER_LOGGER_H_

#include <memory>
#include <string>
#include <vector>

#include "necroutils/dll.h"
#include "necroutils/logger/loglevel.h"
#include "necroutils/logger/logsink.h"

/// @brief Class for logging.
///
/// When creating a logging system,
/// each module should have it's own instance of logger.
///
/// Main module creates LogSink and passes shared pointer
/// to modules' Loggers.
///
/// When `Log` function is called,
/// it passes log information to logger's sinks.
class NECROUTILS_API Logger {
 public:  // -------------------- TYPE DEFINITIONS --------------------
  /// @brief Type for Sink that is being passed around.
  using SinkPtr = std::shared_ptr<ILogSink>;

 public:  // -------------------- CONSTRUCTORS --------------------
  Logger() = delete;
  explicit Logger(std::string name);

 public:  // -------------------- DESTRUCTORS --------------------
  ~Logger() = default;

 public:
  /// @brief Attaches a sink to this logger.
  /// @param sink A sink to attach.
  void AddSink(SinkPtr sink);

  /// @brief Logs a message.
  /// @param message Message to log.
  /// @param log_level Level of the log.
  void Log(const std::string& message, const LogLevel& log_level);

 private:
  std::string name_;
  std::vector<SinkPtr> sinks_;
};

#endif  // INCLUDE_NECROUTILS_LOGGER_LOGGER_H_
