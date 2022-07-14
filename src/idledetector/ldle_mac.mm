#include "idle.h"

#ifdef Q_OS_MAC

int Idle::getSystemIdleTime()
{
    int idleSeconds = -1;
    io_iterator_t iter = 0;
    if (IOServiceGetMatchingServices(kIOMasterPortDefault, IOServiceMatching("IOHIDSystem"), &iter) == KERN_SUCCESS) {
        io_registry_entry_t entry = IOIteratorNext(iter);
        if (entry) {
            CFMutableDictionaryRef dict = NULL;
            kern_return_t status;
            status = IORegistryEntryCreateCFProperties(entry, &dict, kCFAllocatorDefault, 0);
            if (status == KERN_SUCCESS) {
                CFNumberRef obj = CFDictionaryGetValue(dict, CFSTR("HIDIdleTime"));
                if (obj) {
                    int64_t nanoseconds = 0;
                    if (CFNumberGetValue(obj, kCFNumberSInt64Type, &nanoseconds)) {
                        idleSeconds = (int) nanoseconds / NSEC_PER_SEC;
                    }
                }
                CFRelease(dict);
            }
            IOObjectRelease(entry);
        }
        IOObjectRelease(iter);
    }

    return idleSeconds;
}


#endif