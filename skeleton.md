# Simple Proc Driver - Skeleton Guide

## Overview
This section serves as a skeleton guide to help you create a simple `/proc` driver in Linux. It outlines the steps required to create a kernel module that interacts with the `/proc` filesystem, allowing for reading and writing operations.

## Steps to Create a Simple Proc Driver

### 1. **Create the Module Initialization and Exit Functions**

Every kernel module needs an initialization (`init`) and an exit (`exit`) function. These functions will handle loading and unloading the module.

```c
static int __init my_driver_init(void)
{
    // Initialization code here
    return 0;
}

static void __exit my_driver_exit(void)
{
    // Cleanup code here
}
```

### 2. **Define the `proc_ops` Structure**

The `proc_ops` structure defines the file operations (read and write) for the `/proc` file. You will implement the read and write functions for the proc file.

```c
struct proc_ops my_proc_ops = {
    .proc_read = my_driver_read,  // Define the read function
    .proc_write = my_driver_write // Define the write function
};
```

### 3. **Implement the Read Function**

The read function will be called when the user reads from the `/proc` file. The function should return the data you want to show.

```c
ssize_t my_driver_read(struct file *fp, char *user_space_buffer, size_t count, loff_t *offset)
{
    // Logic to read data from kernel space to user space
    return 0;  // Return number of bytes read
}
```

### 4. **Implement the Write Function**

The write function will handle writing data from user space to kernel space. You can store the received data in a buffer.

```c
ssize_t my_driver_write(struct file *fp, const char __user *buffer, size_t count, loff_t *offset)
{
    // Logic to write data from user space to kernel space
    return count;  // Return number of bytes written
}
```

### 5. **Create the `/proc` Entry**

The `proc_create()` function creates an entry in the `/proc` filesystem. Use `proc_create()` with the desired name, permissions, and the `proc_ops` structure.

```c
proc_entry = proc_create("my_proc_file", 0666, NULL, &my_proc_ops);
```

- `0666` gives read and write permissions.
- `"my_proc_file"` is the name of the file in `/proc`.

### 6. **Cleanup and Remove the `/proc` Entry**

Use `proc_remove()` to clean up and remove the `/proc` entry when the module is unloaded.

```c
proc_remove(proc_entry);
```

### 7. **Build and Load the Module**

To compile and load your kernel module:

- Create a `Makefile` to compile the module.
- Use `make` to build the module.
- Use `insmod` to insert the module into the kernel.
- Use `rmmod` to remove the module.

### Example `Makefile`

```Makefile
obj-m += my_driver.o
all:
    make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
    make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
```

### 8. **Testing the Proc Driver**

After inserting the module (`insmod my_driver.ko`), the `/proc/my_proc_file` will be created. You can test it with the following commands:

```bash
# To read the content
cat /proc/my_proc_file

# To write data
echo "Hello, Kernel!" > /proc/my_proc_file
```

### 9. **Debugging the Driver**

You can use `dmesg` to check the kernel logs for any errors or print statements you added for debugging:

```bash
dmesg | tail
```

---

By following this skeleton, you can easily create a basic `/proc` driver that reads and writes data to/from user space.
