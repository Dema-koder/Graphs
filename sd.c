#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

int main() {
    int random_fd = open("/dev/random", O_RDONLY);
    int text_fd = open("text.txt", O_CREAT | O_RDWR | O_TRUNC, 0666);
 
    long page_size = sysconf(_SC_PAGESIZE);
    long chunk_size = page_size * 1024;

    char *buffer = (char *)malloc(chunk_size);
  

    off_t file_size = 500 * 1024 * 1024;
    ftruncate(text_fd, file_size);
    struct stat file_stat;
    fstat(text_fd, &file_stat);
    char *file_ptr = mmap(NULL, file_stat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, text_fd, 0);
 int count = 0;
    long offset = 0;
    while (offset < file_size) {
        ssize_t bytes_read = read(random_fd, buffer, chunk_size);
        if (bytes_read == -1) {
            perror("Failed to read from /dev/random");
            exit(1);
        }

        for (ssize_t i = 0; i < bytes_read; i++) {
            if (isprint(buffer[i])) {
             if (isupper(buffer[i])){
              count++;
             }
                file_ptr[offset] = tolower(buffer[i]);
                offset++;
            }
            if (offset >= file_size) {
                break;
            }
            
   if (offset % 1024 == 0){
    file_ptr[offset] = '\n'; 
   }
        }
    }

   munmap(file_ptr, file_stat.st_size);
 printf("%d ", count);
    close(random_fd);
    close(text_fd);
 

    return 0;
}
