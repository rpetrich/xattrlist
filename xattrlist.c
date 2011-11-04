#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/xattr.h>

void print_hex_chunk(char *data, int n)
{
	for (int i = 0; i < n; i++)
		printf("%02x ", data[i]);
	printf(" ");
	for (int i = 0; i < n; i++) {
		if (isgraph((unsigned) data[i]))
			printf("%c", data[i]);
		else
			printf(".");
	}
}

static void print_hex_buffer(char *buffer, size_t size, size_t bytes_per_line, char *prefix)
{
	for (int i = 0; i < size; i += bytes_per_line) {
		if (prefix)
			printf("%s", prefix);
		if (i + bytes_per_line <= size)
			print_hex_chunk(buffer + i, bytes_per_line);
		else
			print_hex_chunk(buffer + i, size - i);
		printf("\n");
	}
}

int main(int argc, const char *argv[])
{
	for (int i = 1; i < argc; i++) {
		printf("%s\n", argv[i]);
		ssize_t buffer_size = listxattr(argv[i], NULL, 0, XATTR_NOFOLLOW);
		if (buffer_size != -1) {
			char buffer[buffer_size];
			if (listxattr(argv[i], buffer, buffer_size, XATTR_NOFOLLOW) == buffer_size) {
				size_t offset = 0;
				while (offset < buffer_size) {
					ssize_t size = printf("  %s\n", &buffer[offset]) - 2;
					ssize_t value_buffer_size = getxattr(argv[i], &buffer[offset], NULL, 0, 0, XATTR_NOFOLLOW);
					if (value_buffer_size != -1) {
						char *value_buffer = malloc(value_buffer_size);
						if (value_buffer) {
							if (getxattr(argv[i], &buffer[offset], value_buffer, value_buffer_size, 0, XATTR_NOFOLLOW) == value_buffer_size) {
								print_hex_buffer(value_buffer, value_buffer_size, 16, "    ");
								printf("    %zi bytes\n", value_buffer_size);
							} else {
								printf("    <error copying contents>\n");
							}
						} else {
							printf("    <error allocating buffer>\n");
						}
					} else {
						printf("    <error getting size of attr>\n");
					}
					offset += size;
				}
			}
		} else {
			printf("  <error getting size of attrs>\n");
		}
	}
	return 0;
}
