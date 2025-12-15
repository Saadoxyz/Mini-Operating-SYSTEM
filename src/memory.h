// memory.h - Memory management interface
#ifndef MEMORY_H
#define MEMORY_H

#define PAGE_SIZE 4096
#define FRAME_COUNT 16
#define MAX_PAGES_PER_PROCESS 32

// Frame structure
typedef struct {
    int pid;           // Process using this frame (-1 if free)
    int page_number;   // Page number stored in this frame
    int last_access;   // Last access time (for LRU)
    int valid;         // Frame is in use
} frame_t;

// Page table entry
typedef struct {
    int frame_number;  // Physical frame number
    int valid;         // Page is loaded in memory
} page_entry_t;

// Memory management functions
void memory_init();
void memory_show_info();
void memory_show_frames();
int memory_allocate_pages(int pid, int count);
void memory_access_page(int pid, int page);
int memory_get_free_frame();
int memory_find_lru_frame();

#endif
