#include <stdio.h>

struct {
  const char* name;
  FILE *fp;
} file1, file2;
char buffer1[2048];
char buffer2[2048];

int main(int argc, const char* argv[])
{
  size_t totalCompared = 0;
  
  if(argc != 3) {
    printf("usage: bindiff file1 file2\n");
    return 1;
  }
  file1.name = argv[1];
  file2.name = argv[2];

  file1.fp = fopen(file1.name, "rb");
  if(!file1.fp) {
    printf("error: '%s' does not exist\n", file1.name);
    return 1;
  }
  file2.fp = fopen(file2.name, "rb");
  if(!file2.fp) {
    printf("error: '%s' does not exist\n", file2.name);
    return 1;
  }

  while(1) {
    size_t size = fread(buffer1, 1, sizeof(buffer1), file1.fp);
    if(size == 0) {
      break;
    }
    {
      size_t secondRead = fread(buffer2, 1, sizeof(buffer2), file2.fp);
      if(secondRead != size) {
	if(secondRead == 0) {
	  printf("mismatch: file1 is bigger than file2\n");
	  return 1;
	}
	printf("error: file1.fread returned %u, file2.fread returned %u\n",
	       size, secondRead);
	return 1;
      }
    }
    for(size_t i = 0; i < size; i++) {
      if(buffer1[i] != buffer2[i]) {
	printf("offset %u (0x%X) file1 0x%02X != file2 0x%02x\n",
	       totalCompared+size, totalCompared+size, buffer1[i], buffer2[i]);
	return 1;
      }
    }
    totalCompared += size;
    //printf("[DEBUG] compared %u bytes (total %u)\n", size, totalCompared);
  }

  {
    size_t size = fread(buffer2, 1, sizeof(buffer2), file2.fp);
    if(size != 0) {
      printf("mismatch: file2 is bigger than file1\n");
      return 1;
    }
  }

  printf("Match (compared %u bytes)\n", totalCompared);
  return 0;
}
