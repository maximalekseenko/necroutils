// Copyright 2026 maxim (necromax) alekseenko

#include "necroutils/iterdir/iterate_dir.h"

#include <optional>
#include <string>
#include <vector>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else
#include <dirent.h>
#endif

std::optional<std::vector<std::string>> IterateDirectory(
    const std::string& path, bool only_files) {
  std::vector<std::string> found_directories;

#if defined(_WIN32) || defined(_WIN64)
  std::string search_path = path + "\\*";
  WIN32_FIND_DATAA file_data;
  HANDLE handle = FindFirstFileA(search_path.c_str(), &file_data);

  if (handle == INVALID_HANDLE_VALUE) return std::nullopt;

  do {
    std::string name = file_data.cFileName;

    if (only_files && (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
      continue;

    if (name == "." || name == "..") continue;

    found_directories.push_back(path + '\\' + name);
  } while (FindNextFileA(handle, &file_data) != 0);

  FindClose(handle);

#else  // POSIX
  DIR* dir = opendir(path.c_str());
  if (!dir) return std::nullopt;

  struct dirent* entry;
  while ((entry = readdir(dir)) != nullptr) {
    std::string name = entry->d_name;

    if (only_files && entry->d_type == DT_DIR) continue;

    if (name == "." || name == "..") continue;

    found_directories.push_back(path + '/' + name);
  }
  closedir(dir);
#endif

  return found_directories;
}
