#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_FILE_SIZE (500 * 1024 * 1024)

int main() {
    int output_file = open("output.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    ftruncate(output_file, MAX_FILE_SIZE);

    char *file_data = mmap(NULL, MAX_FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, output_file, 0);

    close(output_file);

    int random_data_file = open("/dev/random", O_RDONLY);
    int capital_count = 0;
    int char_count = 0;

    while (char_count < MAX_FILE_SIZE) {
        char random_buffer;
        ssize_t bytes_read = read(random_data_file, &random_buffer, sizeof(random_buffer));

        if (bytes_read == -1) {
            perror("Failed to read from /dev/random");
            close(random_data_file);
            munmap(file_data, MAX_FILE_SIZE);
            exit(EXIT_FAILURE);
        }

        if (isprint((unsigned char)random_buffer)) {
            if (isupper(random_buffer)) {
                capital_count++;
                random_buffer = tolower(random_buffer);
            }

            file_data[char_count] = random_buffer;
            char_count++;
        }
    }

    close(random_data_file);
    munmap(file_data, MAX_FILE_SIZE);

    printf("Number of capital letters: %d\n", capital_count);

    return 0;
}
