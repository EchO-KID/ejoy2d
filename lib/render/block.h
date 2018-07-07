#ifndef ejoy3d_block_h
#define ejoy3d_block_h

#include <stdlib.h>

//! 块数据
struct block {
	char * buffer;       //! buffer
	int sz;              //! 块数据大小
};

static inline void 
block_init(struct block * B, void * buffer, int sz) {
	B->buffer = (char*)buffer;
	B->sz = sz;
}

//! 剔除 buffer 中的sz(前面部分)
static inline void * 
block_slice(struct block * B, int sz) {
	if (B->sz < sz) {
		return NULL;
	}
	void * ret = B->buffer;
	B->buffer += sz;
	B->sz -= sz;
	return ret;
}

#endif
