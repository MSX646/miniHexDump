#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

void usage()
{
	printf("./hexDump [-n <int> -l <int> -o <int> -h] <file to read>\n");
	printf("-n <int> 	Bytes to read (default: all)\n");
	printf("-l <int> 	Bytes per line (default: 16)\n");
	printf("-o <int> 	Byte offset to begin reading (default: 0)\n");
	printf("-h 		Print this message and exit\n");

	exit(1);
}

void print_line(uint8_t *buff, int bytes, long offset, int line)
{
	printf("%08lX |", offset);

	for (int i = 0; i < line; i++)
	{
		if (i > 0 && i % 4 == 0)
			printf(" ");
		if (i < bytes)
			printf(" %02X", buff[i]);
		else
			printf("	");
	}

	printf(" | ");

	for (int i = 0; i < bytes; i++)
	{
		if (buff[i] > 31 && buff[i] < 127)
			printf("%c", buff[i]);
		else
			printf(".");
	}

	printf("\n");
}

void dump_file(FILE *file, long offset, int num, int line)
{
	uint8_t *buff = (uint8_t *)malloc(line);
	if (!buff)
	{
		printf("Malloc error\n");
		exit(1);
	}

	while (true)
	{
		int max_bytes;

		if (offset < 0)
		{
			printf("Negative offset\n");
			exit(1);	
		}
		
		if (num < 0 || line < num)
			max_bytes = line;
		else
			max_bytes = num;

		int bytes = fread(buff, sizeof(uint8_t), max_bytes, file);
		if (bytes > 0)
		{
			print_line(buff, bytes, offset, line);
			offset += bytes;
			num -= bytes;
		}
		else
			break;
	}

	free(buff);
}

int main(int ac, char **av)
{
	// DEFAULT VALUES
	long offset = 0;
	int num = -1;
	int line = 16;
	int av_off = ac - 1;
	int c;

	if (ac == 1)
		usage();

	while ((c = getopt(ac, av, "o:l:n:h")) != -1)
	{
		switch (c)
		{
			case 'o':
				offset = atoi(optarg);
				break;
			case 'l':
				line = atoi(optarg);
				break;
			case 'n':
				num = atoi(optarg);
				break;
			case 'h':
				usage();
			case '?':
				usage();
		}
	}

	if (av[av_off] == NULL || !strcmp(av[av_off], "-l") || !strcmp(av[av_off], "-n") || !strcmp(av[av_off], "-o"))
		usage();

	FILE *file = fopen(av[av_off], "rb");
	if (!file)
	{
		printf("Can't open file\n");
		return 1;
	}

	dump_file(file, offset, num, line);

	fclose(file);
	return 0;
}
