all: main.o scanner.o sya_parser.o sya_grammar.o sya_stack.o utl_err.o utl_mem.o sea_library.o sea_queue.o symtable.o sea_retype.o gvk_generator.o
	gcc -std=c99 -Wall -Wextra -pedantic -o IFJ-17 $^
clean:
	rm IFJ-17
	find . -name '*.o' -delete
