#include "bcm2837/rpi_mbox.h"
#include "mbox.h"

/* Aligned to 16-byte boundary while we have 28-bits for VC */
volatile unsigned int __attribute__((aligned(16))) pt[64];

// Overall, this code implements a low-level interface for communicating with the Raspberry Pi's VideoCore co-processor using the mailbox mechanism
int mbox_call(mbox_channel_type channel, unsigned int value) {
    // the resulting value contains the original message with the channel value set in the lower 4 bits, which is then sent to the mailbox
    value &= ~(0xF); // AND-ed with a 32-bit integer with all bits set to 1 except for the lower 4 bits (which are 0) -> This clears the lower 4 bits of the value argument
    value |= channel; // OR-ed with the lower 4 bits of value -> This sets the lower 4 bits of value to the channel value.
    while ((*MBOX_STATUS & BCM_ARM_VC_MS_FULL) != 0) {} // waits for the mailbox status register to indicate that it is ready for the message to be sent

    // Write to register
    *MBOX_WRITE = value;
    while(1) {
        while (*MBOX_STATUS & BCM_ARM_VC_MS_EMPTY) {} // The loop checks the mailbox status register to see if there is data available, and then reads the response from the mailbox read register
        // If the response matches the original message value, the function returns a boolean value indicating whether the mailbox request was successful or not
        if (value == *MBOX_READ) {
            return pt[1] == MBOX_REQUEST_SUCCEED;
        }
        // If no response is received within the loop, the function returns 0
        return 0;
    }
}