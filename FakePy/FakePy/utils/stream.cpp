#include "stdafx.h"
#include "stream.h"


Stream::Stream(const char *s) {
	str = s;
	pos = 0;
}

Stream::~Stream() {
	printf("Freed: %s", str);
}

int Stream::peek() {
	if (pos >= strlen(str))
		return -1;
	return str[pos];
}

int Stream::nxt() {
	if (pos >= strlen(str))
		return -1;
	return str[pos++];
}

void Stream::back() {
	if (pos < 0) {
		printf("out of bounds\n");
		return;
	}
	pos--;
}
