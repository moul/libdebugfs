all	:	test

#test	:	test.o
#		$(CC) -Wl,--export-dynamic -o $@ $>

clean	:
		rm -f *.o

fclean	:	clean
		rm -f *.so *.a test

re	:	fclean all
