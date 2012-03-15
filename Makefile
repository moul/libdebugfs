TARGETS		=	test
#TARGETS	+=	libdebugfs.a
TARGETS		+=	libdebugfs.so
#TARGETS	+=	libdebugfs-simple.so
CFLAGS		+=	-fPIC

all		:	$(TARGETS)

#test		:	test.o
#			$(CC) -Wl,--export-dynamic -o $@ $>

libdebugfs.so	:	libdebugfs.o
			$(CC) $< -shared -nostartfiles -ldl -o $@

clean		:
			rm -f *.o *~ *.bin .??*~ *#

fclean		:	clean
			rm -f $(TARGETS)

re		:	fclean all
