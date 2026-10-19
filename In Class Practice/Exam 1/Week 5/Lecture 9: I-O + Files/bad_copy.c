#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 4096

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <source> <destination>\n", argv[0]);
    return 1;
  }

  FILE *src = fopen(argv[1], "rb");
  if (!src) {
    perror("Error opening source file");
    return 1;
  }

  FILE *dst = fopen(argv[2], "wb");
  if (!dst) {
    perror("Error opening destination file");
    fclose(src);
    return 1;
  }

  char buffer[BUFFER_SIZE];
  size_t bytes_read;
	int i = 1;
  while ((fgets(buffer,  BUFFER_SIZE, src))) {
    size_t bytes_written = fputs(buffer,  dst);
    if (bytes_written != bytes_read) {
      fprintf(stderr, "Error: write mismatch (%zu read, %zu written)\n",
              bytes_read, bytes_written);
      fclose(src);
      fclose(dst);
      return 1;
    }
  }