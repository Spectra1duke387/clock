#include <curses.h>
#include <getopt.h>
#include <locale.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "clock.h"

#define COLOR_SET 1

// TODO: getopt_long stuff :-)
void usage(void)
{
	printf("Usage: clock [-h] [-c=color] [--version]");
	printf(" [--help] [--hide-exit-text]\n\n");

	printf("clock is an ASCII art clock designed to be a part");
	printf(" of a terminal multiplex setup.\n");
	printf("The clock displayed is always printed");
	printf(" to the center of a window. Keep in \n");
	printf("mind that the clock can be up to 1 second");
	printf(" off and the time displayed is that\n");
	printf("of your local timezone. Different timezones");
	printf(" are to be added at a later date.\n\n");
	printf("commands:\n");
	printf("\t-h\t--help\t\tprint this information\n");
	printf("\t-c [color]\t\tset clock to different color\n");
	printf("\t\tred\n");
	printf("\t\tgreen\n");
	printf("\t\tyellow\n");
	printf("\t\tblue\n");
	printf("\t\tmagenta\n");
	printf("\t\tcyan\n");
	printf("\t\twhite (default)\n");
	printf("\t--version\t\tprint current version (0.9.1)\n");
	printf("\t--hide-exit-text\thide text info");
	printf(" at the bottom of the screen\n");
}

int main(int argc, char **argv)
{
	// for time	
	time_t rawtime;
	struct tm *timeinfo;

	// for ioctl
	struct winsize w;

	char c;
	int color = COLOR_WHITE;
	while ((c = getopt(argc, argv, "c:h")) != -1)
		switch (c) {
		case 'c':

			if (strncmp(optarg, "red", 3) == 0)
				color = COLOR_RED;
			else if (strncmp(optarg, "green", 5) == 0)
				color = COLOR_GREEN;
			else if (strncmp(optarg, "yellow", 6) == 0)
				color = COLOR_YELLOW;
			else if (strncmp(optarg, "blue", 4) == 0)
				color = COLOR_BLUE;
			else if (strncmp(optarg, "magenta", 7) == 0)
				color = COLOR_MAGENTA;
			else if (strncmp(optarg, "cyan", 4) == 0)
				color = COLOR_CYAN; 
			else if (strncmp(optarg, "white", 5) == 0)
				color = COLOR_WHITE;	
			else {
				fprintf(stderr,
					"Please input a valid color\n");
				usage();
				exit(EXIT_FAILURE);
			}

			break;
		case 'h': 
			usage();
			break;
		case '?':
			usage();
			exit(EXIT_FAILURE);
		}

	// For ncurses
	setlocale(LC_ALL, "");
	initscr();
	halfdelay(2);
	keypad(stdscr, TRUE);	/* For exit keys (maybe color nav later :-) */
	noecho();

	char ch;

	if (!has_colors()) {
		endwin();
		printf("sorry :-( please enable colors\n");
	}

	start_color();
	init_pair(COLOR_SET, color, COLOR_BLACK);
	attron(COLOR_PAIR(COLOR_SET));

	while ((ch = getch()) != 'q') {
		erase();

		time(&rawtime);
		timeinfo = localtime(&rawtime);

		ioctl(0, TIOCGWINSZ, &w);

		if (timeinfo -> tm_hour == 0)
			timeinfo -> tm_hour = 12;
		else if (timeinfo -> tm_hour > 12)
			timeinfo -> tm_hour -= 12;

		
		for (int i = 0; i < art_length; i++) {
			move(w.ws_row / 2 - (4 - i), w.ws_col / 2 - 21);

			addwstr(timeinfo -> tm_hour < 10 ?
						ascii_nums[0][i] : 
							ascii_nums[(timeinfo -> tm_hour) / 10][i]);
			addstr(" ");
			addwstr(ascii_nums[timeinfo -> tm_hour % 10][i]);
			addwstr(ascii_nums[COLON][i]);
			addwstr(ascii_nums[(timeinfo -> tm_min) / 10][i]);
			addstr(" ");
			addwstr(ascii_nums[(timeinfo -> tm_min) % 10][i]);
			addstr("\n");
		}

		refresh();
		nanosleep((const struct timespec[]){{0, 500000000L}}, NULL);
	}
	endwin();
}
