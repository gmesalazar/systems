#include <sys/ioctl.h>
#include <termios.h>
#include <stdbool.h>
#include <signal.h>
#include <stdio.h>

#include "util.h"
#include "ued.h"

static int ued_draw_screen(void);
static int ued_ctrl_loop(void);
static int ued_save_cursor_pos(void);
static int ued_set_sig_hand(void);
static int ued_rest_cursor_pos(void);
static int ued_get_keycode(void);
static int ued_enter_edit_mode(void);
static int ued_exit_edit_mode(void);
static int ued_enter_cmd_mode(void);
static int ued_exec_colon_comm(const char*);

static void ued_get_winsize(struct winsize*);
static void ued_goto_lastrow(void);

static bool ued_edit_mode = false; // edit mode

int main(int argc, char **argv)
{
	(void) argc;
	(void) argv;

	if (ued_set_sig_hand() != 0) {} // just go ahead?

	ued_draw_screen();
	ued_ctrl_loop();

    return 0;
}

static int
ued_ctrl_loop(void)
{
	int ch;

	while(true) {

		ch = ued_get_keycode();
		switch(ch) {

		case 68: { // CTRL + H = 1 left
			printf(TLT);
			break;
		}

		case 67: { // CTRL + L = 1 right
			printf(TRT);
			break;
		}

		case 66: { // CTRL + J = 1 down
			printf(TDW);
			break;
		}

		case 65: { // CTRL + W = 1 up
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
			ued_enter_edit_mode();
			break;
		}

		case 27: { // ESC = quit edit mode
			ued_exit_edit_mode();
			break;
		}

		case 127: { // backspace
			printf(TLT TCLR);
			break;
		}

		case 58: { // SHIFT + ; enter command mode
			ued_enter_cmd_mode();
			break;
		}

		default: {
			if (ued_edit_mode)
				printf("%c", ch);
		}
		}
    }

	return 0;
}

static int
ued_get_keycode(void)
{
	int ch;
	struct termios oldattr;
	struct termios newattr;

	tcgetattr(STDIN_FILENO, &oldattr);
	newattr = oldattr;
	newattr.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newattr);

	ch = getchar();

	switch(ch) {
	case 27: 
		if ((ch = ued_get_keycode()) == 91)
			ch = ued_get_keycode();
		break;
	}

	tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
	return ch;
}

static int
ued_draw_screen(void)
{
	int i;
	struct winsize w;

	printf("\033[0;0H");
	printf("\033[2J");

	ued_goto_lastrow();
	ued_get_winsize(&w);

	i = w.ws_row;
	while (i != 1) {
		i--;
		printf("%s~%s", TBLU, TDEF);
		printf("\033[%d;%dH", i, 1);
		if (i == w.ws_row / 2) {
			printf("\033[%d;%dH", i - 3, (int) w.ws_col / 2  - (int) \
			        strlen(PRG_PRNT) / 2);
			printf("%s", PRG_PRNT);
			printf("\033[%d;%dH", i - 1, (int) w.ws_col / 2  - 1);
			printf(":)");
			printf("\033[%d;%dH", i, 1);
		}
	}

	return 0;
}

static int
ued_exec_colon_comm(const char *comm)
{
	if (strcmp(comm, "quit") == 0 || strcmp(comm, "q") == 0) {
		exit(EXIT_SUCCESS);
	}

	return 0;
}

static void
ued_get_winsize(struct winsize* w)
{
	ioctl(STDOUT_FILENO, TIOCGWINSZ, w);
}

static void
ued_goto_lastrow(void)
{
	struct winsize w;
	ued_get_winsize(&w);
	printf("\033[%d;%dH", w.ws_row, 0);
}

static int
ued_save_cursor_pos(void)
{
	return printf("\033[s");
}

static int
ued_enter_edit_mode(void)
{
	ued_save_cursor_pos();
	ued_goto_lastrow();
	printf(TBLD);
	printf("-- INSERT --");
	printf(TDEF);
	ued_rest_cursor_pos();
	ued_edit_mode = true;

	return 0;
}

static int
ued_exit_edit_mode(void)
{
	ued_goto_lastrow();
	printf("\033[2K");
	ued_rest_cursor_pos();
	ued_edit_mode = false;

	return 0;
}

static int
ued_enter_cmd_mode(void)
{
	char comm[10];

	ued_save_cursor_pos();
	ued_goto_lastrow();

	printf("\033[2K");
	printf(":");

	scanf("%9s", (char*) &comm);
	ued_exec_colon_comm(comm);

	return 0;
}

static int
ued_rest_cursor_pos(void)
{
	return printf("\033[u");
}

static void
ued_sig_hand(int sig_num)
{
	if (sig_num == SIGINT) {
		ued_save_cursor_pos();
		ued_goto_lastrow();
		printf("\033[2K");            
		printf("Type :quit<Enter> to exit %s", PRG_NAME);
		ued_rest_cursor_pos();
	}
}

static int
ued_set_sig_hand(void)
{
	struct sigaction sa;

	sa.sa_handler = ued_sig_hand;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	return sigaction(SIGINT, &sa, NULL);
}
