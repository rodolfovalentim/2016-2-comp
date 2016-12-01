all: flex bison gcc
	@echo "Done."

bison: parser.y
	bison parser.y

flex: scanner.l
	flex scanner.l

gcc: scanner.c parser.c tables.c stack.c code.c
	gcc -Wall -o trab4 scanner.c parser.c tables.c tree.c stack.c code.c -ly

git:
	git add .
	git commit -m "Automatic commit"
	git push origin master

clean:
	@rm -f scanner.c scanner
	rm -f trab3
	rm -f parser parser.c parser.h
	rm -rfd my-out diff

test:
	bash gen_tests.sh
	bash check.sh
	@echo "Finished test"
