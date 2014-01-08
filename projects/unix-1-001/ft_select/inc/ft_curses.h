/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_curses.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/01/07 16:45:59 by mwelsch           #+#    #+#             */
/*   Updated: 2014/01/07 18:07:30 by mwelsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef FT_CURSES_H
# define FT_CURSES_H

# define USE_TERMCAP

# ifdef USE_TERMCAP
#  define FT_TGETSTR_BUFSZ 30
char		*ft_tcgetstr(char *id);
# endif

int			ft_tcurshide(void);
int			ft_tcurshow(void);

int			ft_clrscr(void);
int			ft_clrln(void);
int			ft_clrrest(void);
int			ft_clrlnend(void);

int			ft_underline(const char *str);
int			ft_reverse(const char *str);

int			ft_bold(const char *str);
int			ft_normal(void);

#endif /* !FT_CURSES_H */
