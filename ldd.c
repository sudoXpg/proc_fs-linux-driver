#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("sudoXpg");
MODULE_DESCRIPTION("helloworld");

#define MAX_MSG_SIZE 1024   // Define maximum allowed message size

// Global buffer to store input from user space
char *stored_msg;                      
static size_t proc_buffer_size = 0;  // To track the size of the buffer

// Pointer to the proc file entry
struct proc_dir_entry *proc_entry;

/**
 * driver_read - Reads the content of the /proc/huh file.
 * @ fp: The file pointer (not used here).
 * @ user_space_buffer: The buffer to copy the data to.
 * @ count: The maximum number of bytes to copy.
 * @ offset: The current offset (file pointer).
 * 
 * Returns the number of bytes read or 0 if no more data.
 */
ssize_t driver_read(struct file *fp, char *user_space_buffer, size_t count, loff_t *offset) {
    size_t len = strlen(stored_msg);  // Get the length of the stored message

    // If the message is empty, return 0 (no data to read)
    if (!stored_msg) {
        return 0;
    }

    // Avoid infinite loop by checking if the offset has reached the length
    if (*offset >= len) {
        return 0;
    }

    // Copy the message to user space buffer
    int res = copy_to_user(user_space_buffer, stored_msg, len);
    if (res != 0) {
        return -EFAULT;  // If copy fails, return an error
    }

    *offset += len;  // Increment the offset to avoid infinite reads
    printk("inside driver read\n");

    return len;  // Return the number of bytes read
}

/**
 * driver_write - Handles writing to the /proc/huh file.
 * @ filename: The file pointer (not used here).
 * @ buffer: The data to write (from user space).
 * @ count: The number of bytes to write.
 * @ offset: The current offset (file pointer).
 * 
 * Returns the number of bytes written or an error code.
 */
ssize_t driver_write(struct file *filename, const char __user *buffer, size_t count, loff_t *offset) {
    // Initialize the buffer if it hasn't been done yet
    if (stored_msg == NULL) {
        stored_msg = kmalloc(count + 1, GFP_KERNEL);  // Allocate memory for the message
        if (!stored_msg) {
            return -ENOMEM;  // Return error if memory allocation fails
        }
    }

    // Check if the message size exceeds the maximum allowed size
    if (count > MAX_MSG_SIZE - 1) {
        return -EINVAL;  // Return error if the input is too large
    }

    // Copy the data from user space to kernel space
    int res = copy_from_user(stored_msg, buffer, count);
    if (res != 0) {
        return -EFAULT;  // Return error if copying fails
    }

    stored_msg[count] = '\0';  // Null-terminate the string to prevent overflow

    *offset += count;  // Increment the offset (file pointer)

    return count;  // Return the number of bytes written
}

/**
 * driver_proc_ops - Define the file operations for /proc/huh.
 * .proc_read: Function to handle reading from /proc/huh.
 * .proc_write: Function to handle writing to /proc/huh.
 */
struct proc_ops driver_proc_ops = {
    .proc_read = driver_read,   // Set the read function
    .proc_write = driver_write  // Set the write function
};

/**
 * kernel_module_init - Initializes the kernel module and creates /proc/huh.
 * 
 * Returns 0 on success.
 */
static int kernel_module_init(void) {
    printk("proc create start\n");
    proc_entry = proc_create("huh", 0666, NULL, &driver_proc_ops);  // Create the /proc file with read/write permissions
    if (!proc_entry) {
        return -ENOMEM;  // Return error if proc_create fails
    }
    printk("proc create end\n");
    return 0;
}

/**
 * kernel_module_end - Cleans up the kernel module by removing /proc/huh.
 */
static void kernel_module_end(void) {
    printk("proc remove start\n");
    proc_remove(proc_entry);  // Remove the proc file
    printk("proc remove end\n");
}

// Register the module's init and exit functions
module_init(kernel_module_init);
module_exit(kernel_module_end);
