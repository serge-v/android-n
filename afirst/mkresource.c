/*
 * Convert file into C header/source pair.
 */

#include <stdio.h>
#include <err.h>

int main(int argc, char **argv)
{
	if (argc < 5)
		err(1, "usage: mkresource INFILE VAR_NAME OUT_HEADER OUT_SOURCE");

	const char *infile = argv[1];
	const char *var_name = argv[2];
	const char *out_header = argv[3];
	const char *out_source = argv[4];

	FILE *fd = fopen(infile, "rb");
	if (fd == NULL)
		err(1, "cannot open input file %s", infile);

	FILE *hfd = fopen(out_header, "wt");
	if (hfd == NULL)
		err(1, "cannot open output header file %s", out_header);

	FILE *sfd = fopen(out_source, "wt");
	if (sfd == NULL)
		err(1, "cannot open output source file %s", out_source);

	fprintf(hfd, "unsigned char *%s;\n", var_name);
	fprintf(sfd, "unsigned char %s[] = {\n", var_name);

	const size_t buf_size = 4096;
	unsigned char buf[buf_size];
	size_t was_read = 0;
	size_t total_size = 0;
	size_t line_pos = 0;

	while ((was_read = fread(buf, 1, buf_size, fd)) > 0) {
		for (size_t i = 0; i < was_read; i++) {

			if (line_pos == 0)
				fprintf(sfd, "\t");

			fprintf(sfd, "0x%02x,", buf[i]);

			if (line_pos < 15)
				fprintf(sfd, " ");
			else
				fprintf(sfd, "\n");

			line_pos++;

			if (line_pos >= 16)
				line_pos = 0;
		}

		total_size += was_read;
	}

	fprintf(sfd, "\n};\n");
	fprintf(hfd, "unsigned int %s_len = %zu;\n", var_name, total_size);

	fclose(hfd);
	fclose(sfd);
	fclose(fd);
}
