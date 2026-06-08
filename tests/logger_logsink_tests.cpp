#include <gtest/gtest.h>

#include <filesystem>
#include <string>
#include <vector>

#include "necroutils/logger.h"

class TestLogsink : public LogSink {
 public:
  TestLogsink() = default;
  std::vector<std::string> messages;
  void OnLog(const std::string& message) override {
    messages.push_back(message);
  }
};

std::string FixMessageNL(std::string message) {
  if (!message.ends_with('\n')) message.push_back('\n');
  return message;
}

TEST(LoggerSinkTest, LogSinksAreNotDefaultConstructible) {
  EXPECT_FALSE(std::is_default_constructible<LogSink>::value);
  EXPECT_FALSE(std::is_default_constructible<FileLogSink>::value);
  EXPECT_FALSE(std::is_default_constructible<ConsoleLogSink>::value);
}

TEST(LoggerSinkTest, FileLogSinkFactoryCreatesUsableSink) {
  std::string test_filename = "file_sink_out";

  if (std::filesystem::exists(test_filename)) {
    std::filesystem::remove(test_filename);
  }

  auto sink = FileLogSink::Create(test_filename);
  ASSERT_NE(sink, nullptr);
  EXPECT_NO_THROW(sink->Log("msg", LogLevel::Info, "test"));

  ASSERT_TRUE(std::filesystem::exists(test_filename));
  std::filesystem::remove(test_filename);
}

TEST(LoggerSinkTest, ConsoleLogSinkFactoryCreatesUsableSink) {
  auto sink = ConsoleLogSink::Create();
  ASSERT_NE(sink, nullptr);
  EXPECT_NO_THROW(sink->Log("test", LogLevel::Info, "factory_test"));
}

TEST(LoggerSinkTest, LogSinkDefaultMessageCompiles) {
  EXPECT_STREQ(
      LogSink::MakeMessageDefaultFn("msg", LogLevel::Info, "test").c_str(),
      "[INFO]\tmsg");
}

TEST(LoggerSinkTest, FileSinkLogHasOutputOneLine) {
  std::string test_filename = "file_sink_out";

  if (std::filesystem::exists(test_filename)) {
    std::filesystem::remove(test_filename);
  }

  auto sink = FileLogSink::Create(test_filename);
  ASSERT_NE(sink, nullptr);

  sink->Log("msg", LogLevel::Info, "test");

  ASSERT_TRUE(std::filesystem::exists(test_filename));

  std::ifstream log_file(test_filename);
  ASSERT_TRUE(log_file.is_open());

  std::string line;
  std::getline(log_file, line);
  EXPECT_STREQ(
      line.c_str(),
      LogSink::MakeMessageDefaultFn("msg", LogLevel::Info, "test").c_str());
  log_file.close();

  std::filesystem::remove(test_filename);
}

TEST(LoggerSinkTest, FileSinkLogHasOutputTwoLines) {
  std::string test_filename = "file_sink_out";

  if (std::filesystem::exists(test_filename)) {
    std::filesystem::remove(test_filename);
  }

  auto sink = FileLogSink::Create(test_filename);
  ASSERT_NE(sink, nullptr);

  sink->Log("msg1", LogLevel::Info, "test");
  sink->Log("msg2", LogLevel::Error, "testy");

  ASSERT_TRUE(std::filesystem::exists(test_filename));

  std::ifstream log_file(test_filename);
  ASSERT_TRUE(log_file.is_open());

  std::string line;

  EXPECT_TRUE(std::getline(log_file, line));
  EXPECT_STREQ(
      line.c_str(),
      LogSink::MakeMessageDefaultFn("msg1", LogLevel::Info, "test").c_str());

  EXPECT_TRUE(std::getline(log_file, line));
  EXPECT_STREQ(
      line.c_str(),
      LogSink::MakeMessageDefaultFn("msg2", LogLevel::Error, "testy").c_str());

  EXPECT_FALSE(std::getline(log_file, line));

  log_file.close();

  std::filesystem::remove(test_filename);
}

TEST(LoggerSinkTest, ConsoleSinkLogHasOutputOneLine) {
  auto sink = ConsoleLogSink::Create();

  sink->SetMakeMessageFunction([](const std::string& msg, const LogLevel&,
                                  const std::string&) { return msg; });

  ::testing::internal::CaptureStdout();
  sink->Log("msg", LogLevel::Info, "test");

  EXPECT_STREQ(::testing::internal::GetCapturedStdout().c_str(), "msg\n");
}

TEST(LoggerSinkTest, ConsoleSinkLogHasOutputTwoLines) {
  auto sink = ConsoleLogSink::Create();

  sink->SetMakeMessageFunction([](const std::string& msg, const LogLevel&,
                                  const std::string&) { return msg; });

  ::testing::internal::CaptureStdout();
  sink->Log("msg1", LogLevel::Info, "test");
  sink->Log("msg2", LogLevel::Info, "test");

  EXPECT_STREQ(::testing::internal::GetCapturedStdout().c_str(),
               "msg1\nmsg2\n");
}

