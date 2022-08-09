#include "activity.h"


WindowInfo getActiveWindow(){
    // get frontmost process for currently active application
    ProcessSerialNumber psn = { 0L, 0L };
    OSStatus err = GetFrontProcess(&psn);

    CFStringRef processName = NULL;
    err = CopyProcessName(&psn, &processName);

    NSString *pname = (NSString *)processName;

    // loop through all application windows
    CFArrayRef windowList = CGWindowListCopyWindowInfo(kCGWindowListOptionOnScreenOnly | kCGWindowListExcludeDesktopElements, kCGNullWindowID);
    for (NSMutableDictionary* entry in (NSArray*)windowList)
    {
        NSString* ownerName = [entry objectForKey:(id)kCGWindowOwnerName];
        NSString *name = [entry objectForKey:@"kCGWindowName" ];
        NSInteger ownerPID = [[entry objectForKey:(id)kCGWindowOwnerPID] integerValue];
        NSInteger layer = [[entry objectForKey:@"kCGWindowLayer"] integerValue];
        if(layer == 0)
        {
            if([ownerName isEqualToString:pname])
            {
                NSRange range;
                range.location = 0;
                range.length = [ownerName length];

                unichar *chars = new unichar[range.length];
                [ownerName getCharacters:chars range:range];
                QString owner = QString::fromUtf16(chars, range.length);

                range.length = [name length];

                chars = new unichar[range.length];
                [name getCharacters:chars range:range];
                QString windowTitle = QString::fromUtf16(chars, range.length);
                delete[] chars;

                long pid = (long)ownerPID;

                WindowInfo wi;
                wi.setProcessName(owner);
                wi.setWindowTitle(windowTitle);
                wi.setPID(pid);

                CFRelease(windowList);
                CFRelease(processName);
                
                return wi;
            }
        }
    }
}

int Activity::getSystemIdleTime()
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