#include "basic.h"
#include "os.h"
#include "parser.h"

#ifdef _WIN32
  #include <windows.h>
#endif

File_Handle os_open_file(char *filepath) {
  #ifdef _WIN32
    HANDLE handle = CreateFile(static_cast<LPCSTR>(filepath), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    assert(handle != INVALID_HANDLE_VALUE);

    File_Handle result{};
    result.handle = handle;

    return result;
  #endif
}

size_t os_read_entire_file(File_Handle *file_handle, void **out_ptr) {
  #ifdef _WIN32
    HANDLE handle = file_handle->handle;

    LARGE_INTEGER file_size{};
    if (!GetFileSizeEx(handle, &file_size)) {
        assert(0);
    }

    if (file_size.HighPart != 0) {
        assert(0);
    }

    *out_ptr = malloc(file_size.LowPart);

    DWORD bytes_read;
    if (ReadFile(handle, *out_ptr, file_size.LowPart, &bytes_read, NULL)) {
    } else {
        assert(0);
    }

    return file_size.LowPart;
  #endif
}
