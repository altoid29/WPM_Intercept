# WPM_Intercept
A DLL utility for intercepting WriteProcessMemory calls and dumping the buffer back to disk.

# Usage
- [1] Open the program that calls WriteProcessMemory.
- [2] Use standard DLL injection to load the module into that process.
- [3] Any WriteProcessMemory calls will now be dumped back to disk.

# Output
All files are dumped as ".bin", meaning they're binary files for analysis purposes. If you've dumped a module, you can now use cmd to copy all the binaries in their corresponding order and re-create the binary from disk into a module that can now be analyzed and loaded back into memory.
