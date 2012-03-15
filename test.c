#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

//#include "libdebugfs.h"

#define TEST_FILE "test.bin"

struct rec {
  int x, y, z;
};

/**
 * Helpers functions for
 *   int	open(char * filename, int flags, int mode)
 *   int	close(int fd)
 *   int	read(int fd, void * ptr, int numbytes)
 *   int	write(int fd, void * ptr, int numbytes)
 *   int	lseek(int fd, int position, int startpoint)
 *   int	stat(char * file, struct stat * info)
 *
 * http://rabbit.eng.miami.edu/info/functions/unixio.html
 */
int		xopen(char *filename, int flags, int mode) {
  int		ret;

  printf("[+] Opening file '%s' with flags 0x%x and mode 0x%x\n", filename, flags, mode);
  ret = open(filename, flags, mode);
  if (ret <= 0) {
    printf("[-] Unable to open file\n");
    perror(__func__);
    exit(1);
  }
  return ret;
}

/**
 * Helpers functions for
 *   FILE	*fopen(char filename[], char access[]);
 *   int	fclose(FILE *fileref);
 *   int	fputc(int c, FILE *fileref);
 *   int	fprintf(FILE *fileref, char format[], ...);
 *   int	fgetc(FILE *fileref);
 *   char	*fgets(char str[], int length, FILE *fileref);
 *
 * http://rabbit.eng.miami.edu/info/functions/stdio.html
 */
FILE		*xfopen(const char *path, const char *mode) {
  FILE		*ret;

  printf("[+] Fopening file '%s' with mode '%s'\n", path, mode);
  ret = fopen(path, mode);
  if (!ret) {
    printf("[-] Unable to fopen file\n");
    perror(__func__);
    exit(1);
  }
  return ret;
}

/**
 * test main function
 */
int		main(int ac, char **av) {
  int		counter, fd;
  FILE		*ptr_file;
  struct rec	record;
  struct stat	fileStat;

  fd = xopen(TEST_FILE, O_RDONLY | O_CREAT, 0);

  /* Write to file */
  ptr_file = xfopen(TEST_FILE, "wb");
  printf("[+] Writing records to file\n");
  for (counter = 1; counter <= 10; counter++) {
    record.x = counter;
    record.y = counter * 42;
    record.z = 42;
    fwrite(&record, sizeof(record), 1, ptr_file);
  }
  fclose(ptr_file);

  /* Read from file */
  ptr_file = xfopen(TEST_FILE, "rb");
  printf("[+] Reading records from file\n");
  for (counter = 1; counter <= 10; counter++) {
    fread(&record, sizeof(record), 1, ptr_file);
    printf("  - x: %d, y: %d, z: %d\n", record.x, record.y, record.z);
  }
  fclose(ptr_file);

  /* Seek and read from file */
  ptr_file = xfopen(TEST_FILE, "rb");
  printf("[+] Seeking and reading records from file in reverse order\n");
  for (counter = 9; counter >= 0; counter--) {
    fseek(ptr_file, sizeof(record) * counter, SEEK_SET);
    fread(&record, sizeof(record), 1, ptr_file);
    printf("  - x: %d, y: %d, z: %d\n", record.x, record.y, record.z);
  }
  fclose(ptr_file);

  /* Seek to the end and rewind */
  ptr_file = xfopen(TEST_FILE, "rb");
  printf("[+] Seeking to the end of file\n");
  fseek(ptr_file, sizeof(record), SEEK_END);
  printf("[+] Rewinding file\n");
  rewind(ptr_file);
  for (counter = 1; counter <= 10; counter++) {
    fread(&record, sizeof(record), 1, ptr_file);
    printf("  - x: %d, y: %d, z: %d\n", record.x, record.y, record.z);
  }
  fclose(ptr_file);

  /* fstat file */
  ptr_file = xfopen(TEST_FILE, "rb");
  fd = fileno(ptr_file);
  printf("  - fd is: %d\n", fd);
  printf("[+] Fstat file\n");
  if (fstat(fd, &fileStat) < 0) {
    printf("Unable to fstat file!\n");
    return 1;
  }
  printf("  - File Size: \t\t%d bytes\n", (int)fileStat.st_size);
  printf("  - Number of Links: \t%d\n", (int)fileStat.st_nlink);
  printf("  - File inode: \t\t%d\n", (int)fileStat.st_ino);
  printf("  - File Permissions: \t");
  printf((S_ISDIR(fileStat.st_mode)) ? "d" : "-");
  printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
  printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
  printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
  printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
  printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
  printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
  printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
  printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
  printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");
  printf("\n\n");

  fclose(ptr_file);

  return 0;
}
