#include "../drivers/screen.h" 
#include "./util.h"

int main() {
    CHECKING(configure_screen(DEFAULT_VIDEO_ADDRESS, MAX_COLS, MAX_ROWS, DEFAULT_START_ROW, DEFAULT_START_COLUMN, 2));
    
    CHECKING(clear_screen());
    
    // test scroll
   
    for (int i = 0; i < 23; i++) {
        char str[255];
        int_to_ascii(i, str);
        kprint_at(str, i, 0);
    }

   // kprint_at("This text forces the kernel to scroll. Row 0 will disappear. ", 23, 60);
   // kprint("And with this text, the kernel will scroll again, and row 1 will disappear too!");

    

    /*
    //test error message
    print_error("error happened"); 
    */

    /*
    //test reverse
     char str[] = "Hello world";
    reverse_substring(str, 6, 5);
    kprint( str ) ; 
    
    */
   
   /*
   // test int_to_ascii
    for(int i = -2; i < 15; i++) {

        char tmp[5];
        int_to_ascii(i, tmp); 
        kprint_at( tmp, i + 2, 0 ) ; 
    }
   */

    /*
    //test print
    kprint_at( "Hallo world", 10, 10) ;
    kprint_at( "this is \n two lines", 11, 12) ;
    kprint_at("This text spans multiple lines", 10, 75);
    kprint_at("What happens when we run out of space?", 23, 45);
    */

    /*
    char* video_memory = (char*) 0xb8000;
    *video_memory = 'X';
     port_byte_out(0x3d4, 14);
   int position = port_byte_in(0x3d5);
   position = position << 8;

   port_byte_out(0x3d4, 15);
   position +=  port_byte_in(0x3d5);

   int offset_from_vga = position * 2;
   char *vga = 0xb8000;
   vga[offset_from_vga] = 'X';
   vga[offset_from_vga + 1] = 0x0f;
    */
  

}