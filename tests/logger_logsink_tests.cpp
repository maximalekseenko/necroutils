#include <gtest/gtest.h>

#include "necroutils/logger.h"

TEST(LoggerTest, NoDefaultSinkConstructor) {}

TEST(LoggerTest, NoDefaultDerivedSinkConstructors) {}

TEST(LoggerTest, DerivedSinkFactories) {}

TEST(LoggerTest, FileSinkLogging) {}

TEST(LoggerTest, ConsoleSinkLogging) {}

TEST(LoggerTest, SinkLogging) {}

TEST(LoggerTest, SinkCustomMessage) {}

TEST(LoggerTest, SinkBlacklistLoggerAdd) {}

TEST(LoggerTest, SinkBlacklistLoggerRemove) {}

TEST(LoggerTest, SinkBlacklistLogLevelAdd) {}

TEST(LoggerTest, SinkBlacklistLogLevelRemove) {}