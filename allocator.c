#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define SIZE 10000000

unsigned int arr[SIZE];
clock_t begin, end;

void allocate_first_fit(unsigned int adrs, int size) {
	int ind = -1, l = -1;
	for (int i = 0; i < SIZE; i++) {
		if (arr[i] == 0 && ind == -1) {
			ind = i;
		}
		if (arr[i] != 0) {
			if (ind != -1 && i - ind >= size) {
				l = ind + 1;
				break;
			}
			ind = -1;
		}
	}
	for (int i = l; i <= l + size; i++) 
		arr[i] = adrs;
}

void allocate_best_fit(unsigned int adrs, int size) {
	int mn = SIZE*10, ind = -1, l = -1;
	for (int i = 0; i < SIZE; i++) {
		if (arr[i] == 0 && ind == -1) {
			ind = i;
		}
		if (arr[i] != 0) {
			if (ind != -1 && i - ind >= size && i - ind < mn)
				mn = i-ind, l = ind + 1;
			ind = -1;
		}
		if (i == SIZE - 1) {
			if (i - ind < mn && i - ind >= size) {
				mn = i - ind, l = ind + 1;
			}
		}
	}
	for (int i = l; i <= l + size; i++) 
		arr[i] = adrs;
}

void allocate_worst_fit(unsigned int adrs, int size) {
	int mx = 0, ind = -1, l = -1;
	for (int i = 0; i < SIZE; i++) {
		if (arr[i] == 0 && ind == -1) {
			ind = i;
		}
		if (arr[i] != 0) {
			if (ind != -1 && i - ind >= size && i - ind > mx)
				mx = i-ind, l = ind + 1;
			ind = -1;
		}
		if (i == SIZE - 1) {
			if (i - ind > mx && i - ind >= size) {
				mx = i - ind, l = ind + 1;
			}
		}
	}
	for (int i = l; i <= l + size; i++) 
		arr[i] = adrs;
}

void clear(unsigned int adrs) {
	for (int i = 0; i < SIZE; i++) {
		if (arr[i] == adrs) {
			arr[i] = 0;
		}
	}
} 

void f(void (*fun)(unsigned int, int)) {
	double kol = 0;
	begin = clock();
	FILE* file = fopen("queries.txt", "r");
	char line[50];
	bool check = false;
	while (fgets(line, sizeof(line), file)) {
		char* token;
		token = strtok(line, " ");
		if (strcmp(&token[0], "end") == 0) {
			check = true;
			break;
		} else 
		if (strcmp(&token[0], "allocate") == 0) {
			token = strtok(NULL, " ");
			unsigned int adrs = atoi(&token[0]);
			token = strtok(NULL, "\n");
			int size = atoi(&token[0]);
			fun(adrs, size);
		} else
		if (strcmp(&token[0], "clear") == 0) {
			token = strtok(NULL, "\n");
			unsigned int adrs = atoi(&token[0]);
			clear(adrs);
		}
		kol++;
	}
	fclose(file);
	end = clock();
	double time = ((double)(end - begin) / CLOCKS_PER_SEC);
	printf("%lf\n", kol / time);
	for (int i = 0; i < SIZE; i++)
		arr[i] = 0;
}

int main() {
	f(allocate_first_fit);
	f(allocate_best_fit);
	f(allocate_worst_fit);
}
