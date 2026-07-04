#include "mstream.h"
#include "util.h"
#include "string.h"

mstream::mstream()
{
	start = end = pos = NULL;
	currentBit = 0;
	eomFlag = true;
}

mstream::mstream(char* buf, uint64_t len)
{
	start = buf;
	end = start + len;
	pos = start;
	eomFlag = false;
	currentBit = 0;
}

mstream::mstream(uint64_t len)
{
	start = new char[len];
	end = start + len;
	pos = start;
	eomFlag = false;
	currentBit = 0;
}

uint64_t mstream::read(void* dest, uint64_t bytes)
{
	if (eomFlag)
		return 0;
	char* newpos = pos + bytes;
	if (newpos > end || newpos < start)
	{
		eomFlag = true;
		bytes = end - pos;
	}
	memcpy(dest, (void*)pos, bytes);
	pos = newpos;

	return bytes;
}

uint64_t mstream::write(const void* src, uint64_t bytes)
{
	if (eomFlag)
		return 0;
	char* newpos = pos + bytes;
	if (newpos > end || newpos < start)
	{
		eomFlag = true;
		return 0;
	}
	memcpy((void*)pos, src, bytes);
	pos = newpos;

	return bytes;
}

void mstream::seek(uint64_t to)
{
	pos = start + to;
	currentBit = 0;
	eomFlag = false;
	if (pos < start) {
		pos = start;
		eomFlag = true;
	}
	if (pos >= end) {
		pos = end;
		eomFlag = true;
	}
}

void mstream::seekBits(uint64_t to) {
	pos = start + (to >> 3);
	currentBit = to & 0x7;
	eomFlag = false;
	if (pos < start) {
		pos = start;
		eomFlag = true;
	}
	if (pos >= end) {
		pos = end;
		eomFlag = true;
	}
}

void mstream::seek(uint64_t to, int whence)
{
	switch (whence)
	{
	case (SEEK_SET):
		pos = start + to;
		break;
	case (SEEK_CUR):
		pos += to;
		break;
	case (SEEK_END):
		pos = end + to;
		break;
	}
	currentBit = 0;
	eomFlag = false;
	if (pos >= end || pos < start)
		eomFlag = true;
}

uint64_t mstream::skip(uint64_t bytes)
{
	if (eomFlag)
		return 0;
	char* newpos = pos + bytes;
	if (newpos >= end || newpos < start)
	{
		bytes = end - pos;
		eomFlag = true;
	}
	pos = newpos;
	currentBit = 0;
	return bytes;
}

uint64_t mstream::tell()
{
	return pos - start;
}

uint64_t mstream::tellBits() {
	return (pos - start) * 8 + currentBit;
}

char* mstream::getBuffer()
{
	return (char*)start;
}

char* mstream::getOffsetBuffer() {
	return (char*)pos;
}

bool mstream::eom()
{
	return eomFlag;
}

void mstream::freeBuf()
{
	if (start)
		delete[](char*)start;
	start = 0;
}

mstream::~mstream(void)
{

}

uint64_t mstream::size()
{
	return end - start;
}