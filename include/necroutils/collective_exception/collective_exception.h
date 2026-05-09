// Copyright 2026 maxim (necromax) alekseenko

#ifndef INCLUDE_NECROUTILS_COLLECTIVE_EXCEPTION_COLLECTIVE_EXCEPTION_H_
#define INCLUDE_NECROUTILS_COLLECTIVE_EXCEPTION_COLLECTIVE_EXCEPTION_H_

#include <stdexcept>
#include <string>
#include <vector>

#include "necroutils/dll.h"

/// @brief An exception for handling
/// cases of multiple exceptions.
class DLL_PUBLIC CollectiveException : public std::exception {
 public:  // -------------------- CONSTRUCTORS --------------------
  CollectiveException() = default;

  explicit CollectiveException(const std::string& header) { SetHeader(header); }

 public:  // -------------------- PUBLIC FUNCTIONS --------------------
  /// @brief Sets header for this exception.
  /// @param header A new header for this exception.
  void SetHeader(const std::string& header);

  /// @brief Collects a new exception.
  /// @param eptr Pointer to an exception to
  void Add(std::exception_ptr eptr);

  /// @brief Checks if no exceptions were collected.
  bool empty() const;

  /// @brief Returns amount of collected exceptions.
  std::size_t size() const;

  /// @brief Allows access to collected exceptions.
  const std::vector<std::exception_ptr>& exceptions() const;

  /// @brief Returns message of this exception.
  const char* what() const noexcept override;

  /// @brief Throws itself if any exception had beed collected.
  ///
  /// Equivalent of
  /// ```
  /// if (!col_exc.empty()) throw col_exc;
  /// ```
  /// @throw CollectiveException
  void ThrowIfCollected();

 private:  // -------------------- HELPER FUNCTIONS --------------------
  std::string BuildMessage() const;

 private:  // -------------------- PRIVATE MEMBERS --------------------
  std::vector<std::exception_ptr> exceptions_;
  std::string header_;
  mutable std::string what_message_;
};

#endif  // INCLUDE_NECROUTILS_COLLECTIVE_EXCEPTION_COLLECTIVE_EXCEPTION_H_
