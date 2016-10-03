
all: flex gcc
	@echo "Done."

flex: scanner.l
	flex scanner.l

gcc: scanner.c
	gcc -Wall -o trab1 scanner.c

clean:
	@rm -f scanner.c scanner
	rm -f trab1
	rm -rfd my-out diff

run:
	bash gen_tests.sh

check:
	bash check.sh

test: run check
	@echo "Finished test."
