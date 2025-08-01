#include "./dependencies/Interfaces/lexer.h"
#include "./dependencies/Interfaces/REPL.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

char* read_file(const char* filename);

int main(int argc, char** argv)
{
	Start();

}


char* read_file(const char* filename)  {
	
	int32_t file = open(filename, O_RDONLY, S_IRUSR);

	struct stat sb;

	if (fstat(file, &sb) == -1)
	{
		perror("fstat failed\nerrno");
		exit(EXIT_FAILURE);
	}

	char* file_contents = mmap(NULL, (size_t)sb.st_size, PROT_READ, MAP_PRIVATE, file, 0);

	return file_contents;
}