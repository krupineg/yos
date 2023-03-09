#define DEFAULT_VIDEO_ADDRESS (char *)0xb8000
#define DEFAULT_START_ROW 1
#define DEFAULT_START_COLUMN 0
#define MAX_ROWS 24
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0f
#define RED_ON_WHITE 0xf4

/* Screen i/o ports */
#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

#define ERROR_UNABLE_TO_CONFIGURE -999
#define ERROR_MESSAGE_UNABLE_TO_CONFIGURE "Error: unable to configure"
#define ERROR_UNCONFIGURED -1000
#define ERROR_MESSAGE_UNCONFIGURED "Error: the screen is not configured"
#define ERROR_OUT_OF_BOUNDARIES -1001
#define ERROR_MESSAGE_OUT_OF_BOUNDARIES "Error: out of boundaries"
#define UNKNOWN_RESULT_CODE "Error: unknown result code"
#define CHECKING(CODE) \
    ({ int err = CODE; switch(err){ \
        case 0:     \
            break;      \
        case ERROR_UNCONFIGURED:           \
            print_error(ERROR_MESSAGE_UNCONFIGURED); return err;   \
        case ERROR_UNABLE_TO_CONFIGURE:       \
            print_error(ERROR_MESSAGE_UNABLE_TO_CONFIGURE); return err;  \
        case ERROR_OUT_OF_BOUNDARIES:       \
            print_error(ERROR_MESSAGE_OUT_OF_BOUNDARIES); return err;   \
        default: print_error(UNKNOWN_RESULT_CODE); return err; \
        }; })

/* Public kernel API */
int clear_screen();
int kprint_at(char *message, int col, int row);
int kprint(char *message);
void print_error(char *message);
int configure_screen(char * buffer, int width, int height, int startRow, int startColumn, int pixel);