TEST(LoggerSinkTest, DerivedSinkDefaultsToLogSinkDefaultMessageFn) {
  TestLogsink sink1;
  TestLogsink sink2;

  sink2.SetMakeMessageFunction(LogSink::MakeMessageDefaultFn);

  sink1.Log("msg", LogLevel::Info, "test");
  sink2.Log("msg", LogLevel::Info, "test");
  EXPECT_STREQ(sink1.messages.back().c_str(), sink1.messages.back().c_str());
}

TEST(LoggerSinkTest, SinkCustomMessageChangesOutput) {
  TestLogsink sink;

  sink.Log("msg", LogLevel::Info, "test");
  EXPECT_STREQ(
      sink.messages.back().c_str(),
      FixMessageNL(LogSink::MakeMessageDefaultFn("msg", LogLevel::Info, "test"))
          .c_str());

  sink.SetMakeMessageFunction([](const std::string& msg, const LogLevel&,
                                 const std::string& src) { return src + msg; });

  sink.Log("msg", LogLevel::Info, "test");
  EXPECT_STREQ(sink.messages.back().c_str(), "testmsg\n");
}

TEST(LoggerSinkTest, SinkLogAddsNewLineIfMessageHasNone) {
  TestLogsink sink;

  sink.SetMakeMessageFunction([](const std::string& msg, const LogLevel&,
                                 const std::string&) { return msg; });

  sink.Log("msg", LogLevel::Info, "test");
  EXPECT_STREQ(sink.messages.back().c_str(), "msg\n");
}

TEST(LoggerSinkTest, SinkLogDontAddNewLineIfMessageHasOne) {
  TestLogsink sink;

  sink.SetMakeMessageFunction([](const std::string& msg, const LogLevel&,
                                 const std::string&) { return msg; });

  sink.Log("msg\n", LogLevel::Info, "test");
  EXPECT_STREQ(sink.messages.back().c_str(), "msg\n");
}

TEST(LoggerSinkTest, SinkBlacklistSourceAdd) {
  TestLogsink sink;

  sink.SetMakeMessageFunction([](const std::string& msg, const LogLevel&,
                                 const std::string&) { return msg; });

  sink.Log("test1 msg1 info", LogLevel::Info, "test1");
  EXPECT_STREQ(sink.messages.back().c_str(), "test1 msg1 info\n");
  sink.Log("test1 msg1 error", LogLevel::Error, "test1");
  EXPECT_STREQ(sink.messages.back().c_str(), "test1 msg1 error\n");
  sink.Log("test2 msg1 info", LogLevel::Info, "test2");
  EXPECT_STREQ(sink.messages.back().c_str(), "test2 msg1 info\n");
  sink.Log("test2 msg1 error", LogLevel::Error, "test2");
  EXPECT_STREQ(sink.messages.back().c_str(), "test2 msg1 error\n");

  sink.AddSourceToBlacklist("test1");

  sink.Log("test1 msg2 info", LogLevel::Info, "test1");
  EXPECT_STREQ(sink.messages.back().c_str(), "test2 msg1 error\n");
  sink.Log("test1 msg2 error", LogLevel::Error, "test1");
  EXPECT_STREQ(sink.messages.back().c_str(), "test2 msg1 error\n");
  sink.Log("test2 msg2 info", LogLevel::Info, "test2");
  EXPECT_STREQ(sink.messages.back().c_str(), "test2 msg2 info\n");
  sink.Log("test2 msg2 error", LogLevel::Error, "test2");
  EXPECT_STREQ(sink.messages.back().c_str(), "test2 msg2 error\n");
}

TEST(LoggerSinkTest, SinkBlacklistSourceRemove) {
  TestLogsink sink;

  sink.SetMakeMessageFunction([](const std::string& msg, const LogLevel&,
                                 const std::string&) { return msg; });

  sink.Log("test1 msg1 info", LogLevel::Info, "test1");
  EXPECT_STREQ(sink.messages.back().c_str(), "test1 msg1 info\n");
  sink.Log("test1 msg1 error", LogLevel::Error, "test1");
  EXPECT_STREQ(sink.messages.back().c_str(), "test1 msg1 error\n");
  sink.Log("test2 msg1 info", LogLevel::Info, "test2");
  EXPECT_STREQ(sink.messages.back().c_str(), "test2 msg1 info\n");
  sink.Log("test2 msg1 error", LogLevel::Error, "test2");
  EXPECT_STREQ(sink.messages.back().c_str(), "test2 msg1 error\n");

  sink.AddSourceToBlacklist("test1");

  sink.Log("test1 msg2 info", LogLevel::Info, "test1");
  EXPECT_STREQ(sink.messages.back().c_str(), "test2 msg1 error\n");
  sink.Log("test1 msg2 error", LogLevel::Error, "test1");
  EXPECT_STREQ(sink.messages.back().c_str(), "test2 msg1 error\n");
  sink.Log("test2 msg2 info", LogLevel::Info, "test2");
  EXPECT_STREQ(sink.messages.back().c_str(), "test2 msg2 info\n");
  sink.Log("test2 msg2 error", LogLevel::Error, "test2");
  EXPECT_STREQ(sink.messages.back().c_str(), "test2 msg2 error\n");

  sink.RemoveSourceFromBlacklist("test1");

  sink.Log("test1 msg3 info", LogLevel::Info, "test1");
  EXPECT_STREQ(sink.messages.back().c_str(), "test1 msg3 info\n");
  sink.Log("test1 msg3 error", LogLevel::Error, "test1");
  EXPECT_STREQ(sink.messages.back().c_str(), "test1 msg3 error\n");
  sink.Log("test2 msg3 info", LogLevel::Info, "test2");
  EXPECT_STREQ(sink.messages.back().c_str(), "test2 msg3 info\n");
  sink.Log("test2 msg3 error", LogLevel::Error, "test2");
  EXPECT_STREQ(sink.messages.back().c_str(), "test2 msg3 error\n");
}

