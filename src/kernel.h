// kernel.h - Kernel function declarations
#ifndef KERNEL_H
#define KERNEL_H

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

// VGA Color codes
#define COLOR_BLACK         0x0
#define COLOR_BLUE          0x1
#define COLOR_GREEN         0x2
#define COLOR_CYAN          0x3
#define COLOR_RED           0x4
#define COLOR_MAGENTA       0x5
#define COLOR_BROWN         0x6
#define COLOR_LIGHT_GREY    0x7
#define COLOR_DARK_GREY     0x8
#define COLOR_LIGHT_BLUE    0x9
#define COLOR_LIGHT_GREEN   0xA
#define COLOR_LIGHT_CYAN    0xB
#define COLOR_LIGHT_RED     0xC
#define COLOR_LIGHT_MAGENTA 0xD
#define COLOR_YELLOW        0xE
#define COLOR_WHITE         0xF

// Helper macro to make color attribute
#define MAKE_COLOR(fg, bg) ((bg << 4) | fg)
#define VGA_COLOR_DEFAULT MAKE_COLOR(COLOR_WHITE, COLOR_BLACK)

// Global variables
extern volatile unsigned short* vga_buffer;
extern int cursor_x;
extern int cursor_y;
extern unsigned char current_color;

// I/O functions
void outb(unsigned short port, unsigned char val);
unsigned char inb(unsigned short port);

// Display functions
void clear_screen();
void set_color(unsigned char foreground, unsigned char background);
void print_char(char c);
void print(const char* str);
void print_colored(const char* str, unsigned char foreground, unsigned char background);
void print_int(int num);

// Keyboard functions
char scancode_to_ascii(unsigned char scancode);
char get_key();

#endif
