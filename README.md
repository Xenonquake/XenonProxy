# XenonProxy

Xenon-Backup!

A lightweight, recursive file backup utility written in C23, designed for learning the fundamentals of modern C programming while providing a practical command-line tool. `xenonbk` takes a source directory and recursively copies it to a destination directory with a focus on simplicity and educational value.

## Features

- **C23 Focus**: Leverages modern C23 features (e.g., `nullptr`) for clean, portable code.
- **Simple CLI**: `xenonbk <source_dir> <destination_dir>`—easy to use in scripts or cron jobs.
- **Recursive Copying**: Handles nested directories with a 4 KB buffer for efficient I/O.
- **Error Handling**: Basic checks and reporting for file/directory operations.

## Purpose

Built as a learning exercise to master C23 fundamentals—file I/O, directory traversal, memory management, and system calls—while delivering a functional backup tool. Think of it as a sandbox for exploring modern C23 features.

## Installation

```sh
    gcc -std=c2x -Wall -Wextra -Wpedantic -o xenonbk main.c
    clang -std=c23 -Wall -Wextra -Wpedantic -o xenonbk main.c
