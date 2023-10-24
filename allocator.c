#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define SIZE 10000000

unsigned int array[SIZE];
clock_t start, end;

void allocate_first_fit(unsigned int adrs, int size) {
	int pos = -1, l = -1;
	for (int i = 0; i < SIZE; i++) {
		if (array[i] == 0 && pos == -1) {
			pos = i;
		}
		if (array[i] != 0) {
			if (pos != -1 && i - pos >= size) {
				l = pos + 1;
				break;
			}
			pos = -1;
		}
	}
	for (int i = l; i <= l + size; i++)
		array[i] = adrs;
}

void allocate_best_fit(unsigned int adrs, int size) {
	int mn = SIZE * 10, pos = -1, l = -1;
	for (int i = 0; i < SIZE; i++) {
		if (array[i] == 0 && pos == -1) {
			pos = i;
		}
		if (array[i] != 0) {
			if (pos != -1 && i - pos >= size && i - pos < mn)
				mn = i - pos, l = pos + 1;
			pos = -1;
		}
		if (i == SIZE - 1) {
			if (i - pos < mn && i - pos >= size) {
				mn = i - pos, l = pos + 1;
			}
		}
	}
	for (int i = l; i <= l + size; i++)
		array[i] = adrs;
}

void allocate_worst_fit(unsigned int adrs, int size) {
	int mx = 0, pos = -1, l = -1;
	for (int i = 0; i < SIZE; i++) {
		if (array[i] == 0 && pos == -1) {
			pos = i;
		}
		if (array[i] != 0) {
			if (pos != -1 && i - pos >= size && i - pos > mx)
				mx = i - pos, l = pos + 1;
			pos = -1;
		}
		if (i == SIZE - 1) {
			if (i - pos > mx && i - pos >= size) {
				mx = i - pos, l = pos + 1;
			}
		}
	}
	for (int i = l; i <= l + size; i++)
		array[i] = adrs;
}

void clear(unsigned int adrs) {
	for (int i = 0; i < SIZE; i++) {
		if (array[i] == adrs) {
			array[i] = 0;
		}
	}
}

void f(void (*fun)(unsigned int, int)) {
	double kol = 0;
	start = clock();
	FILE* file = fopen("queries.txt", "r");
	char line[50];
	bool bb = false;
	while (fgets(line, sizeof(line), file)) {
		char* word;
		word = strtok(line, " ");
		if (strcmp(&word[0], "end") == 0) {
			bb = true;
			break;
		}
		else
			if (strcmp(&word[0], "allocate") == 0) {
				word = strtok(NULL, " ");
				unsigned int adrs = atoi(&word[0]);
				word = strtok(NULL, "\n");
				int size = atoi(&word[0]);
				fun(adrs, size);
			}
			else
				if (strcmp(&word[0], "clear") == 0) {
					word = strtok(NULL, "\n");
					unsigned int adrs = atoi(&word[0]);
					clear(adrs);
				}
		kol++;
	}
	fclose(file);
	end = clock();
	double time = ((double)(end - start) / CLOCKS_PER_SEC);
	printf("%lf\n", kol / time);
	for (int i = 0; i < SIZE; i++)
		array[i] = 0;
}

int main() {
	f(allocate_first_fit);
	f(allocate_best_fit);
	f(allocate_worst_fit);
}
