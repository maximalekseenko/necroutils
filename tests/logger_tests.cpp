#include <gtest/gtest.h>

#include "necroutils/logger.h"

class TestSink : public LogSink {
 public:
  std::string lastMessage;

  void OnLog(const std::string& msg) override { lastMessage = msg; }
};

// ----------------------
// LogLevel string tests
// ----------------------

// ----------------------
// Logger output tests
// ----------------------
TEST(LoggerTest, NoSinkProducesNoOutput) {
  Logger logger("test");
  ::testing::internal::CaptureStdout();
  logger.Log("lorem ipsum", LogLevel::Debug);
  std::string output = ::testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "");  // No sink → nothing printed
}

TEST(LoggerTest, DefaultFormatOutputsCorrectly) {
  Logger logger("test");
  auto sink = ConsoleLogSink::Create();
  logger.AddSink(sink);

  ::testing::internal::CaptureStdout();
  logger.Log("lorem ipsum", LogLevel::Debug);
  std::string output = ::testing::internal::GetCapturedStdout();

  EXPECT_EQ(output, "[DEBUG]\tlorem ipsum\n");
}

TEST(LoggerTest, CustomMessageFunctionWorks) {
  Logger logger("test");
  auto sink = std::make_shared<TestSink>();
  logger.AddSink(sink);

  sink->SetMakeMessageFunction(
      [](const std::string& msg, const LogLevel&, const std::string& src) {
        return src + ": " + msg + "\n";
      });

  logger.Log("lorem ipsum", LogLevel::Debug);
  EXPECT_EQ(sink->lastMessage, "test: lorem ipsum\n");
}