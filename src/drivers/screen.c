#include "./screen.h"
#include "./ports.h"

/* Declaration of private functions */
int get_cursor_offset();
void set_cursor_offset(int offset);
int print_char(char c, int row, int col, char attr);
int get_offset(int row, int col);
int get_offset_row(int offset);
int get_offset_col(int offset);
void print_all(char *message, int row, int col, int offset);

int kprint(char *message) {
    return kprint_at(message, 0, 0);
}

int kprint_at(char *message, int row, int col) {
    if(row < 0 || col < 0) {
        int offset = get_cursor_offset();
        row = get_offset_row(offset);
        col = get_offset_col(offset);
        print_all(message, row, col, offset);
    }
    int offset = get_offset(row, col);
    print_all(message, row, col, offset);
    return 0;
}

void print_all(char *message, int row, int col, int offset) {
    for(int i = 0; message[i] != 0; i++) {
        offset = print_char(message[i], row, col, WHITE_ON_BLACK);
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }
}

int print_char(char c, int row, int col, char attr) {
    unsigned char *vidmem = (unsigned char*) VIDEO_ADDRESS;
    if (!attr) attr = WHITE_ON_BLACK;
    if(col < 0 || row < 0 || col >= MAX_COLS || row >= MAX_ROWS) {
        vidmem[2 *MAX_ROWS *MAX_COLS - 2] = 'E';
        vidmem[2 *MAX_ROWS *MAX_COLS - 1] = RED_ON_WHITE;
        return get_offset(row, col);
    }

    int offset;
    if (col >= 0 && row >= 0) offset = get_offset(col, row);
    else offset = get_cursor_offset();

    if(c == '\n') {
        row = get_offset_row(offset);
        offset = get_offset(0, row + 1);
    } else {
        vidmem[offset] = c;
        vidmem[offset+1] = attr;
        offset += 2;
    }
    set_cursor_offset(offset);
    return offset;
}

int get_cursor_offset() {
    port_byte_out(REG_SCREEN_CTRL, 14);
    int offset = port_byte_in(REG_SCREEN_DATA) << 8;
    port_byte_out(REG_SCREEN_CTRL, 15);
    offset += port_byte_in(REG_SCREEN_DATA);
    return offset * 2;
}

void set_cursor_offset(int offset) {
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}

void clear_screen() {
    int screen_size = MAX_COLS * MAX_ROWS;
    char *screen = VIDEO_ADDRESS;
    for(int i = 0; i < screen_size; i++) {
        screen[ i * 2 ] = ' ';
        screen[ i * 2 + 1 ] = WHITE_ON_BLACK;
    }
    set_cursor_offset(get_offset(0, 0));
}

int get_offset(int col, int row) { return 2 * (row * MAX_COLS + col); }
int get_offset_row(int offset) { return offset / (2 * MAX_COLS); }
int get_offset_col(int offset) { return (offset - (get_offset_row(offset)*2*MAX_COLS))/2; }