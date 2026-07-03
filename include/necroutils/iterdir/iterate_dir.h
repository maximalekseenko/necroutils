// Copyright 2026 maxim (necromax) alekseenko

#ifndef INCLUDE_NECROUTILS_ITERDIR_ITERATE_DIR_H_
#define INCLUDE_NECROUTILS_ITERDIR_ITERATE_DIR_H_

#include <optional>
#include <string>
#include <vector>

std::optional<std::vector<std::string>> IterateDirectory(
    const std::string& path, bool only_files = true);

#endif  // INCLUDE_NECROUTILS_ITERDIR_ITERATE_DIR_H_
