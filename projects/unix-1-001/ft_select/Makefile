#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2013/11/19 14:02:18 by mwelsch           #+#    #+#              #
#    Updated: 2014/01/08 05:43:52 by mwelsch          ###   ########.fr        #
#                                                                              #
#******************************************************************************#
NAME = ft_select
NAME_D = $(NAME)_d
EXT =
TARGET = $(NAME)
TARGET_D = $(NAME_D)
PLATFORM = $(shell uname -s && uname -m)
INC_DIR = ./inc
SRC_DIR = ./src
OBJ_DIR = ./obj
OBJ_DEBUG_DIR = $(OBJ_DIR)/debug
OBJ_RELEASE_DIR = $(OBJ_DIR)/release

LIBFT = ../../algo-1-001/libft
UNITS = $(shell ls $(SRC_DIR)| grep .c)
UNITS_O = $(UNITS:.c=.o)

SRCS = $(patsubst %,$(SRC_DIR)/%,$(UNITS))
OBJS = $(patsubst %,$(OBJ_RELEASE_DIR)/%,$(UNITS_O))
OBJS_D = $(patsubst %,$(OBJ_DEBUG_DIR)/%,$(UNITS_O))

FLAGS = -Wall -Wextra -Werror -std=c89 -ansi -pedantic \
		-Qunused-arguments -D_POSIX_SOURCE

CFLAGS = $(FLAGS)
CFLAGS_D = $(FLAGS) -ggdb -D_DEBUG
LDFLAGS = -L$(LIBFT) -lft
LDFLAGS_D = -L$(LIBFT) -lft_d

CC = clang $(CFLAGS)
CC_D = clang $(CFLAGS_D)
LD = $(CC)
LD_D = $(CC_D)
RM = /bin/rm -f

all: debug release

debug: $(TARGET_D)
release: $(TARGET)

$(TARGET): $(OBJS)
	$(LD) -o $@ $^ $(LDFLAGS)

$(TARGET_D): $(OBJS_D)
	$(LD_D) -o $@ $^ $(LDFLAGS_D)

$(OBJ_DEBUG_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC_D) -c -o $@ $< -I$(LIBFT)/inc -I$(INC_DIR)

$(OBJ_RELEASE_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c -o $@ $< -I$(LIBFT)/inc -I$(INC_DIR)


clean:
	$(RM) $(OBJS)
	$(RM) $(OBJS_D)

fclean : clean
	$(RM) $(TARGET)
	$(RM) $(TARGET_D)

re: fclean all

.PHONY: clean fclean all re debug release