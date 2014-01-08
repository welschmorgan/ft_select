#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define KEYCODE_R 0x43
#define KEYCODE_L 0x44
#define KEYCODE_U 0x41
#define KEYCODE_D 0x42

int getch();
int main()
{
    int c;

    printf("\nPRESS A ARROW\n");
    while(1)
    {
        c = getch();
        printf("\nPRINT OF DEBUG\n");
        switch(c)
        {
		case KEYCODE_L:
			printf("\nLEFT\n");
            break;
		case KEYCODE_R:
			printf("\nRIGHT\n");
            break;
		case KEYCODE_U:
			printf("\nFORWARD\n");
            break;
		case KEYCODE_D:
			printf("\nINDIETRO\n");
            break;
        }
    }
    return 0;
}

int getch() 
{
    char c;
    struct termios before,after;
    tcgetattr (STDIN_FILENO, &before);
/*
** STDIN_FILENO e' il File Descriptor dello stdin.
** Salviamo in before la configurazione attuale dello stdin
*/
    memcpy(&after, &before, sizeof(struct termios));
    after.c_lflag &=~ (ICANON | ECHO);
/*
** modifichiamo la configurazione dello stadin modificando
** il flag c_lflag con degli or e and bit a bit
*/
    after.c_cc[VMIN] = 1;


    after.c_cc[VTIME] = 0; 
    tcsetattr (STDIN_FILENO, TCSANOW, &after);
/*
** qui effettuiamo effettivamente le modifiche
*/
    if (read (STDIN_FILENO, &c, sizeof(char)) == -1)
/*
** read e' una system call, leggiamo dallo stdin
*/
    {
        perror("\nErrore in lettura\nIl programma termina");
        tcsetattr (STDIN_FILENO, TCSANOW, &before);
        exit(-1);
    }

    tcsetattr (STDIN_FILENO, TCSANOW, &before);
/*
** si ripristina la configurazione dello stdin
*/
    return c;
}
