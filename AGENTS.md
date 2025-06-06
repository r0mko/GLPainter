This repository contains QtQuick components for hardware accelerated rendering of complex UI elements like editors, 2D diagrams, mapping renderers and high performance text rendering.

Guidelines for contributions:

- **Language and Standard:** All code is written in C++20. Build system is CMake and Qt 6 is used throughout the project.
- **Documentation:** English is the only allowed language for comments and documentation. In source code, use Doxygen style comments. In other file formats, use the conventional comment style of that file type. Keep comments minimal and only when code intent is not obvious.
- **Directory layout:**
  - `lib/` contains libraries and QML plugins.
  - `bin/` contains executables.
  - `test/` contains tests written with GoogleTest.
- **Merge strategy:** Always squash commits when merging pull requests so the repository history shows only one commit per change.
- **Commits and branch names:** Use only printable ASCII characters for branch names and commit messages. Do not use non-ASCII symbols, non-breaking spaces, bidirectional unicode marks, or any characters outside the standard printable ASCII range. All commit messages and branch names must be in clear English.
- **Precondition:** Any code generation, refactoring, or automated modification must be preceded by a successful CMake configuration (cmake dry run). If `cmake -S . -B ../build` fails, do not attempt further code changes or generation until configuration errors are resolved.



Follow these instructions unless overridden by a more specific AGENTS.md deeper in the directory tree.
