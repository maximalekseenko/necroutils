/// Copyright 2026 maxim (necromax) alekseenko

#ifndef INCLUDE_NECROUTILS_LOGGER_LOGSINK_FILE_H_
#define INCLUDE_NECROUTILS_LOGGER_LOGSINK_FILE_H_

#include <fstream>
#include <string>

#include "necroutils/dll.h"
#include "necroutils/logger/logsink.h"

class NECROUTILS_API FileLogSink : public LogSink {
 public:
  static Ptr Create(const std::string& filename) {
    return Ptr(new FileLogSink(filename));
  }

 protected:
  explicit FileLogSink(const std::string& filename) : output_file_{filename} {};

  void OnLog(const std::string& message) override {
    output_file_ << message << std::flush;
  }

 private:
  std::ofstream output_file_;
};

#endif  // INCLUDE_NECROUTILS_LOGGER_LOGSINK_FILE_H_
