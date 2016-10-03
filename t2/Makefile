all: flex bison gcc
	@echo "Done."

bison: parser.y
	bison parser.y

flex: scanner.l
	flex scanner.l

gcc: scanner.c parser.c
	gcc -Wall -o parser scanner.c parser.c -ly

clean:
	@rm -f scanner.c scanner
	rm -f trab1
	rm -f parser parser.c parser.h
	rm -rfd my-out diff

test:
	bash gen_tests.sh
	bash check.sh
	@echo "Finished test"
