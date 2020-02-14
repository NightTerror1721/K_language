#include <stdio.h>

#include "utils/cbuffer.h"


int main(int argc, char** argv)
{
	CBuffer* b = cbuf_new();

	cbuf_putc(b, 'h');
	cbuf_putc(b, 'o');
	cbuf_putc(b, 'l');
	cbuf_putc(b, 'a');

	cbuf_flush(b);

	cbuf_puts(b, "hola mundo");

	cbuf_flush(b);

	cbuf_printf(b, "%d + %d = %d; // %s;\n", 5, 10, 15, "esto es una prueba de string tocho");
	printf("%ssalto guay\n", cbuf_rawdata(b));


	int c;
	cbuf_popc(b, &c);


	cbuf_flush(b);
	cbuf_puts(b, "10, 50, -750; texto de prueba");

	int x, y, z;
	char text[256];
	cbuf_scanf(b, "%d, %d, %d; %s", &x, &y, &z, text);


	cbuf_delete(b);

	return 0;
}
