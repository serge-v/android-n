#include <windows.h>
#include <stdio.h>

int convert_bitmap(char* name, char* path)
{
	BITMAPINFO bi;
	DWORD i, n, size;
	BYTE* buff;
	BYTE r, g, b, a;
	FILE* out_h;
	FILE* out_c;
	char out_path[1024];
	HBITMAP hBitmap = 0;

	hBitmap = (HBITMAP)LoadImageA(NULL,
	                              path,
	                              IMAGE_BITMAP,
	                              0,
	                              0,
	                              LR_LOADFROMFILE | LR_DEFAULTSIZE);
	if (!hBitmap) {
		fprintf(stderr, "Cannot open %s. Error: %d\n", path, GetLastError());
		return 2;
	}

	memset(&bi, 0, sizeof(bi));
	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

	n = GetDIBits(GetDC(NULL), hBitmap, 0, 0, NULL, &bi, DIB_RGB_COLORS);
	size = bi.bmiHeader.biWidth * bi.bmiHeader.biHeight * 4;

	buff = malloc(bi.bmiHeader.biSizeImage);

	bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
	bi.bmiHeader.biBitCount = 32;
	bi.bmiHeader.biCompression = BI_RGB;

	n = GetDIBits(GetDC(NULL), hBitmap, 0, bi.bmiHeader.biHeight, buff, &bi, DIB_RGB_COLORS);

	strcpy(out_path, path);
	strcat(out_path, ".h");
	out_h = fopen(out_path, "wt");
	if (!out_h) {
		fprintf(stderr, "Cannot open %s. Error: %d\n", out_path, errno);
		return 2;
	}

	strcpy(out_path, path);
	strcat(out_path, ".c");
	out_c = fopen(out_path, "wt");
	if (!out_c) {
		fprintf(stderr, "Cannot open %s. Error: %d\n", out_path, errno);
		return 2;
	}

	fprintf(out_h, "const int %s_width = %d;\n", name, bi.bmiHeader.biWidth);
	fprintf(out_h, "const int %s_height = %d;\n", name, bi.bmiHeader.biHeight);
	fprintf(out_h, "extern unsigned char %s_rgba[];\n", name);

	fprintf(out_c, "unsigned char %s_rgba[] = {\n", name);
	for (i = 0; i < bi.bmiHeader.biSizeImage / 4; i++) {
		if (i % 4 == 0)
			fprintf(out_c, "\n    ");
		r = buff[i * 4 + 2];
		g = buff[i * 4 + 1];
		b = buff[i * 4 + 0];
		a = 0xFF;

		if (r == 0xFF && g == 0xFF && b == 0xFF)
			a = 0x00;
		fprintf(out_c, "0x%02X, 0x%02X, 0x%02X, 0x%02X,    ", r, g, b, a);

	}
	fprintf(out_c, "\n};\n\n");

	fclose(out_h);
	fclose(out_c);
	free(buff);

	return 0;
}

int main(int argc, char* argv[])
{
	if (argc < 3)
		return 1;
	return convert_bitmap(argv[1], argv[2]);
}
