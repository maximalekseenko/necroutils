// Copyright 2026 maxim (necromax) alekseenko

#include "necroutils/collective_exception/collective_exception.h"

#include <sstream>
#include <string>
#include <vector>

void CollectiveException::SetHeader(const std::string& header) {
  header_ = header;

  if (!header_.empty() && header_.back() != '\n') header_ += '\n';

  what_message_.clear();
}

void CollectiveException::Add(std::exception_ptr eptr) {
  exceptions_.push_back(eptr);
  what_message_.clear();
}

bool CollectiveException::empty() const { return exceptions_.empty(); }

std::size_t CollectiveException::size() const { return exceptions_.size(); }

const std::vector<std::exception_ptr>& CollectiveException::exceptions() const {
  return exceptions_;
}

const char* CollectiveException::what() const noexcept {
  if (what_message_.empty()) {
    what_message_ = BuildMessage();
  }
  return what_message_.c_str();
}

void CollectiveException::ThrowIfCollected() {
  if (!exceptions_.empty()) throw *this;
}

std::string CollectiveException::BuildMessage() const {
  std::ostringstream oss;

  if (!header_.empty()) oss << header_;

  for (std::size_t i = 0; i < exceptions_.size(); ++i) {
    oss << "[" << i << "] ";

    try {
      if (exceptions_[i]) {
        std::rethrow_exception(exceptions_[i]);
      } else {
        oss << "null exception_ptr\n";
      }
    } catch (const std::exception& e) {
      oss << e.what() << "\n";
    } catch (...) {
      oss << "unknown exception.\n";
    }
  }

  return oss.str();
}
