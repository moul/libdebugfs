TARGETS	=	test
#TARGETS	+=	libdebugfs.a
#TARGETS	+=	libdebugfs.so
#TARGETS	+=	libdebugfs-simple.so

all	:	$(TARGETS)

#test	:	test.o
#		$(CC) -Wl,--export-dynamic -o $@ $>

clean	:
		rm -f *.o *~ *.bin .??*~

fclean	:	clean
		rm -f $(TARGETS)

re	:	fclean all
