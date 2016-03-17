#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdbool.h>
#include <signal.h>

#include "../util/util.h"
#include "ued.h"

#define PRG_ACRO "ued"
#define PRG_NAME "micro editor"
#define PRG_PRNT PRG_ACRO " - " PRG_NAME

static int draw_screen(void);
static int save_cursor_pos(void);
static int set_sig_hand(void);
static int rest_cursor_pos(void);
static int getchar_noecho(void);
static int exec_colon_comm(const char* comm);
static struct winsize  goto_lastrow(void);
static struct winsize  get_winsize(void);

int main(int argc, char **argv)
{
    int ch;

    bool insr_mode = false; // insert mode
    bool edit_mode = false; // edit mode

    if (set_sig_hand() != 0) {}; // just go ahead?
    draw_screen();
    
    while(true) {

        ch = getchar_noecho();
        switch(ch) {
            
        case  8: { // CTRL + H = 1 left
            printf(TLT);
            break;
        }
            
        case 12: { // CTRL + L = 1 right
            printf(TRT);
            break;
        }

        case 10: { // CTRL + J = 1 down
            printf(TDW);
            break;
        }

        case 23: { // CTRL + W = 1 up
            printf(TUP);
            break;
        }

        case  2: { // CTRK + B = erase char before 
            printf(TLT TCLR);
            break;
        }
            
        case  4: { // CTRL + D = erase char at
            printf(TCLR);
            break;
        }

        case  5: {  // CTRL + E = edit mode
            save_cursor_pos();
            goto_lastrow();
            printf("%s", TBLD);
            printf("%s", "-- INSERT --");
            printf("%s", TDEF);
            rest_cursor_pos();
            edit_mode = 1;
            break;
        }

        case 27: { // ESC = quit edit mode
            //save_cursor_pos();
            goto_lastrow();
            printf("\033[2K");
            rest_cursor_pos();
            edit_mode = 0;
            break;
        }
            
        case 127: { // backspace
            printf("\033[1D");
            printf("\033[1P");
            break;
        }
            
        case   9: { // CTRL + I
            if (edit_mode)
                insr_mode = insr_mode ? 0 : 1;
            break;
        }
           
        case  58: { // : (SHIFT + ;)
            char comm[10];
            
            save_cursor_pos();
            goto_lastrow();
            printf("\033[2K");
            printf(": ");

            scanf("%9s", (char*) &comm);
            exec_colon_comm(comm);
            
            break;
        }
            
        default: {
            if (edit_mode) {
                if (insr_mode) { // if insert mode on
                    printf("%c", ch);
                } else {
                    printf("\033[1@");
                    printf("%c", ch);
                }
            }
        }
        }    
    }
    
    return 0;
}

static int
/* thanks to prof bruno silvestre */
getchar_noecho(void)
{
    int ch;
    struct termios oldattr;
    struct termios newattr;
    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
    return ch;
}

static int
draw_screen(void)
{
    int i;
    
    printf("\033[0;0H");
    printf("\033[2J");

    goto_lastrow();
    struct winsize w = get_winsize();
    
    i = w.ws_row;
    while (i != 1) {
        i--;
        printf("%s~%s", TBLU, TDEF);
        printf("\033[%d;%dH", i, 1);
        if (i == w.ws_row / 2) {
            printf("\033[%d;%dH", i - 3, (int) w.ws_col / 2  - (int) strlen(PRG_PRNT) / 2);
            printf("%s", PRG_PRNT);
            printf("\033[%d;%dH", i - 1, (int) w.ws_col / 2  - 1);
            printf(":)");
            printf("\033[%d;%dH", i, 1);
        }
    }

    return 0;
}

static int
exec_colon_comm(const char *comm)
{
    if (strcmp(comm, "quit") == 0 || strcmp(comm, "q") == 0) {
        exit(EXIT_SUCCESS);
    }

    return 0;
}

static struct winsize
get_winsize(void)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w;
}

static struct winsize
goto_lastrow(void)
{
    struct winsize w = get_winsize();
    printf("\033[%d;%dH", w.ws_row, 0);
    return w;
}

static int
save_cursor_pos(void)
{
    return printf("\033[s");
}

static int
rest_cursor_pos(void)
{
    return printf("\033[u");
}

static void
sig_hand(int sig_num)
{
    if (sig_num == SIGINT) {
        save_cursor_pos();
        goto_lastrow();
        printf("\033[2K");            
        printf("Type :quit<Enter> to exit %s", PRG_NAME);
        rest_cursor_pos();
    }
}

static int
set_sig_hand(void)
{
    struct sigaction sa;
    sa.sa_handler = sig_hand;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    return sigaction(SIGINT, &sa, NULL);
}
