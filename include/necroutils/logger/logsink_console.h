/// Copyright 2026 maxim (necromax) alekseenko

#ifndef INCLUDE_NECROUTILS_LOGGER_LOGSINK_CONSOLE_H_
#define INCLUDE_NECROUTILS_LOGGER_LOGSINK_CONSOLE_H_

#include <iostream>
#include <string>

#include "necroutils/dll.h"
#include "necroutils/logger/logsink.h"

class NECROUTILS_API ConsoleLogSink : public LogSink {
 public:
  static Ptr Create() { return Ptr(new ConsoleLogSink()); }

 protected:
  ConsoleLogSink() = default;

  void OnLog(const std::string& message) override {
    std::cout << message << std::flush;
  }
};

#endif  // INCLUDE_NECROUTILS_LOGGER_LOGSINK_CONSOLE_H_
