/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_termctl.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/01/07 16:59:21 by mwelsch           #+#    #+#             */
/*   Updated: 2014/01/07 23:05:11 by mwelsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <libft_printf.h>
#include <libft_memory.h>
#include "ft_termctl.h"
#include "ft_curses.h"

struct sigaction		g_terminal_act;

int			ft_terminal_initialised(t_terminal *term)
{
	return (term ? (term->descriptor != -1 && term->init) : 0);
}

/*
** Which standard stream is connected to our TTY?
*/
int			ft_terminal_find_stream(int *descriptor)
{
#ifdef _DEBUG
	ft_printf(".\n");
#endif
	if (isatty(STDERR_FILENO))
		*descriptor = STDERR_FILENO;
	else
		if (isatty(STDIN_FILENO))
			*descriptor = STDIN_FILENO;
		else
			if (isatty(STDOUT_FILENO))
				*descriptor = STDOUT_FILENO;
			else
				return (errno = ENOTTY);
	return (errno = 0);
}

/*
** Disable buffering for terminal streams.
*/
void		ft_terminal_disable_buffering(t_terminal *term)
{
#ifdef _DEBUG
	ft_printf("Disabling buffer mode for terminal (stream %d).\n",
			  term->descriptor);
#else
	(void)term;
#endif
	if (term->descriptor == STDIN_FILENO)
		setvbuf(stdin, NULL, _IONBF, 0);
	if (term->descriptor == STDOUT_FILENO)
		setvbuf(stdout, NULL, _IONBF, 0);
	if (term->descriptor == STDERR_FILENO)
		setvbuf(stderr, NULL, _IONBF, 0);
}
	/* Set the new terminal settings.
	 * Note that we don't actually check which ones were successfully
	 * set and which not, because there isn't much we can do about it. */
int			ft_terminal_update(t_terminal *term)
{
#ifdef _DEBUG
	ft_printf("Updating terminal.\n");
#endif
	if (tcsetattr(term->descriptor, TCSANOW, &term->settings.current) == -1)
		return (errno);
	return (errno = EXIT_SUCCESS);
}

int			ft_terminal_restore(t_terminal *term)
{
#ifdef _DEBUG
	ft_printf("Retoring terminal.\n");
#endif
	if (tcsetattr(term->descriptor, TCSANOW, &term->settings.original) == -1)
		return (errno);
	if (tcgetattr(term->descriptor, &term->settings.current) == -1)
		return (errno);
	return (errno = EXIT_SUCCESS);
}

/*
** Obtain terminal settings.
*/
int			ft_terminal_capture(t_terminal *term)
{
	int				c;

#ifdef _DEBUG
	ft_printf("Capturing terminal settings.\n");
#endif
	if ((c = ft_terminal_find_stream(&term->descriptor)))
		return (c);
	if ((tcgetattr(term->descriptor, &term->settings.original) == -1) ||
		(tcgetattr(term->descriptor, &term->settings.current) == -1))
		return (errno = ENOTSUP);
	return (errno = 0);
}

int			ft_terminal_prepare_raw(t_terminal *term)
{
#ifdef _DEBUG
	ft_printf("Preparing raw mode for terminal.\n");
#endif
	if (!term)
		return (EXIT_FAILURE);
	BREAK_ON_SIGINT(term->settings.current);
	IGNORE_INPUT_PAR(term->settings.current);
	NO_STRIP8(term->settings.current);
	NO_NL_TRANSLATION(term->settings.current);
#ifdef IUCLC
	NO_UCLC(term->settings.current);
#endif
	USE_8BIT_CHARS(term->settings.current);
	ENABLE_RECEIVER(term->settings.current);
	GENERATE_SIGNALS(term->settings.current);
	SET_NONCANONICAL(term->settings.current);
	DISABLE_ECHO(term->settings.current);
	DISABLE_INPUT_PROC(term->settings.current);
	SET_READBLOCK(term->settings.current, 0, 1);
	return (EXIT_SUCCESS);
}

int			ft_terminal_set_signals(t_terminal_signal sig_func)
{
	signal(SIGHUP, sig_func);
	return (EXIT_SUCCESS);
}

int			ft_terminal_init(t_terminal *term,
							 t_terminal_atexit atexit_func,
							 t_terminal_signal sig_func)
{
	int		c;

	c = 0;
	if (ft_terminal_initialised(term))
		return (EXIT_SUCCESS);
	ft_terminal_prepare_raw(term);
	if ((c = ft_terminal_capture(term)))
		return (c);
	ft_terminal_disable_buffering(term);
	/* At exit() or return from main(),
	 * restore the original settings. */
	if ((c = ft_terminal_set_signals(sig_func)))
		return (c);
	term->atexit = atexit_func;
	if (atexit(atexit_func ? atexit_func : ft_terminal_atexit))
		return (errno = ENOTSUP);

	if ((c = ft_terminal_update(term)))
		_exit(EXIT_FAILURE);
	term->init = 1;
	return (errno = 0);
}

t_terminal	*ft_terminal_create(int init_now,
								t_terminal_atexit atexit_func,
								t_terminal_signal sig_handler)
{
	t_terminal	*term;

	term = ft_memalloc(sizeof(t_terminal));
	if (!term)
		return (NULL);
	term->init = 0;
	term->descriptor = -1;
	term->atexit = atexit_func;
	if (init_now)
		ft_terminal_init(term, atexit_func, sig_handler);
	return (term);
}

void		ft_terminal_destroy(t_terminal **term)
{
	if (term && *term)
	{
		(*term)->init = 0;
		ft_memdel((void**)term);
	}
}
