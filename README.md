# shelly - A Simple Shell in C

## Overview
**shelly** is a basic command-line shell implemented in C.
**shelly**, which means *mine* *(שלי)* in hebrew, is a personal project which strives to replicate the core functionalities of a Unix shell, such as executing commands, handling built-in commands, and managing processes.

## Features
- **Execute commands**: Run external commands using `execve`.
- **Built-in commands**: Implement common shell commands like `cd`, `pwd` and `exit`.
- **Signal handling**: Manage `Ctrl+C` (SIGINT) gracefully.
- TODO: **Process management**: Handle foreground and background processes.
- TODO: **Basic I/O redirection**: Support input/output redirection (`>`, `<`).
- TODO: **Pipelining**: Execute multiple commands using pipes (`|`).

## Installation
1. Clone the repository:
   ```sh
   git clone https://github.com/gadkadosh/shelly.git
   cd shelly
   ```
2. Compile the shell:
   ```sh
   make
   ```
3. Run the shell:
   ```sh
   bin/shelly
   ```

## Usage
- Run commands like you would in a typical shell:
  ```sh
  ls -l
  echo "Hello, world!"
  ```
- Change directories:
  ```sh
  cd /path/to/directory
  ```
- Exit the shell:
  ```sh
  exit
  ```

## Future Improvements
- Input/output redirection
- Pipes
- Background execution
- Implement job control (`fg`, `bg` commands).
- Add support for environment variables.
- Improve error handling and debugging tools.
- Implement tab completion and command history.
- Prompt customization

## License
This project is licensed under the MIT License.
