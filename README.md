# XenonProxy

## Features

- **C23 Focus**: Leverages modern C23 features (e.g., `nullptr`) for clean, portable code.

## Purpose

Built as a learning exercise to master C23 fundamentals—file I/O, directory traversal, memory management, and system calls—while delivering a functional backup tool. Think of it as a sandbox for exploring modern C23 features.

## Installation

```sh
    gcc -std=c2x -Wall -Wextra -Wpedantic -o xenonbk main.c
    clang -std=c23 -Wall -Wextra -Wpedantic -o xenonbk main.c
