/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/01/07 22:00:00 by mwelsch           #+#    #+#             */
/*   Updated: 2014/01/07 23:09:01 by mwelsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <libft.h>
#include "ft_termctl.h"
#include "ft_curses.h"

#define VK_NULL 0x0
#define VK_SOH 0x01
#define VK_STX 0x02
#define VK_ETX 0x03
#define VK_EOT 0x04
#define VK_ENQ 0x04
#define VK_ACK 0x06
#define VK_BEL 0x07
#define VK_BACKSPACE 0x08
#define VK_TAB 0x09
#define VK_LF 0x10
#define VK_CR 0x13
#define VK_NACK 0x15
#define VK_SYN 0x16
#define VK_CANCEL 0x18
#define VK_CLRLINE 0x21
#define VK_SPACE 0x20

#define VK_RIGHT 0x43
#define VK_LEFT 0x44
#define VK_UP 0x41
#define VK_DDOWN 0x42
#define VK_ESCAPE 0x27
#define VK_VTAB 0x0B
#define VK_RETURN 0x0d
#define VK_DELETE 0x7F

char		*tgetstr(char *id, char **area);
/*
** `mb'
String of commands to enter blinking mode.
`md'
String of commands to enter double-bright mode.
`mh'
String of commands to enter half-bright mode.
`mk'
String of commands to enter invisible mode.
`mp'
String of commands to enter protected mode.
`mr'
String of commands to enter reverse-video mode.
`me'
String of commands to turn off all appearance modes
 */

static t_list			*g_app_args = NULL;
static t_terminal		*g_terminal = NULL;

int				parse_app_args(int ac, char **argv)
{
	char		*ptr;
	int			i;

	if (g_app_args)
		return (EXIT_SUCCESS);
	if (!argv || !*argv)
		return (EXIT_FAILURE);
	g_app_args = ft_lstnew(NULL, 0);
	i = 1;
	while (i < ac && argv[i])
	{
		ptr = argv[i];
		ft_printf("Adding arg '%s'\n", ptr);
		ft_lstadd(&g_app_args,
				  ft_lstnew(ptr, ft_strlen(ptr)));
		i ++;
	}
	return (EXIT_SUCCESS);
}

static void		ft_printkey(char c)
{
	if (c >= 33 && c <= 126)
		printf("0x%02x = 0%03o = %3d = '%c'\n", c, c, c, c);
	else
		printf("0x%02x = 0%03o = %3d\n", c, c, c);
}

static void		ft_printsel(int c, char **v, int id)
{
	int			i;

	i = 1;
	(void)id;
	while (i < c)
	{
		ft_printf("%d: %s%s", i, v[i], ((i == (c - 1)) ? "\n" : ""));
		i ++;
	}
}

int ft_getttysize(struct winsize *size)
{
	if (ioctl(STDIN_FILENO,TIOCGWINSZ, (char*)size) < 0)
	{
		ft_error ("Erreur TIOCGEWINSZ\n");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

void	restore_terminal(void)
{
	if (ft_terminal_initialised(g_terminal))
	{
		ft_printf("Restauring terminal state ...\n");
		ft_terminal_restore(g_terminal);
	}
}

void		signal_terminal(int signal)
{
	char	signal_name[20];

	if (signal == SIGHUP)
		ft_strncpy(signal_name, "SIGHUP", 20);
	else if (signal == SIGUSR1)
		ft_strncpy(signal_name, "SIGUSR1", 20);
	else if (signal == SIGINT)
	{
		printf("Caught SIGINT, exiting now\n");
		ft_terminal_restore(g_terminal);
	}
	else
		fprintf(stderr, "Caught wrong signal: %d\n", signal);
	ft_printf("Caught '%s'.\n", signal_name);
	_exit(128 + signum);
}

int		cleanup(void)
{
	if (g_terminal)
		ft_terminal_destroy(g_terminal);
	return (EXIT_SUCCESS);
}

int		init(int arg_count, char **arg_list)
{
	if (g_terminal)
		return (EXIT_SUCCESS);
	if (arg_count <=1)
	{
		printf("usage: %s [arg0] [arg1] [arg2] ...\n",
			   arg_list ? arg_list[0] : NULL);
		return (EXIT_FAILURE);
	}
	if (parse_app_args(arg_count, arg_list))
		return (EXIT_FAILURE);
	if ((g_terminal = ft_terminal_create(1, restore_terminal, signal_terminal)))
	{
		if (errno == ENOTTY)
			ft_error("This program requires a terminal.\n");
		else
			ft_error("Cannot initialize terminal: %s.\n",
					strerror(errno));
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int			mainloop(int ac, char **av)
{
	int				c;
	int				i;
	struct winsize	size;

	ft_getttysize(&size);
#ifdef _DEBUG
	ft_printf("Your terminal size is %dx%d (cols * rows)\n"
			  "\t%dx%d pixels\n",
			  size.ws_col, size.ws_row,
			  size.ws_xpixel, size.ws_ypixel);
#endif
	i = 1;
	ft_printsel(ac, av, i);
	while ((c = read(STDIN_FILENO, &c, 1)) != EOF) {
		ft_clrscr();
		if (c == VK_RIGHT)
		{
			if (i < (ac - 1))
				i ++;
			else
				i = 1;
		}
		else if (c == VK_LEFT)
		{
			if (i > 1)
				i --;
			else
				i = ac - 1;
		}
		else if (c == VK_SPACE || c == VK_RETURN)
			printf("Sending '%s' to shell.\n", av[i]);
		else if (c == VK_DELETE || c == VK_BACKSPACE)
			printf("Deleting '%s'.\n", av[i]);
		else
			ft_printkey(c);
		if (c == VK_ETX || c == VK_EOT || c == VK_ESCAPE)
			break ;
		ft_printsel(ac, av, i);
	}
	return (EXIT_SUCCESS);
}

int main(int ac, char **av)
{
	int				rc;

	if ((rc = init(ac, av)))
		_exit(rc);
	mainloop(ac, av);
	cleanup();
	return (EXIT_SUCCESS);
}

/*
**	printf("Press CTRL+C or Q to quit.\n");
**	while ((c = getc(stdin)) != EOF) {
**		ft_clearscreen();
**		if (c == 3 || c == 'Q' || c == 'q')
**			break;
*/
