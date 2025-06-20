#pragma once

#ifdef _WIN32
  #include <windows.h>
#endif

struct File_Handle {
  #ifdef _WIN32
    HANDLE handle;
  #endif
};

struct Thread_Handle {
  #ifdef _WIN32
    HANDLE handle;
  #endif
};

File_Handle os_open_file(char *filepath);
size_t os_read_entire_file(File_Handle *file_handle, void **out_ptr);
#ifdef _WIN32
DWORD WINAPI ThreadProc(LPVOID lpParameter);
#endif
Thread_Handle os_create_thread(void *parameter);
