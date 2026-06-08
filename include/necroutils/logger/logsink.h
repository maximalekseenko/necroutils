/// Copyright 2026 maxim (necromax) alekseenko

#ifndef INCLUDE_NECROUTILS_LOGGER_LOGSINK_H_
#define INCLUDE_NECROUTILS_LOGGER_LOGSINK_H_

#include <functional>
#include <memory>
#include <string>
#include <unordered_set>

#include "necroutils/dll.h"
#include "necroutils/logger/loglevel.h"

class NECROUTILS_API LogSink {
 public:
  using Ptr = std::shared_ptr<LogSink>;

  using MakeMessageFn = std::function<std::string(
      const std::string&, const LogLevel&, const std::string&)>;

  LogSink(LogSink&& other) = delete;
  LogSink& operator=(LogSink&& other) = delete;

  LogSink(const LogSink&) = delete;
  LogSink& operator=(const LogSink&) = delete;

  virtual ~LogSink() = default;

  void AddSourceToBlacklist(std::string source_name) {
    blacklist_src_names_.insert(source_name);
  }

  void AddLogLevelToBlacklist(LogLevel log_level) {
    blacklist_log_levels_.insert(log_level);
  }

  void RemoveSourceFromBlacklist(std::string source_name) {
    blacklist_src_names_.erase(source_name);
  }

  void RemoveLogLevelFromBlacklist(LogLevel log_level) {
    blacklist_log_levels_.erase(log_level);
  }

  void SetMakeMessageFunction(MakeMessageFn make_message) {
    make_message_ = make_message;
  }

  void Log(const std::string& message, const LogLevel& log_level,
           const std::string& source_name);

  static std::string MakeMessageDefaultFn(const std::string& message,
                                          const LogLevel& log_level,
                                          const std::string& source_name);

 protected:
  LogSink() = default;

  /// @brief Called when Log is happening.
  /// @param message Message compiled with make_message_.
  /// Guaranteed to end with a new line.
  virtual void OnLog(const std::string& message) = 0;

 private:
  MakeMessageFn make_message_{LogSink::MakeMessageDefaultFn};

  std::unordered_set<std::string> blacklist_src_names_;
  std::unordered_set<LogLevel> blacklist_log_levels_;
};

#endif  // INCLUDE_NECROUTILS_LOGGER_LOGSINK_H_
