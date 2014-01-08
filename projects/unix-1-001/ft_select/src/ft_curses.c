/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_curses.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/01/07 16:44:18 by mwelsch           #+#    #+#             */
/*   Updated: 2014/01/07 18:07:17 by mwelsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <term.h>
#include <libft_printf.h>
#include "ft_curses.h"

#ifdef USE_TERMCAP
char		*ft_tcgetstr(char *id)
{
	static char			buf[FT_TGETSTR_BUFSZ];
	char				*p;

	p = buf;
	return (tgetstr(id, &p));
}
#endif

int			ft_tcurshide(void)
{
#ifdef USE_TERMCAP
	return (ft_printf("%s", ft_tcgetstr("vi")));
#else
	return (ft_printf("\033[?25l"));
#endif
}

int			ft_tcurshow(void)
{
#ifdef USE_TERMCAP
	return (ft_printf("%s", ft_tcgetstr("vs")));
#else
	return (ft_printf("\033[?25h"));
#endif
}

int			ft_clrscr(void)
{
#ifdef USE_TERMCAP
	return (ft_printf("%s", ft_tcgetstr("cl")));
#else
	return (ft_printf("\033[2J\033[1;1H"));
#endif
}

int			ft_clrln(void)
{
#ifdef USE_TERMCAP
	return (ft_printf("%s", ft_tcgetstr("dl")));
#else
	return (ft_printf("\033[2K"));
#endif
}

int			ft_clrrest(void)
{
#ifdef USE_TERMCAP
	return (ft_printf("%s", ft_tcgetstr("cd")));
#else
	return (ft_printf("\033[J"));
#endif
}

int			ft_clrlnend(void)
{
#ifdef USE_TERMCAP
	return (ft_printf("%s", ft_tcgetstr("ce")));
#else
	return (ft_printf("%s: clear line end without termcap deactivated.\n",
					  __FUNCTION__));
#endif
}

int			ft_underline(const char *str)
{
#ifdef USE_TERMCAP
	return (ft_printf("%s%s%s", ft_tcgetstr("us"), str, ft_tcgetstr("ue")));
#else
	return (ft_printf("\033[3;37;40m%s\033[0m",str));
#endif
}

int			ft_reverse(const char *str)
{
#ifdef USE_TERMCAP
	return (ft_printf("%s%s%s", ft_tcgetstr("mr"), str, ft_tcgetstr("me")));
#else
	return (ft_printf("%c[5m%s%c[0m", 27, str, 27));
#endif
}


int			ft_bold(const char *str)
{
#ifdef USE_TERMCAP
	return (ft_printf("%s%s%s", ft_tcgetstr("md"), str, ft_tcgetstr("me")));
#else
	return (printf("%c[1m%s%c[0m", 27, str, 27));
#endif
}

int			ft_normal(void)
{
#ifdef USE_TERMCAP
	return (ft_printf("%s", ft_tcgetstr("me")));
#else
	return (printf("%c[m", 27));
#endif
}
