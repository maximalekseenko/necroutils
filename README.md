# Necroutils.

This is my (necromax's) custom library 
with common solution to problems.

## Logger.

### What Logger is for?

Logger allows different modules to pass logs to each other.

### Main principle.

Each module hosts an instance of [`Logger`](./include/necroutils/logger/logger.h).

When a module needs to log something they would call `Log` function of their `Logger`.

Main module creates an instance of [`LogSink`](./include/necroutils/logger/logsink.h)
(Better stored as `Logger::SinkPtr`).

On initialization, main module passes it's `LogSink` to it's modules and they `AddSink` to their `Logger`s.

### LogSink types.

LogSinks differ by the way they output their logs.

`ConsoleLogSink` would write each log into the console using iostream.

`FileLogSink` would open a file and write a new logs there.

It is possible to create custom `LogSink`s.
In order to do so, your LogSink must derive from `ILogSink` class 
and should override `OnLog` function.

### Additional features.

A `Logger` can have multiple `LogSink`s. On log, each of added `LogSink`s will write a log.
This way, it is possible to output a log to multiple sources. 

`LogSink` can filter incoming logs by names of the loggers and/or levels of the log using 
`AddLogLevelToBlacklist`
`RemoveSourceFromBlacklist`
`RemoveLogLevelFromBlacklist`
`SetMakeMessageFunction`
functions.

The way how `logSink`'s logs look can be changed with `SetMakeMessageFunction` function.
`MakeMessageFn` takes all parameters of the log and returns a string that will be logged.

## Collective Exception.

# Compiling.
CPP_LINT_PATH