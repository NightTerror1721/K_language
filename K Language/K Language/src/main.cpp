#include <stdio.h>

#include "utils/cbuffer.h"
#include "utils/clist.h"


int main(int argc, char** argv)
{
	/*CBuffer* b = cbuf_new();

	cbuf_putc(b, 'h');
	cbuf_putc(b, 'o');
	cbuf_putc(b, 'l');
	cbuf_putc(b, 'a');

	cbuf_flush(b);

	cbuf_puts(b, "hola mundo");

	cbuf_flush(b);

	cbuf_printf(b, "%d + %d = %d; // %s;\n", 5, 10, 15, "esto es una prueba de string tocho");
	printf("%ssalto guay\n", cbuf_rawdata(b));

	CIterator it = cbuf_iterator(b);
	cforeach(char, ch, &it)
	{
		if(ch)
			putc(*ch, stdout);
	}
	putc('\n', stdout);


	int c;
	cbuf_popc(b, &c);


	cbuf_flush(b);
	cbuf_puts(b, "10, 50, -750; texto de prueba");

	int x, y, z;
	char text[256];
	cbuf_scanf(b, "%d, %d, %d; %s", &x, &y, &z, text);


	cbuf_delete(b);*/

	CIntList* list = (CIntList*) clist_new(int);

	clist_push_back(list, 50);
	clist_push_back(list, 100);
	clist_push_back(list, 150);

	clist_push_front(list, 200);


	/*CIterator it = clist_iterator(list);
	cforeach(int, i, &it)
	{
		printf("%d ", *i);
	}
	printf("\n");*/

	for (CListIterator it = clist_iter_begin(list), iend = clist_iter_end(list); !clist_iter_equals(it, iend); clist_iter_next(&it))
	{
		printf("%d ", *clist_iter_get(list, it));
	}

	printf("%d\n", *clist_get(list, 2));


	clist_delete(list);


	/*CList(int) list;

	clist_init(&list);

	clist_push_back(&list, 50);*/


	return 0;
}
