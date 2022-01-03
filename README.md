#Linux Device Drivers

*   **arrays**
    *   passing array of numbers through command line arguments and printing numbers
*   **cmd_line_arguments**
    *   passing strings through cmd line arguments
*   **Export Symbol**
    *   This contains dependency of one modules
    *   created addition of array function in add.c file and using this function in avg file which does the average of number we got through the above function in some other module
*   **hello world**
    *   This is just basic hello world module
*   **character drivers**
    *   created 4 drivers and 4 applications to test add,sub,mul,div
*   **semaphores**
    *   created two producers writing to kernel buffer
    and checking in kernel if buffer is getting overwritten.
    *   created two threads where in two threads are writing and reading.
*   **IOCTL**
    *   sending uart configuration data through ioctl function
*   **gpio**
    *   switch interrupt program.In interrupt handler toggling led pin.
*   **Timers**
    *   blinking led using timers by sending data through ioctl


