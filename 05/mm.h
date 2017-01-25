#include <stdio.h>




extern int mm_init (void);
extern void *mm_malloc (size_t size);
extern void mm_free (void *ptr);
extern void *mm_realloc(void *ptr, size_t size);
extern void *find_fit(size_t size);
extern void print_heap();
//extern void try_split(free_blk_header_t *fp, size_t needed);

