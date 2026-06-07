/// Copyright 2026 maxim (necromax) alekseenko

#include "necroutils/logger/logsink.h"

#include <iostream>
#include <string>

void LogSink::Log(const std::string& message, const LogLevel& log_level,
                  const std::string& source_name) {
  if (blacklist_log_levels_.find(log_level) != blacklist_log_levels_.end())
    return;
  if (blacklist_src_names_.find(source_name) != blacklist_src_names_.end())
    return;

  std::string compiled_message = make_message_(message, log_level, source_name);

  if (!compiled_message.ends_with('\n')) compiled_message.push_back('\n');

  OnLog(make_message_(message, log_level, source_name));
}

std::string LogSink::MakeMessageDefault(const std::string& message,
                                        const LogLevel& log_level,
                                        const std::string&) {
  std::string final_message;

  final_message += "[";
  switch (log_level) {
    case LogLevel::Debug:
      final_message += "DEBUG";
      break;
    case LogLevel::Info:
      final_message += "INFO";
      break;
    case LogLevel::Warning:
      final_message += "WARNING";
      break;
    case LogLevel::Error:
      final_message += "ERROR";
      break;

    default:
      break;
  }
  final_message += "]";
  return final_message + "\t" + message;
}
