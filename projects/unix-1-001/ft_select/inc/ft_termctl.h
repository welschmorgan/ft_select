/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_termctl.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/01/07 21:57:39 by mwelsch           #+#    #+#             */
/*   Updated: 2014/01/07 22:53:50 by mwelsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_TERMCTL_H
# define FT_TERMCTL_H

#include <termios.h>

struct					termios;
typedef void (*t_terminal_atexit)(void);
typedef void (*t_terminal_signal)(int);

typedef struct			s_termsettings
{
	struct termios		original;
	struct termios		current;
}						t_termsettings;

typedef struct			s_terminal
{
	int					descriptor;
	t_termsettings		settings;
	t_terminal_atexit	atexit;
	int					init;
}						t_terminal;

void		ft_terminal_atexit(void);
void		ft_terminal_signal(int signum);

/*
** Which standard stream is connected to our TTY?
*/
int			ft_terminal_find_stream(int *descriptor);

/*
** Disable buffering for terminal streams.
*/
void		ft_terminal_disable_buffering(t_terminal *term);

/* Set the new terminal settings.
 * Note that we don't actually check which ones were successfully
 * set and which not, because there isn't much we can do about it. */
int			ft_terminal_update(t_terminal *term);

/*
** Restores terminal settings.
*/
int			ft_terminal_restore(t_terminal *term);

/*
** Backups terminal settings.
*/
int			ft_terminal_capture(t_terminal *term);

/*
** Sets flags needed to setup the supplied terminal in raw mode
*/
int			ft_terminal_prepare_raw(t_terminal *term);

/*
** Creates a new terminal object
*/
t_terminal	*ft_terminal_create(int init_now,
								t_terminal_atexit atexit_func,
								t_terminal_signal signal_func);

/*
** Destroys a previously allocated terminal object
*/
void		ft_terminal_destroy(t_terminal **term);

/*
** Initialize terminal for non-canonical, non-echo mode,
** that should be compatible with standard C I/O.
** Returns 0 if success, nonzero errno otherwise.
*/
int			ft_terminal_init(t_terminal *term,
							 t_terminal_atexit atexit_func,
							 t_terminal_signal sig_func);

/*
** Returns true if terminal has already been initialised
*/
int		ft_terminal_initialised(t_terminal *term);
/*
** Restore terminal to original settings
*/
void	ft_terminal_done(void);

/*
** "Default" signal handler: restore terminal, then exit.
** exit() is not async-signal safe, but _exit() is.
** Use the common idiom of 128 + signal number for signal exits.
** Alternative approach is to reset the signal to default handler,
** and immediately raise() it.
*/
void	ft_terminal_signal(int signum);

/*
** Let BREAK cause a SIGINT in input.
*/
# define BREAK_ON_SIGINT(var) var.c_iflag &= ~IGNBRK; var.c_iflag |= BRKINT;
/*
** Ignore framing and parity errors in input.
*/
# define IGNORE_INPUT_PAR(var) var.c_iflag &= ~PARMRK; var.c_iflag |= IGNPAR;
/*
** Do not strip eighth bit on input.
*/
# define NO_STRIP8(var) var.c_iflag &= ~ISTRIP;
/*
** Do not do newline translation on input.
*/
# define NO_NL_TRANSLATION(var) var.c_iflag &= ~(INLCR | IGNCR | ICRNL);
/*
** Do not do uppercase-to-lowercase mapping on input.
*/
# ifdef IUCLC
#  define NO_UCLC(var) var.c_iflag &= ~IUCLC;
# endif
/*
** Use 8-bit characters. This too may affect standard streams,
** but any sane C library can deal with 8-bit characters.
*/
# define USE_8BIT_CHARS(var) var.c_iflag &= ~CSIZE; var.c_iflag |= CS8;
/*
** Enable receiver.
*/
# define ENABLE_RECEIVER(var) var.c_iflag |= CREAD;
/*
** Let INTR/QUIT/SUSP/DSUSP generate the corresponding signals.
*/
# define GENERATE_SIGNALS(var) var.c_lflag |= ISIG;
/*
** Enable noncanonical mode.
** This is the most important bit, as it disables line buffering etc.
*/
# define SET_NONCANONICAL(var) var.c_lflag &= ~ICANON;
/*
** Disable echoing input characters.
*/
# define DISABLE_ECHO(var) var.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL);
/*
** Disable implementation-defined input processing.
*/
# define DISABLE_INPUT_PROC(var) var.c_lflag &= ~IEXTEN;
/*
** To maintain best compatibility with normal behaviour of terminals,
** we set TIME=0 and MAX=1 in noncanonical mode. This means that
** read() will block until at least one byte is available.
*/
# define SET_TIME(var, time) var.c_cc[VTIME] = time;
# define SET_MIN(var, min) var.c_cc[VMIN] = min;
# define SET_READBLOCK(var, time, min) SET_TIME(var, time); SET_MIN(var, min);

#endif /* !FT_TERMCTL */
