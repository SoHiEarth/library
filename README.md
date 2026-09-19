# library
A CLI tool that acts as a manager for small home libraries.

## Usage

Invoke the tool with the `lib` command.

## Installation

### Required Libraries

- `ncurses`
> Installed by default on MacOS and most Linux distros.
- `pugixml`
> Install using the package manager of your choice.

### Building

1. Clone the repo using `git clone https://github.com/sohiearth/library lib` (Use the `lib` directory to prevent conflicts with MacOS)
2. `cd` into the directory.
3. `cmake -B build`
4. `cmake --build build`
