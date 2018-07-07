#ifndef ejoy3d_array_h
#define ejoy3d_array_h

//! 

struct array_node;

struct array {
	int n;                      //! n个元素
	int sz;                     //! 每个元素的size
	char * buffer;              //! 该数组的buffer数据
	struct array_node * freelist;         //! 链表头，方便使用链表
};

int array_size(int n, int sz);                                    //! n个元素， 每个元素大小为sz
void array_init(struct array *p, void * buffer, int n, int sz);   //! 
void * array_alloc(struct array *p);
void array_free(struct array *p, void *v);
void array_exit(struct array *p, void (*close)(void *p, void *ud), void *ud);

int array_id(struct array *p, void *);
void * array_ref(struct array *p, int id);

#endif
