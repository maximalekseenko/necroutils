/// Copyright 2026 maxim (necromax) alekseenko

#ifndef INCLUDE_NECROUTILS_LOGGER_LOGSINK_H_
#define INCLUDE_NECROUTILS_LOGGER_LOGSINK_H_

#include <cstdint>
#include <fstream>
#include <functional>
#include <string>
#include <unordered_set>

#include "necroutils/dll.h"
#include "necroutils/logger/loglevel.h"

/// @brief Interface type for LogSinks.
class NECROUTILS_API ILogSink {
 public:
  virtual ~ILogSink() = default;

 public:  // -------------------- TYPE DEFINITIONS --------------------
  /// @brief Type for a function for creating messages.
  using MakeMessageFn = std::function<std::string(
      const std::string&, const LogLevel&, const std::string&)>;

 public:  // -------------------- PUBLIC FUNCTIONS --------------------
  /// @brief Adds source to blacklist.
  ///
  /// Logs from sources in blacklist will be ignored.
  /// @param source_name The name of the source to ignore.
  void AddSourceToBlacklist(std::string source_name);

  /// @brief Adds log level to blacklist.
  ///
  /// Logs of levels that are in blacklist will be ignored.
  /// @param log_level The log level to ignore.
  void AddLogLevelToBlacklist(LogLevel log_level);

  /// @brief Removes source from blacklist.
  /// @param source_name The name of the source to stop ignoreing.
  void RemoveSourceFromBlacklist(std::string source_name);

  /// @brief Removes log level from blacklist.
  /// @param source_name The log level to stop ignoreing.
  void RemoveLogLevelFromBlacklist(LogLevel log_level);

  /// @brief Sets a new function to be used for forming log messages.
  /// @param make_message A new function to use.
  void SetMakeMessageFunction(MakeMessageFn make_message);

 protected:  // -------------------- VIRTUAL FUNCTIONS --------------------
  /// @brief Virtual function for handing logging.
  ///
  /// Passed message is already parsed and filtered.
  /// It already ends with a new line.
  /// @param message
  virtual void OnLog(const std::string& message) = 0;

 private:  // -------------------- FRIEND FUNCTIONS --------------------
  friend class Logger;

  /// @brief Log
  /// @param message
  /// @param log_level
  /// @param source_name
  void Log(const std::string& message, const LogLevel& log_level,
           const std::string& source_name);

 private:  // -------------------- DEFAULT FUNCTIONS --------------------
  static std::string MakeMessageDefault(const std::string& message,
                                        const LogLevel& log_level,
                                        const std::string& source_name);

 private:  // -------------------- PRIVATE MEMBERS --------------------
  MakeMessageFn make_message_{ILogSink::MakeMessageDefault};
  std::unordered_set<std::string> blacklist_src_names_;
  std::unordered_set<LogLevel> blacklist_log_levels_;
};

// +------------------- -------- -------------------+
// -------------------- DERIVEDS --------------------
// +------------------- -------- -------------------+

class NECROUTILS_API FileLogSink : public ILogSink {
 public:
  explicit FileLogSink(const std::string& filename);

 protected:
  void OnLog(const std::string& message) override;

 private:
  std::ofstream output_file_;
};

class NECROUTILS_API ConsoleLogSink : public ILogSink {
 protected:
  void OnLog(const std::string& message) override;
};

#endif  // INCLUDE_NECROUTILS_LOGGER_LOGSINK_H_
