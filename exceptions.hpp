#ifndef __EXCEPTIONS__
#define __EXCEPTIONS__

struct parser_exception {
	const char* msg;
	int pos;
	parser_exception(const char* m, int p) : msg(m), pos(p) {}
};

#endif
