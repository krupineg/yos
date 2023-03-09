#ifdef __aarch64__
#include "./arm64/ports.h"
#elif __i386__
#include "./i386/ports.h"
#endif
#include "./screen.h"
#include "../kernel/util.h"

/* Declaration of private functions */
int get_cursor_offset();
void set_cursor_offset(int offset);
int print_char(char * buffer, int width, int height, int bytePerPixel, char c, int row, int col, char attr);
int get_offset(int width, int bpp, int row, int col) ;
int get_offset_row(int width, int bpp, int offset) ;
int get_offset_col(int width, int bpp, int offset);
int print_all(char *message, int row, int col);
int print_unsafe(char * buffer, int width, int height, int bytePerPixel, char *message, int row, int col, char attr);

char * SCREEN_BUFFER = 0x0;
int SCREEN_WIDTH = 0;
int SCREEN_HEIGHT = 0;
int SCREEN_BYTE_PER_PIXEL = 0;
int SCREEN_START_ROW = 1;
int SCREEN_START_COLUMN = 1;
const int SCREEN_ERROR_ROW = 0;
const int SCREEN_ERROR_COLUMN = 0;

struct Viewport
{
    char* buffer;
    int width;
    int height;
    /* data */
};


int should_be_configured() {
    if(SCREEN_BUFFER == 0x0 || SCREEN_WIDTH == 0 || SCREEN_HEIGHT == 0 || SCREEN_BYTE_PER_PIXEL == 0) {
        return ERROR_UNCONFIGURED;
    }
    return 0;
}

int configure_screen(char * buffer, int width, int height, int startRow, int startColumn, int pixel) {
    SCREEN_BUFFER = buffer;
    SCREEN_WIDTH = width;
    SCREEN_HEIGHT = height;
    SCREEN_BYTE_PER_PIXEL = pixel;
    SCREEN_START_ROW = startRow;
    SCREEN_START_COLUMN = startColumn;
    return should_be_configured();
}


int kprint(char *message) {
    return kprint_at(message, -1, -1);
}

int kprint_at(char *message, int row, int col) {
    if(should_be_configured() != 0) {
        return ERROR_UNCONFIGURED;
    }
    if(row < 0 || col < 0) {
        int offset = get_cursor_offset();
        row = get_offset_row(SCREEN_WIDTH, SCREEN_BYTE_PER_PIXEL, offset);
        col = get_offset_col(SCREEN_WIDTH, SCREEN_BYTE_PER_PIXEL, offset);
    }
    return print_all(message, row, col);
}

int print_unsafe(char * buffer, int width, int height, int bytePerPixel, char *message, int row, int col, char attr) {
    
    for(int i = 0; message[i] != 0; i++) {
        int offset = print_char(buffer, width, height, bytePerPixel, message[i], row, col, attr);
        if(offset < 0) return offset;
        //scroll
        /*
        if(offset >= width * height * bytePerPixel) {
            for(int row = 1; row < height; i++) {
                memory_copy(buffer + get_offset(width, bytePerPixel, i, 0), buffer + get_offset(width, bytePerPixel, i -1, 0), width * bytePerPixel);
            }
            char *last_line = get_offset(width, bytePerPixel, 0, height - 1) + buffer;
            for (i = 0; i < width *bytePerPixel; i++) last_line[i] = 0;
        }
        */
        set_cursor_offset(offset);
        row = get_offset_row(width, bytePerPixel, offset);
        col = get_offset_col(width, bytePerPixel,offset);

    }
    return 0;
}

char* get_error_buffer() {
    if(should_be_configured() == 0) {
        return SCREEN_BUFFER;
    }
    return DEFAULT_VIDEO_ADDRESS;
}

int get_error_buffer_width() {
    if(should_be_configured() == 0) {
        return SCREEN_WIDTH;
    }
    return MAX_COLS;
}

int get_error_buffer_byte_per_pixel() {
    if(should_be_configured() == 0) {
        return SCREEN_BYTE_PER_PIXEL;
    }
    return 2;
}

void print_error(char *message) {
    char *buffer = get_error_buffer();
    int width = get_error_buffer_width();
    int bpp = get_error_buffer_byte_per_pixel();
    if(print_unsafe(buffer, width, 24, bpp, message, SCREEN_ERROR_ROW, SCREEN_ERROR_COLUMN, RED_ON_WHITE) == ERROR_OUT_OF_BOUNDARIES) {
        print_unsafe(buffer, width, 24, bpp, ERROR_MESSAGE_OUT_OF_BOUNDARIES, SCREEN_ERROR_ROW, SCREEN_ERROR_COLUMN, RED_ON_WHITE);
    }
}

int print_all(char *message, int row, int col) {
    if(should_be_configured() != 0) return ERROR_UNCONFIGURED;
    return print_unsafe(SCREEN_BUFFER, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BYTE_PER_PIXEL, message, row + SCREEN_START_ROW, col + SCREEN_START_COLUMN, WHITE_ON_BLACK);
}

int print_char(char * buffer, int width, int height, int bytePerPixel, char c, int row, int col, char attr) {
    if (!attr) attr = WHITE_ON_BLACK;
    if(col < 0 || row < 0 || col >= width || row >= height) {
        return ERROR_OUT_OF_BOUNDARIES;
    }

    int offset;
    if (col >= 0 && row >= 0) offset = get_offset(width, bytePerPixel, row, col);
    else offset = get_cursor_offset();

    if(c == '\n') {
        row = get_offset_row(width, bytePerPixel, offset);
        offset = get_offset(width, bytePerPixel, row + SCREEN_BYTE_PER_PIXEL - 1, col);
    } else {
        buffer[offset] = c;
        for(int j = 1; j < SCREEN_BYTE_PER_PIXEL; j++) {
            buffer[ offset * SCREEN_BYTE_PER_PIXEL + j ] = attr;
            break;
        }
        offset += 2;
    }

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
    if(should_be_configured() == 0 && offset >= 0 && offset <= SCREEN_HEIGHT * SCREEN_HEIGHT * SCREEN_BYTE_PER_PIXEL ) {
        offset /= 2;
        port_byte_out(REG_SCREEN_CTRL, 14);
        port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
        port_byte_out(REG_SCREEN_CTRL, 15);
        port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
    }
   
}

int clear_screen() {
    if(should_be_configured() != 0) {
        return ERROR_UNCONFIGURED;
    }
    int screen_size = SCREEN_WIDTH * SCREEN_HEIGHT * SCREEN_BYTE_PER_PIXEL;
    char *screen = SCREEN_BUFFER;
    for(int i = 0; i < screen_size; i++) {
        screen[ i * SCREEN_BYTE_PER_PIXEL ] = ' ';
        for(int j = 1; j < SCREEN_BYTE_PER_PIXEL; j++) {
            screen[ i * SCREEN_BYTE_PER_PIXEL + j ] = WHITE_ON_BLACK;
            break;
        }
    }
    set_cursor_offset(get_offset(SCREEN_WIDTH, SCREEN_BYTE_PER_PIXEL, 0, 0));
    return 0;
}

int get_offset(int width, int bpp, int row, int col) { return bpp * (row * width + col); }
int get_offset_row(int width, int bpp, int offset) { return offset / (bpp * width); }
int get_offset_col(int width, int bpp, int offset) { return (offset - (get_offset_row(width, bpp, offset)*bpp*width))/bpp; }

