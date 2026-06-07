/// Copyright 2026 maxim (necromax) alekseenko

#ifndef INCLUDE_NECROUTILS_LOGGER_LOGGER_H_
#define INCLUDE_NECROUTILS_LOGGER_LOGGER_H_

#include <memory>
#include <string>
#include <vector>

#include "necroutils/dll.h"
#include "necroutils/logger/loglevel.h"
#include "necroutils/logger/logsink.h"

class NECROUTILS_API Logger {
 public:
  Logger() = delete;
  explicit Logger(std::string name);

  ~Logger() = default;

  void AddSink(LogSink::Ptr sink);

  void Log(const std::string& message, const LogLevel& log_level);

 private:
  std::string name_;
  std::vector<LogSink::Ptr> sinks_;
};

#endif  // INCLUDE_NECROUTILS_LOGGER_LOGGER_H_
