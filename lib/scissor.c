#include "scissor.h"
#include "screen.h"
#include "shader.h"

#include <assert.h>

#define SCISSOR_MAX 8

//! 裁剪的 四边形
//! (x,y)            
//!   -----------
//!  |           |
//!  |           | height
//!  |           |
//!  |           |
//!   -----------
//!     width
struct box {
	int x;
	int y;
	int width;
	int height;
};

//! 裁剪的区域
struct scissor {
	int depth;
	struct box s[SCISSOR_MAX];
};

static struct scissor S;


//! 裁剪的 四边形
//!            
//!   -----------
//!  | a          |
//!  |      ------|----
//!  |     |b     |    |
//!  |     |      |    |
//!   -----------      |
//! (x,y)  |           |      
//!         -----------

//! 求 ab 的交集四边形
static void
intersection(struct box * b, int * x, int * y, int * w, int * h) {

	//! 取大的x，y
	int newx = b->x > *x ? b->x : *x;
	int newy = b->y > *y ? b->y : *y;
  
	//! 
	int bx = b->x + b->width;
	int by = b->y + b->height;
	int ax = *x + *w;
	int ay = *y + *h;
	int neww = (bx > ax ? ax : bx) - newx;
	int newh = (by > ay ? ay : by) - newy;
  
	*x = newx;
	*y = newy;
	*w = neww;
	*h = newh;
}

//! push 一个裁剪四边形 
void 
scissor_push(int x, int y, int w, int h) {
	assert(S.depth < SCISSOR_MAX);
	shader_flush();                      //! 这会提交一次渲染，drawCall加1
	if (S.depth == 0) {                  //! 如果没有使用深度
		shader_scissortest(1);           //! 开启 裁剪
	}
  
	if (S.depth >= 1) {                                 //! 如果 深度 大于1
		intersection(&S.s[S.depth-1], &x, &y, &w, &h);  //! 求裁剪的交集
	}
  
	struct box * s = &S.s[S.depth++];
	s->x = x;
	s->y = y;
	s->width = w;
	s->height = h;
	screen_scissor(s->x,s->y,s->width,s->height);
}

//! pop 一个裁剪四边形
void 
scissor_pop() {
	assert(S.depth > 0);
	shader_flush();          //! 提交一次 drawCall
	--S.depth;
	if (S.depth == 0) {
		shader_scissortest(0);
		return;
	}
	struct box * s = &S.s[S.depth-1];
	screen_scissor(s->x,s->y,s->width,s->height);
}
