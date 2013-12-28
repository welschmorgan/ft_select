#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2013/12/28 07:00:51 by mwelsch           #+#    #+#              #
#    Updated: 2013/12/28 09:01:36 by mwelsch          ###   ########.fr        #
#                                                                              #
#******************************************************************************#
define	ft_make =
	echo "Making library $2 ($1)"
	make $1 -C $2
endef

LIBRARIES = libmath libopt libft
PROJECTS = corewar pushswap ft_sh1

all: build_libraries build_projects
clean: clean_libraries clean_projects
fclean: fclean_libraries fclean_projects
re: fclean all

build_libraries: $(LIBRARIES)
	for f in $^; do make all -C $$f; done

clean_libraries:
	for f in $(LIBRARIES); do make clean -C $$f; done

fclean_libraries:
	for f in $(LIBRARIES); do make fclean -C $$f; done

build_projects: $(PROJECTS)
	for f in $^; do make all -C $$f; done

clean_projects:
	for f in $(PROJECTS); do make clean -C $$f; done

fclean_projects:
	for f in $(PROJECTS); do make fclean -C $$f; done

.PHONY: all build_libraries build_projects