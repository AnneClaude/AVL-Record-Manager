all: store_basic store_advanced

store_basic: store_basic.c
	gcc -Wall -Wextra store_basic.c -o store_basic

store_advanced: store_advanced_series.c
	gcc -Wall -Wextra store_advanced_series.c -o store_advanced

clean:
	rm -f store_basic store_advanced
