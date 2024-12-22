# /proc File Kernel Module - Huh

This project demonstrates how to create a custom `/proc` file in the Linux kernel. The kernel module allows reading and writing to a virtual `/proc/huh` file. It handles user input and provides a mechanism to store and retrieve messages from the kernel.

## Features

- Creates a `/proc/huh` file where messages can be written and read.
- Supports reading from the `/proc/huh` file, returning the stored message.
- Allows writing messages to `/proc/huh` using the `write` function, with error handling for input size and memory allocation.
- Implements basic error checks and memory management using `kmalloc` and `copy_from_user`.

## Files

- **`huh.c`** - The main kernel module file that creates and manages the `/proc/huh` file.

## How It Works

### Kernel Module Overview

- The module defines two operations: reading (`proc_read`) and writing (`proc_write`) to the `/proc/huh` file.
- When data is written to the file, it is stored in a kernel buffer.
- When the file is read, the stored message is returned to user space.

### Code Explanation

1. **Global Variables**:
    - `stored_msg`: A pointer to the kernel buffer used to store the written message.
    - `proc_entry`: A pointer to the `proc_dir_entry` structure that represents the `/proc/huh` file.

2. **Functions**:
    - **`driver_read`**: Handles reading from `/proc/huh`. It returns the stored message to user space. It avoids infinite reading by using an offset.
    - **`driver_write`**: Handles writing to `/proc/huh`. It copies data from user space to kernel space, checks input size, and allocates memory dynamically.

3. **Initialization and Cleanup**:
    - **`kernel_module_init`**: Creates the `/proc/huh` file and registers the read and write operations.
    - **`kernel_module_end`**: Cleans up by removing the `/proc/huh` file when the module is unloaded.

## Installation

### Build the Kernel Module

1. Clone this repository to your local machine.
2. Navigate to the directory containing the source code.
3. Run the following command to compile the kernel module:

   ```bash
   make
   ```

### Load the Kernel Module

To load the module into the kernel:

```bash
sudo insmod ldd.ko
```

### Interacting with `/proc/huh`

- **Write to `/proc/huh`**:

   Use `echo` to write a message to the `/proc/huh` file:

   ```bash
   echo "Hello, Kernel!" > /proc/huh
   ```

- **Read from `/proc/huh`**:

   Use `cat` to read the stored message:

   ```bash
   cat /proc/huh
   ```

### Remove the Kernel Module

To remove the module from the kernel:

```bash
sudo rmmod huh
```



## Reference for Future Projects

### When to Use This Knowledge:

- **Creating Custom `/proc` Files**: This module is a great reference for when you need to create custom `/proc` files to interact with kernel space from user space.
- **Reading and Writing Kernel Data**: The code demonstrates how to handle user-space read/write operations for kernel modules.
- **Error Handling**: The module includes basic error handling for user input, memory allocation, and buffer copying. This is a good reference for managing user data in kernel space.

### Key Concepts for Future Use:

- **`kmalloc`**: Dynamic memory allocation in kernel space.
- **`copy_from_user` and `copy_to_user`**: Functions for transferring data between user space and kernel space.
- **`proc_create` and `proc_remove`**: Functions to create and remove `/proc` entries.
- **File Operations (`.proc_read`, `.proc_write`)**: Customize how files in `/proc` behave by defining read and write operations.

## License

This project is licensed under the GPL-2.0 License [LICENSE](LICENSE)


