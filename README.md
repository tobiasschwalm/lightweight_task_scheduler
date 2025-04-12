# Lightweight Task Scheduler (LTS)

The Lightweight Task Scheduler (LTS) is a simple and efficient task scheduler implemented in C. It allows you to create, manage, and execute periodic tasks. Designed for embedded systems, LTS is lightweight, easy to integrate, and highly customizable.

## Features

- Create and manage periodic tasks.
- Start and stop the scheduler dynamically.
- Mockable time functions for testing.
- Low memory footprint with static memory allocation.
- Example usage and unit tests included.

## Platform Abstraction

LTS is portable across platforms. Time management functions (`current_time_ms` and `sleep_ms`) are abstracted in `platform_time.h`, allowing developers to provide platform-specific implementations. Examples include:

- **POSIX Systems**: Use `clock_gettime` and `usleep`.
- **Microcontrollers**: Use hardware timers like `HAL_GetTick` and `HAL_Delay` (e.g., STM32).
- **Custom Platforms**: Implement using available hardware timers or delay functions.

This abstraction ensures easy adaptation to any platform.

## Memory Requirements

LTS is designed for embedded systems with limited memory. All memory is statically allocated at compile time, eliminating the need for dynamic memory allocation. The memory usage depends on the `MAX_TASKS` macro defined in `scheduler_config.h`.

### Memory Breakdown
- **Task Structure**: 18 bytes per task (on a 32-bit system).
- **Scheduler Context**: Includes an array of tasks and metadata:
  ```c
  typedef struct SchedulerContext 
  {
      task_id_t numTasks;                  /* 4 bytes */
      Task tasks[MAX_TASKS];               /* MAX_TASKS * 18 bytes */
      volatile uint8_t scheduler_stopped;  /* 1 byte */
  } SchedulerContext;
  ```
- **Example**:
  - **MAX_TASKS = 10**: 185 bytes total.
  - **MAX_TASKS = 50**: 905 bytes total.

This predictable memory usage makes LTS suitable for resource-constrained environments.

## Unit Testing

LTS includes a comprehensive suite of unit tests located in the `tests/` directory. These tests cover:

- Task creation, start, stop, and removal.
- Scheduler start and stop behavior.
- Task execution with different cycles and offsets.

Mock implementations of platform-specific functions (`current_time_ms` and `sleep_ms`) are used to simulate time progression, ensuring deterministic and repeatable testing.

To run the tests:
```bash
./build/test_scheduler
```

## Getting Started with the Devcontainer

The project is configured to be built inside a development container using CMake. This ensures a consistent development environment.

### Steps to Build and Run

1. **Open the Devcontainer**:
   - Install the [DevContainers Extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers) in Visual Studio Code.
   - Open the project folder in VS Code.
   - Reopen the folder in the devcontainer when prompted, or use the **Command Palette** (`Ctrl+Shift+P`) and select:
     ```
     Dev Containers: Reopen in Container
     ```

2. **Build the Project**:
   - Run the `build.sh` script to configure and build the project:
     ```bash
     ./build.sh
     ```

3. **Run Examples**:
   - After building, the example executables will be located in the `build` directory. Run them using:
     ```bash
     ./build/example_one_time_task
     ./build/example_periodic_task
     ./build/example_stop_scheduler
     ./build/example_error_handling
     ```

4. **Run Tests**:
   - The test executable will also be located in the `build` directory. Run the tests using:
     ```bash
     ./build/test_scheduler
     ```

## Directory Structure

```
/workspaces/lts
├── lib/                # LTS library source and headers
│   ├── include/        # Public headers
│   └── src/            # Library implementation
├── example/            # Example usage of LTS
│   └── src/            # Example source code
├── tests/              # Unit tests for LTS
├── .devcontainer/      # Devcontainer configuration
├── build.sh            # Build script
├── CMakeLists.txt      # CMake configuration
└── README.md           # Project documentation
```

## Contributing

Contributions are welcome! Feel free to open issues or submit pull requests to improve LTS.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

### Third-Party Licenses

This project uses the following third-party libraries:

- **Highlight.js**: Used for syntax highlighting in code examples.  
  License: [BSD 3-Clause License](https://github.com/highlightjs/highlight.js/blob/main/LICENSE)  
  Copyright (c) 2006-2023 Ivan Sagalaev and other contributors.

- **Roboto Font**: Used as the primary font for the webpage.  
  License: [Apache License, Version 2.0](https://fonts.google.com/specimen/Roboto#license)  
  Copyright (c) 2011 Google.

Please ensure compliance with the respective licenses when redistributing or modifying this project.

