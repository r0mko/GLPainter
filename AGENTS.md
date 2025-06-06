This repository contains QtQuick components for hardware accelerated rendering of complex UI elements like editors, 2D diagrams, mapping renderers and high performance text rendering.

Guidelines for contributions:

- **Language and Standard:** All code is written in C++20. Build system is CMake and Qt 6 is used throughout the project.
- **Documentation:** English is the only allowed language for comments and documentation. In source code, use Doxygen style comments. In other file formats, use the conventional comment style of that file type. Keep comments minimal and only when code intent is not obvious.
- **Directory layout:**
  - `lib/` contains libraries and QML plugins.
  - `bin/` contains executables.
  - `test/` contains tests written with GoogleTest.
- **Merge strategy:** Always squash commits when merging pull requests so the repository history
  shows only one commit per change.

Follow these instructions unless overridden by a more specific AGENTS.md deeper in the directory tree.

Before running any task, run the environment setup script:
```bash
apt update && apt install -y libgl1-mesa-dev
pip install aqtinstall
aqt install-qt linux desktop 6.9.1 -O /workspace/qt
export PATH=/workspace/qt/6.9.1/gcc_64/bin:$PATH
export CMAKE_PREFIX_PATH=/workspace/qt/6.9.1/gcc_64
```