TEST(LoggerSinkTest, SinkBlacklistLogLevelAdd) {
  TestLogsink sink;

  sink.SetMakeMessageFunction([](const std::string& msg, const LogLevel&,
                                 const std::string&) { return msg; });

  sink.Log("test1 msg1 info", LogLevel::Info, "test1");
  EXPECT_STREQ(sink.messages.back().c_str(), "test1 msg1 info\n");
  sink.Log("test1 msg1 error", LogLevel::Error, "test1");
  EXPECT_STREQ(sink.messages.back().c_str(), "test1 msg1 error\n");
  sink.Log("test2 msg1 info", LogLevel::Info, "test2");
  EXPECT_STREQ(sink.messages.back().c_str(), "test2 msg1 info\n");
  sink.Log("test2 msg1 error", LogLevel::Error, "test2");
  EXPECT_STREQ(sink.messages.back().c_str(), "test2 msg1 error\n");

  sink.AddLogLevelToBlacklist(LogLevel::Info);

  sink.Log("test1 msg2 info", LogLevel::Info, "test1");
  EXPECT_STREQ(sink.messages.back().c_str(), "test2 msg1 error\n");
  sink.Log("test1 msg2 error", LogLevel::Error, "test1");
  EXPECT_STREQ(sink.messages.back().c_str(), "test1 msg2 error\n");
  sink.Log("test2 msg2 info", LogLevel::Info, "test2");
  EXPECT_STREQ(sink.messages.back().c_str(), "test1 msg2 error\n");
  sink.Log("test2 msg2 error", LogLevel::Error, "test2");
  EXPECT_STREQ(sink.messages.back().c_str(), "test2 msg2 error\n");
}

TEST(LoggerSinkTest, SinkBlacklistLogLevelRemove) {
  TestLogsink sink;

  sink.SetMakeMessageFunction([](const std::string& msg, const LogLevel&,
                                 const std::string&) { return msg; });

  sink.Log("test1 msg1 info", LogLevel::Info, "test1");
  EXPECT_STREQ(sink.messages.back().c_str(), "test1 msg1 info\n");
  sink.Log("test1 msg1 error", LogLevel::Error, "test1");
  EXPECT_STREQ(sink.messages.back().c_str(), "test1 msg1 error\n");
  sink.Log("test2 msg1 info", LogLevel::Info, "test2");
  EXPECT_STREQ(sink.messages.back().c_str(), "test2 msg1 info\n");
  sink.Log("test2 msg1 error", LogLevel::Error, "test2");
  EXPECT_STREQ(sink.messages.back().c_str(), "test2 msg1 error\n");

  sink.AddLogLevelToBlacklist(LogLevel::Info);

  sink.Log("test1 msg2 info", LogLevel::Info, "test1");
  EXPECT_STREQ(sink.messages.back().c_str(), "test2 msg1 error\n");
  sink.Log("test1 msg2 error", LogLevel::Error, "test1");
  EXPECT_STREQ(sink.messages.back().c_str(), "test1 msg2 error\n");
  sink.Log("test2 msg2 info", LogLevel::Info, "test2");
  EXPECT_STREQ(sink.messages.back().c_str(), "test1 msg2 error\n");
  sink.Log("test2 msg2 error", LogLevel::Error, "test2");
  EXPECT_STREQ(sink.messages.back().c_str(), "test2 msg2 error\n");

  sink.RemoveLogLevelFromBlacklist(LogLevel::Info);

  sink.Log("test1 msg3 info", LogLevel::Info, "test1");
  EXPECT_STREQ(sink.messages.back().c_str(), "test1 msg3 info\n");
  sink.Log("test1 msg3 error", LogLevel::Error, "test1");
  EXPECT_STREQ(sink.messages.back().c_str(), "test1 msg3 error\n");
  sink.Log("test2 msg3 info", LogLevel::Info, "test2");
  EXPECT_STREQ(sink.messages.back().c_str(), "test2 msg3 info\n");
  sink.Log("test2 msg3 error", LogLevel::Error, "test2");
  EXPECT_STREQ(sink.messages.back().c_str(), "test2 msg3 error\n");
}