/// Copyright 2026 maxim (necromax) alekseenko

#include "necroutils/logger/logsink.h"

#include <iostream>
#include <string>

void ILogSink::AddSourceToBlacklist(std::string source_name) {
  blacklist_src_names_.insert(source_name);
}

void ILogSink::AddLogLevelToBlacklist(LogLevel log_level) {
  blacklist_log_levels_.insert(log_level);
}

void ILogSink::RemoveSourceFromBlacklist(std::string source_name) {
  blacklist_src_names_.erase(source_name);
}

void ILogSink::RemoveLogLevelFromBlacklist(LogLevel log_level) {
  blacklist_log_levels_.erase(log_level);
}

void ILogSink::SetMakeMessageFunction(MakeMessageFn make_message) {
  make_message_ = make_message;
}

void ILogSink::Log(const std::string& message, const LogLevel& log_level,
                   const std::string& source_name) {
  // Check for blacklists.
  if (blacklist_log_levels_.find(log_level) != blacklist_log_levels_.end())
    return;
  if (blacklist_src_names_.find(source_name) != blacklist_src_names_.end())
    return;

  // Log.
  OnLog(make_message_(message, log_level, source_name));
}

std::string ILogSink::MakeMessageDefault(const std::string& message,
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

FileLogSink::FileLogSink(const std::string& filename)
    : output_file_{filename} {}

void FileLogSink::OnLog(const std::string& message) {
  output_file_ << message << std::endl;
}

void ConsoleLogSink::OnLog(const std::string& message) {
  std::cout << message << std::endl;
}
