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

  unsigned char buffer[BUFFER_SIZE];
  size_t bytes_read;
	int i = 1;
  while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, src)) > 0) {
		printf("%d -> bytes read = %ld\n",i++,bytes_read);
    size_t bytes_written = fwrite(buffer, 1, bytes_read, dst);
    if (bytes_written != bytes_read) {
      fprintf(stderr, "Error: write mismatch (%zu read, %zu written)\n",
              bytes_read, bytes_written);
      fclose(src);
      fclose(dst);
      return 1;
    }
  }

  if (ferror(src)) {
    perror("Error reading source file");
  }

  fclose(src);
  fclose(dst);

  printf("File copied successfully.\n");
  return 0;
}
