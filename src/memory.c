// memory.c - Virtual memory paging simulator
#include "kernel.h"
#include "memory.h"
#include "scheduler.h"

static frame_t frames[FRAME_COUNT];
static page_entry_t page_tables[MAX_PROCESSES][MAX_PAGES_PER_PROCESS];
static int current_time = 0;
static int page_faults = 0;
static int page_hits = 0;

// Initialize memory manager
void memory_init() {
    for (int i = 0; i < FRAME_COUNT; i++) {
        frames[i].pid = -1;
        frames[i].page_number = -1;
        frames[i].last_access = 0;
        frames[i].valid = 0;
    }
    
    for (int i = 0; i < MAX_PROCESSES; i++) {
        for (int j = 0; j < MAX_PAGES_PER_PROCESS; j++) {
            page_tables[i][j].frame_number = -1;
            page_tables[i][j].valid = 0;
        }
    }
    
    current_time = 0;
    page_faults = 0;
    page_hits = 0;
}

// Show memory statistics
void memory_show_info() {
    int used_frames = 0;
    for (int i = 0; i < FRAME_COUNT; i++) {
        if (frames[i].valid) used_frames++;
    }
    
    print("\n");
    set_color(COLOR_CYAN, COLOR_BLACK);
    print("  ===============================================\n");
    set_color(COLOR_YELLOW, COLOR_BLACK);
    print("              Memory Information\n");
    set_color(COLOR_CYAN, COLOR_BLACK);
    print("  ===============================================\n\n");
    
    set_color(COLOR_WHITE, COLOR_BLACK);
    print("  Page size: ");
    set_color(COLOR_GREEN, COLOR_BLACK);
    print_int(PAGE_SIZE);
    set_color(COLOR_WHITE, COLOR_BLACK);
    print(" bytes\n");
    
    print("  Total frames: ");
    set_color(COLOR_CYAN, COLOR_BLACK);
    print_int(FRAME_COUNT);
    set_color(COLOR_WHITE, COLOR_BLACK);
    print("\n");
    
    print("  Used frames: ");
    set_color(COLOR_YELLOW, COLOR_BLACK);
    print_int(used_frames);
    set_color(COLOR_WHITE, COLOR_BLACK);
    print(" [");
    set_color(COLOR_GREEN, COLOR_BLACK);
    for (int i = 0; i < used_frames; i++) print("#");
    set_color(COLOR_DARK_GREY, COLOR_BLACK);
    for (int i = used_frames; i < FRAME_COUNT; i++) print("-");
    set_color(COLOR_WHITE, COLOR_BLACK);
    print("]\n");
    
    print("  Free frames: ");
    set_color(COLOR_LIGHT_GREEN, COLOR_BLACK);
    print_int(FRAME_COUNT - used_frames);
    set_color(COLOR_WHITE, COLOR_BLACK);
    print("\n\n");
    
    set_color(COLOR_LIGHT_BLUE, COLOR_BLACK);
    print("  Statistics:\n");
    set_color(COLOR_WHITE, COLOR_BLACK);
    print("    * Page faults: ");
    set_color(COLOR_RED, COLOR_BLACK);
    print_int(page_faults);
    set_color(COLOR_WHITE, COLOR_BLACK);
    print("\n");
    
    print("    * Page hits: ");
    set_color(COLOR_GREEN, COLOR_BLACK);
    print_int(page_hits);
    set_color(COLOR_WHITE, COLOR_BLACK);
    print("\n");
    
    if (page_faults + page_hits > 0) {
        int hit_rate = (page_hits * 100) / (page_faults + page_hits);
        print("    * Hit rate: ");
        if (hit_rate > 70) {
            set_color(COLOR_GREEN, COLOR_BLACK);
        } else if (hit_rate > 40) {
            set_color(COLOR_YELLOW, COLOR_BLACK);
        } else {
            set_color(COLOR_RED, COLOR_BLACK);
        }
        print_int(hit_rate);
        print("%");
        set_color(COLOR_WHITE, COLOR_BLACK);
        print("\n");
    }
    print("\n");
}
// Show frame allocation table
void memory_show_frames() {
    print("\n=== Frame Allocation Table ===\n");
    print("Frame  PID  Page  Last Access\n");
    print("-----  ---  ----  -----------\n");
    
    for (int i = 0; i < FRAME_COUNT; i++) {
        // Frame number
        if (i < 10) print(" ");
        print_int(i);
        print("    ");
        
        if (frames[i].valid) {
            // PID
            if (frames[i].pid < 10) print(" ");
            print_int(frames[i].pid);
            print("   ");
            
            // Page
            if (frames[i].page_number < 10) print(" ");
            print_int(frames[i].page_number);
            print("   ");
            
            // Last access
            if (frames[i].last_access < 10) print(" ");
            print_int(frames[i].last_access);
            print("\n");
        } else {
            print("---  ----  -----------\n");
        }
    }
    print("\n");
}

