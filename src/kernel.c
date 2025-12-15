// kernel.c - Main kernel with VGA, keyboard, and color support
#include "kernel.h"
#include "shell.h"
#include "scheduler.h"
#include "memory.h"

// VGA buffer
volatile unsigned short* vga_buffer = (unsigned short*)0xB8000;
int cursor_x = 0;
int cursor_y = 0;
unsigned char current_color = VGA_COLOR_DEFAULT;

// I/O port functions
void outb(unsigned short port, unsigned char val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

unsigned char inb(unsigned short port) {
    unsigned char ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// Set color for subsequent text
void set_color(unsigned char foreground, unsigned char background) {
    current_color = MAKE_COLOR(foreground, background);
}

// Clear screen with current color
void clear_screen() {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i] = (current_color << 8) | ' ';
    }
    cursor_x = 0;
    cursor_y = 0;
}

// Print character with current color
void print_char(char c) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else if (c == '\b') {
        if (cursor_x > 0) {
            cursor_x--;
            int pos = cursor_y * VGA_WIDTH + cursor_x;
            vga_buffer[pos] = (current_color << 8) | ' ';
        }
    } else {
        int pos = cursor_y * VGA_WIDTH + cursor_x;
        vga_buffer[pos] = (current_color << 8) | c;
        cursor_x++;
        if (cursor_x >= VGA_WIDTH) {
            cursor_x = 0;
            cursor_y++;
        }
    }
    
    if (cursor_y >= VGA_HEIGHT) {
        cursor_y = VGA_HEIGHT - 1;
        for (int i = 0; i < VGA_WIDTH * (VGA_HEIGHT - 1); i++) {
            vga_buffer[i] = vga_buffer[i + VGA_WIDTH];
        }
        for (int i = 0; i < VGA_WIDTH; i++) {
            vga_buffer[VGA_WIDTH * (VGA_HEIGHT - 1) + i] = (current_color << 8) | ' ';
        }
    }
}

// Print string with current color
void print(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        print_char(str[i]);
    }
}

// Print string with specific color
void print_colored(const char* str, unsigned char foreground, unsigned char background) {
    unsigned char old_color = current_color;
    set_color(foreground, background);
    print(str);
    current_color = old_color;
}

// Print integer
void print_int(int num) {
    if (num == 0) {
        print_char('0');
        return;
    }
    
    if (num < 0) {
        print_char('-');
        num = -num;
    }
    
    char buffer[12];
    int i = 0;
    while (num > 0) {
        buffer[i++] = '0' + (num % 10);
        num /= 10;
    }
    
    while (i > 0) {
        print_char(buffer[--i]);
    }
}

// Keyboard driver
char scancode_to_ascii(unsigned char scancode) {
    static const char scancode_map[] = {
        0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
        '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
        0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
        0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*',
        0, ' '
    };
    
    if (scancode < sizeof(scancode_map)) {
        return scancode_map[scancode];
    }
    return 0;
}

char get_key() {
    while (1) {
        if (inb(0x64) & 1) {
            unsigned char scancode = inb(0x60);
            if (scancode < 0x80) {
                return scancode_to_ascii(scancode);
            }
        }
    }
}

// Main kernel entry point with COLORS!
void kernel_main() {
    // Set initial color
    set_color(COLOR_WHITE, COLOR_BLACK);
    clear_screen();
    
    // Cyan header
    set_color(COLOR_LIGHT_CYAN, COLOR_BLACK);
    print("  _____ _____ _____ _____  _____  _____ \n");
    print(" |     |     |   | |     ||     ||   __|\n");
    print(" | | | |-   -| | | |  |  ||   --||__   |\n");
    print(" |_|_|_|_____|_|___|_____||_____||_____|\n");
    
    set_color(COLOR_WHITE, COLOR_BLACK);
    print("\n");
    
    // Yellow title
    set_color(COLOR_YELLOW, COLOR_BLACK);
    print("        >> MiniOS v1.0 - VMware Edition <<\n");
    
    // Magenta subtitle
    set_color(COLOR_LIGHT_MAGENTA, COLOR_BLACK);
    print("           Made with <3 by Your Team\n");
    
    set_color(COLOR_DARK_GREY, COLOR_BLACK);
    print("  =============================================\n\n");
    
    // Green boot message
    set_color(COLOR_LIGHT_GREEN, COLOR_BLACK);
    print("  [*] Booting MiniOS...\n");
    
    set_color(COLOR_CYAN, COLOR_BLACK);
    print("      [");
    set_color(COLOR_GREEN, COLOR_BLACK);
    print("####################");
    set_color(COLOR_CYAN, COLOR_BLACK);
    print("] 100%\n\n");
    
    // White for subsystem init
    set_color(COLOR_WHITE, COLOR_BLACK);
    print("  [*] Initializing subsystems...\n");
    
    // Initialize scheduler
    scheduler_init();
    set_color(COLOR_GREEN, COLOR_BLACK);
    print("      [OK] ");
    set_color(COLOR_WHITE, COLOR_BLACK);
    print("Scheduler initialized\n");
    
    // Initialize memory manager
    memory_init();
    set_color(COLOR_GREEN, COLOR_BLACK);
    print("      [OK] ");
    set_color(COLOR_WHITE, COLOR_BLACK);
    print("Memory manager initialized\n");
    
    print("\n");
    
    // Welcome box in light blue
    set_color(COLOR_LIGHT_BLUE, COLOR_BLACK);
    print("  +-------------------------------------------+\n");
    print("  |");
    set_color(COLOR_YELLOW, COLOR_BLACK);
    print("  Welcome to MiniOS Interactive Shell!   ");
    set_color(COLOR_LIGHT_BLUE, COLOR_BLACK);
    print("|\n");
    
    print("  |");
    set_color(COLOR_WHITE, COLOR_BLACK);
    print("  Type 'help' to see available commands  ");
    set_color(COLOR_LIGHT_BLUE, COLOR_BLACK);
    print("|\n");
    
    print("  |");
    set_color(COLOR_LIGHT_GREEN, COLOR_BLACK);
    print("  Have fun exploring! ^_^                ");
    set_color(COLOR_LIGHT_BLUE, COLOR_BLACK);
    print("|\n");
    
    print("  +-------------------------------------------+\n");
    
    set_color(COLOR_WHITE, COLOR_BLACK);
    print("\n");
    
    // Start shell
    shell_loop();
}
