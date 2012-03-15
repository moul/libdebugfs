#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

//#include "libdebugfs.h"

struct rec {
  int x, y, z;
};

int		main(int ac, char **av) {
  int		counter, fd;
  FILE		*ptr_file;
  struct rec	record;
  struct stat	fileStat;

  /* Write to file */
  printf("[+] Opening file with 'wb'\n");
  ptr_file = fopen("test.bin", "wb");
  if (!ptr_file) {
    printf("[-] Unable to open file !\n");
    return 1;
  }
  printf("[+] Writing records to file\n");
  for (counter = 1; counter <= 10; counter++) {
    record.x = counter;
    record.y = counter * 42;
    record.z = 42;
    fwrite(&record, sizeof(record), 1, ptr_file);
  }
  fclose(ptr_file);

  /* Read from file */
  printf("[+] Opening file with 'rb'\n");
  ptr_file = fopen("test.bin", "rb");
  if (!ptr_file) {
    printf("Unable to open file!\n");
    return 1;
  }
  printf("[+] Reading records from file\n");
  for (counter = 1; counter <= 10; counter++) {
    fread(&record, sizeof(record), 1, ptr_file);
    printf("  - x: %d, y: %d, z: %d\n", record.x, record.y, record.z);
  }
  fclose(ptr_file);

  /* Seek and read from file */
  printf("[+] Opening file with 'rb'\n");
  ptr_file = fopen("test.bin", "rb");
  if (!ptr_file) {
    printf("Unable to open file!\n");
    return 1;
  }
  printf("[+] Seeking and reading records from file in reverse order\n");
  for (counter = 9; counter >= 0; counter--) {
    fseek(ptr_file, sizeof(record) * counter, SEEK_SET);
    fread(&record, sizeof(record), 1, ptr_file);
    printf("  - x: %d, y: %d, z: %d\n", record.x, record.y, record.z);
  }
  fclose(ptr_file);

  /* Seek to the end and rewind */
  printf("[+] Opening file with 'rb'\n");
  ptr_file = fopen("test.bin", "rb");
  if (!ptr_file) {
    printf("Unable to open file!\n");
    return 1;
  }
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
  printf("[+] Opening file with 'rb'\n");
  ptr_file = fopen("test.bin", "rb");
  if (!ptr_file) {
    printf("Unable to open file!\n");
    return 1;
  }
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
