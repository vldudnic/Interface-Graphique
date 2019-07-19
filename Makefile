# Variable definitions.

PLATFORM	= $(shell uname)
CC		= gcc
OBJDIR		= ./objs
TESTS		= ./tests
SRC = ./src
INCLUDES	= ./include
INCFLAGS	:= -I${INCLUDES} -I${TESTS}
#OPTFLAGS	:= -Os -DNDEBUG
OPTFLAGS	:= -g
CCFLAGS		:= -c ${OPTFLAGS} -Wall -std=c99

# The list of objects to include in the library

LIBEIOBJS	:= objs/aux_point.o objs/aux_polygon.o objs/aux_bouton.o objs/ei_application.o objs/ei_bouton.o objs/ei_draw_polygon.o objs/ei_draw.o objs/ei_event.o objs/ei_frame.o objs/ei_picking.o objs/ei_placer.o objs/ei_top_level.o objs/ei_widget.o objs/ei_widgetclass.o objs/aux_top_level.o




# Platform specific definitions (OS X, Linux)

ifeq (${PLATFORM},Darwin)

	# Building for Mac OS X

	PLATDIR		= _osx
	INCFLAGS	:= ${INCFLAGS} -I/opt/local/include/SDL
	LINK		= ${CC}
	LIBEI		= ${OBJDIR}/libei.a
	LIBEIBASE	= ${PLATDIR}/libeibase.a
	LIBS		= ${LIBEIBASE} -L/opt/local/lib -lSDL -lSDL_ttf -lSDL_image -framework AppKit
	CCFLAGS		:= ${CCFLAGS} -D__OSX__

else

	# Building for Linux

	PLATDIR		= _x11
	INCFLAGS	:= ${INCFLAGS} -I/usr/include/SDL
	LINK		= ${CC}
#	ARCH	        = 32
	ARCH	        = 64
	LIBEI		= ${OBJDIR}/libei.a
	LIBEIBASE	= ${PLATDIR}/libeibase${ARCH}.a
	LIBS		= ${LIBEIBASE} -lSDL -lSDL_ttf -lSDL_image -lm
	CCFLAGS		:= ${CCFLAGS} -D__LINUX__ -m${ARCH}
	LDFLAGS		= -m${ARCH} -g

endif



# Main target of the makefile. To build specific targets, call "make <target_name>"

TARGETS		=	${LIBEI} \
			minimal lines frame button hello_world puzzle two048
all : ${TARGETS}



########## Test-programs

# minimal

minimal : ${OBJDIR}/minimal.o ${LIBEIBASE}
	${LINK} -o minimal ${LDFLAGS} ${OBJDIR}/minimal.o ${LIBS}

${OBJDIR}/minimal.o : ${TESTS}/minimal.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/minimal.c -o ${OBJDIR}/minimal.o

# lines

lines : ${OBJDIR}/lines.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o lines ${OBJDIR}/lines.o ${LIBEI} ${LIBS}

${OBJDIR}/lines.o : ${TESTS}/lines.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/lines.c -o ${OBJDIR}/lines.o

# frame

frame : ${OBJDIR}/frame.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o frame ${LDFLAGS} ${OBJDIR}/frame.o ${LIBEI} ${LIBS}

${OBJDIR}/frame.o : ${TESTS}/frame.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/frame.c -o ${OBJDIR}/frame.o

# button

button : ${OBJDIR}/button.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o button ${LDFLAGS} ${OBJDIR}/button.o ${LIBEI} ${LIBS}

${OBJDIR}/button.o : ${TESTS}/button.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/button.c -o ${OBJDIR}/button.o

# hello_world

hello_world : ${OBJDIR}/hello_world.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o hello_world ${LDFLAGS} ${OBJDIR}/hello_world.o ${LIBEI} ${LIBS}

${OBJDIR}/hello_world.o : ${TESTS}/hello_world.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/hello_world.c -o ${OBJDIR}/hello_world.o

# puzzle

puzzle : ${OBJDIR}/puzzle.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o puzzle ${LDFLAGS} ${OBJDIR}/puzzle.o ${LIBEI} ${LIBS}

${OBJDIR}/puzzle.o : ${TESTS}/puzzle.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/puzzle.c -o ${OBJDIR}/puzzle.o


# two048

two048 : ${OBJDIR}/two048.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o two048 ${LDFLAGS} ${OBJDIR}/two048.o ${LIBEI} ${LIBS}

${OBJDIR}/two048.o : ${TESTS}/two048.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/two048.c -o ${OBJDIR}/two048.o


# Building of the library libei

${LIBEI} : ${LIBEIOBJS}
	ar rcs ${LIBEI} ${LIBEIOBJS}


${OBJDIR}/aux_point.o : ${SRC}/aux_point.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${SRC}/aux_point.c -o ${OBJDIR}/aux_point.o

${OBJDIR}/aux_polygon.o : ${SRC}/aux_polygon.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${SRC}/aux_polygon.c -o ${OBJDIR}/aux_polygon.o

${OBJDIR}/ei_draw.o : ${SRC}/ei_draw.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${SRC}/ei_draw.c -o ${OBJDIR}/ei_draw.o

${OBJDIR}/ei_draw_polygon.o : ${SRC}/ei_draw_polygon.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${SRC}/ei_draw_polygon.c -o ${OBJDIR}/ei_draw_polygon.o

${OBJDIR}/aux_bouton.o : ${SRC}/aux_bouton.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${SRC}/aux_bouton.c -o ${OBJDIR}/aux_bouton.o

${OBJDIR}/ei_widgetclass.o : ${SRC}/ei_widgetclass.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${SRC}/ei_widgetclass.c -o ${OBJDIR}/ei_widgetclass.o

${OBJDIR}/ei_frame.o : ${SRC}/ei_frame.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${SRC}/ei_frame.c -o ${OBJDIR}/ei_frame.o

${OBJDIR}/ei_bouton.o : ${SRC}/ei_bouton.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${SRC}/ei_bouton.c -o ${OBJDIR}/ei_bouton.o

${OBJDIR}/ei_top_level.o : ${SRC}/ei_top_level.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${SRC}/ei_top_level.c -o ${OBJDIR}/ei_top_level.o

${OBJDIR}/ei_widget.o : ${SRC}/ei_widget.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${SRC}/ei_widget.c -o ${OBJDIR}/ei_widget.o

${OBJDIR}/ei_placer.o : ${SRC}/ei_placer.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${SRC}/ei_placer.c -o ${OBJDIR}/ei_placer.o

${OBJDIR}/ei_picking.o : ${SRC}/ei_picking.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${SRC}/ei_picking.c -o ${OBJDIR}/ei_picking.o

${OBJDIR}/ei_event.o : ${SRC}/ei_event.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${SRC}/ei_event.c -o ${OBJDIR}/ei_event.o

${OBJDIR}/ei_application.o : ${SRC}/ei_application.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${SRC}/ei_application.c -o ${OBJDIR}/ei_application.o

${OBJDIR}/aux_top_level.o : ${SRC}/aux_top_level.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${SRC}/aux_top_level.c -o ${OBJDIR}/aux_top_level.o


# Building of the doxygen documentation.

doc :
	doxygen docs/doxygen.cfg



# Removing all built files.

clean:
	rm -f ${TARGETS}
	rm -f *.exe
	rm -f ${OBJDIR}/*