// Get free frame
int memory_get_free_frame() {
    for (int i = 0; i < FRAME_COUNT; i++) {
        if (!frames[i].valid) {
            return i;
        }
    }
    return -1;
}

// Find LRU frame for replacement
int memory_find_lru_frame() {
    int lru_frame = 0;
    int oldest_time = frames[0].last_access;
    
    for (int i = 1; i < FRAME_COUNT; i++) {
        if (frames[i].last_access < oldest_time) {
            oldest_time = frames[i].last_access;
            lru_frame = i;
        }
    }
    
    return lru_frame;
}

// Allocate pages to process
int memory_allocate_pages(int pid, int count) {
    pcb_t* proc = scheduler_get_process(pid);
    if (!proc) {
        print("Error: Process not found\n");
        return 0;
    }
    
    if (count > MAX_PAGES_PER_PROCESS) {
        print("Error: Too many pages requested\n");
        return 0;
    }
    
    // Mark pages as allocated but not loaded
    for (int i = 0; i < count; i++) {
        page_tables[pid % MAX_PROCESSES][i].valid = 0;
        page_tables[pid % MAX_PROCESSES][i].frame_number = -1;
    }
    
    return 1;
}

// Access a page (simulate memory access)
void memory_access_page(int pid, int page) {
    current_time++;
    
    pcb_t* proc = scheduler_get_process(pid);
    if (!proc) {
        print("Error: Process ");
        print_int(pid);
        print(" not found\n");
        return;
    }
    
    if (page >= MAX_PAGES_PER_PROCESS) {
        print("Error: Invalid page number\n");
        return;
    }
    
    int proc_index = pid % MAX_PROCESSES;
    page_entry_t* pte = &page_tables[proc_index][page];
    
    // Page hit
    if (pte->valid) {
        page_hits++;
        frames[pte->frame_number].last_access = current_time;
        print("Page hit: PID=");
        print_int(pid);
        print(" page=");
        print_int(page);
        print(" frame=");
        print_int(pte->frame_number);
        print("\n");
        return;
    }
    
    // Page fault
    page_faults++;
    print("Page fault: PID=");
    print_int(pid);
    print(" page=");
    print_int(page);
    
    int frame = memory_get_free_frame();
    
    // No free frame - use LRU replacement
    if (frame == -1) {
        frame = memory_find_lru_frame();
        
        // Evict old page
        int old_pid = frames[frame].pid;
        int old_page = frames[frame].page_number;
        
        print(" (evicting PID=");
        print_int(old_pid);
        print(" page=");
        print_int(old_page);
        print(")");
        
        // Invalidate old page table entry
        page_tables[old_pid % MAX_PROCESSES][old_page].valid = 0;
    }
    
    // Load new page
    frames[frame].pid = pid;
    frames[frame].page_number = page;
    frames[frame].last_access = current_time;
    frames[frame].valid = 1;
    
    pte->frame_number = frame;
    pte->valid = 1;
    
    print(" -> loaded to frame=");
    print_int(frame);
    print("\n");
}
