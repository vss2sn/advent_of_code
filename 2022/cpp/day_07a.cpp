#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

struct File {
  std::string name;
  long long size;
};

struct Directory {
  std::string name;
  std::vector<std::unique_ptr<Directory>> dirs;
  std::vector<std::unique_ptr<File>> files;
  Directory * parent;
  long long size;
};

void traverse(Directory * main_dir, int level) {
  for (int i = 0; i < level; i++) {
    std::cout << "  ";
  }
  std::cout << "- " << main_dir->name << "(dir)" << '\n';
  for (const auto& dir : main_dir->dirs) {
    traverse(dir.get(), level + 1);
  }
  for (const auto& file : main_dir->files) {
    for (int i = 0; i < level+1; i++) {
      std::cout << "  ";
    }
    std::cout << "- " << file->name << "(file, size=" << file->size << ')' << '\n';
  }
}

long long update_sizes(Directory * main_dir) {
  for (const auto& dir : main_dir->dirs) {
    main_dir->size += update_sizes(dir.get());
  }
  for (const auto& file : main_dir->files) {
    main_dir->size += file->size;
  }
  return main_dir->size;
}

std::tuple<long long, long long> count_dirs_lte_threshold(Directory * main_dir, long long threshold) {
  long long sum = 0;
  long long count = 0;
  for (const auto& dir : main_dir->dirs) {
    if (dir->size <= threshold) {
      std::cout << dir->name << '\n';
      count++;
      sum += dir->size;
    }
    const auto ans = count_dirs_lte_threshold(dir.get(), threshold);
    count += std::get<0>(ans);
    sum += std::get<1>(ans);
  }
  return {count, sum};
}

int main(int argc, char * argv[]) {
  std::string input = "../input/day_07_input";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);

  std::unique_ptr<Directory> file_system = std::make_unique<Directory>();
  file_system->name = '/';
  file_system->parent = file_system.get();
  auto current = file_system.get();
  bool skip = false;
  while(skip || std::getline(file, line)) {
    skip = false;
    if (line[0] == '$') {
      const auto i = line.find(' ', 2);
      const auto cmd = line.substr(2, i - 2);
      if (cmd == "cd") {
        const auto next = line.substr(i+1, line.size() - 1 - i);
        if (next == "..") current = current->parent;
        else if (next == "/") current = file_system.get();
        else {
          bool found = false;
          for (const auto& dir : current->dirs) {
            if (dir->name == next) {
              found = true;
              current = dir.get();
              std::cout << "Current dir is now: " << current->name << '\n';
              break;
            }
          }
          if (!found) {
            std::cout << "Adding dir: " << next << '\n';
            current->dirs.emplace_back();
            current->dirs.back()->name = next;
            current->dirs.back()->parent = current;
            current = current->dirs.back().get();
            std::cout << "Current dir is now: " << current->name << '\n';
          }
        }
      } else if (cmd == "ls") {
        while (std::getline(file, line)) {
          if (line.substr(0, 3) == "dir") {
            const auto dir_name = line.substr(4, line.size() - 4);
            bool found = false;
            for (const auto& dir : current->dirs) {
              if (dir->name == dir_name) {
                found = true;
              }
            }
            if (!found) {
              std::cout << "Adding dir: " << dir_name << '\n';
              current->dirs.push_back(std::make_unique<Directory>());
              current->dirs.back()->parent = current;
              current->dirs.back()->name = dir_name;
            }
          } else if (line[0] != '$') {
            const auto i = line.find(' ');
            const auto file_size = line.substr(0, i);
            const auto file_name = line.substr(i + 1, line.size() - i - 1);
            std::cout << "Adding file: " << file_name << " with size " << file_size << '\n';
            current->files.push_back(std::make_unique<File>());
            current->files.back()->size = std::stoll(file_size);
            current->files.back()->name = file_name;
          } else {
            skip = true;
            break;
          }
        }
      }
    }
  }
  traverse(file_system.get(), 0);
  update_sizes(file_system.get());
  const long long threshold = 100000;
  auto [count, sum] = count_dirs_lte_threshold(file_system.get(), threshold);
  if (file_system->size <= threshold) {
     count +=  1;
     sum += file_system->size;
  }
  std::cout << count << ' ' << sum << '\n';

  return 0;
}
