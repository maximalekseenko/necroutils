/// Copyright 2026 maxim (necromax) alekseenko

#include "necroutils/logger/logger.h"

#include <string>

#include "necroutils/logger/logsink.h"

Logger::Logger(std::string name) : name_{name} {}

void Logger::AddSink(LogSink::Ptr sink) { sinks_.push_back(sink); }

void Logger::Log(const std::string& message, const LogLevel& log_level) {
  for (const auto& sink : sinks_) sink->Log(message, log_level, name_);
}
