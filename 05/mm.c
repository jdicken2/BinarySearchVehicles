
 /* mm-naive.c - The fastest, least memory-efficient malloc package.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include "mm.h"
#include "memlib.h"


/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

#define NUM_SIZE_CLASSES 5

//Initialize the header and footer
typedef struct header blkhdr;
typedef struct footer blkftr;

struct header{
  size_t size;
  blkhdr *next;
  blkhdr *prior;
};

struct footer{
  size_t size;
};

//Random comment so I can push again


/*
 * mm_init - initialize the malloc package.
 */

//Global Arry of Pointers to doubly-linked lists
#define MIN_BLK_HDR_SIZE ALIGN(sizeof(blkhdr))
#define MIN_BLK_FTR_SIZE ALIGN(sizeof(blkftr))

//blkhdr *free_lists;
//size_t min_class_size[] = { MIN_BLK_SIZE, 64, 128, 256, 1024 };
/*void *coalesce(size_t *bp) {
 size_t *next = (char *)bp + (*bp & ~1L),
 *prev = (char *)bp - (*(size_t *)((char *)bp-SIZE_T_SIZE) & ~1L);
 int next_alloc = *next & 1,
 prev_alloc = *prev & 1,
 if (prev_alloc && next_alloc) {
  return bp;
 } else if (!prev_alloc && next_alloc) {
   *prev += *bp; // header
  *(size_t *)((char *)bp + *bp - SIZE_T_SIZE) = *prev; // footer
  return prev;
  } else if (prev_alloc && !next_alloc) {
 ...
 } else {
 ...
 }*/
int mm_init(void)
{

  //Perfroms any necessary initializations
  //such as initial heap
  //return -1 if there is a problem, otherwise 0
  //So I need to initiate the header, heap, and footer
  /*int i;
 free_lists = sbrk(NUM_SIZE_CLASSES * sizeof(free_blk_header_t));
 for (i=0; i<NUM_SIZE_CLASSES; i++) {
    free_lists[i].size = 1;
    free_lists[i].next = free_lists[i].prior = &free_lists[i];*/

    blkhdr *hp=mem_sbrk(MIN_BLK_HDR_SIZE);
    hp->size=MIN_BLK_HDR_SIZE;
    hp->next=hp;
    hp->prior=hp;

    blkftr *fp=mem_sbrk(MIN_BLK_FTR_SIZE);
    fp->size=MIN_BLK_FTR_SIZE;
    return 0;
 }

 /*int j;
 free_lists = sbrk(NUM_SIZE_CLASSES * sizeof(free_blk_footer_t));
 for (j=0; i<NUM_SIZE_CLASSES; j++) {
    free_lists[j].size = 2;
    free_lists[j].next = free_lists[i].prior = &free_lists[i];
 }*/

  //}

void print_heap(){
  blkhdr *bp=mem_heap_lo();
  while(bp < (blkhdr *)mem_heap_hi()){
      printf("%s block at %p, size %d\n", 
            (bp->size&1)?"allocated":"free",
              bp,
              (int)(bp->size));
      bp=(blkhdr *)((char *)bp + (bp->size & ~1));


   

  }
}


/*
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
  int newsize=ALIGN(size + MIN_BLK_HDR_SIZE);
  blkhdr *bp=find_fit(newsize);
  if (bp==NULL)
  {
    bp=mem_sbrk(newsize);
    if((long)bp==-1)
    {
      return NULL;
    }
    else{
      /**(size_t *)p=size;
      return (void *)((char *)p +SIZE_T_SIZE);*/
      bp->size=newsize|1;
    }
   
  }
  else{

    bp->size |=1;
    bp->prior->next=bp->next;
    bp->next->prior=bp->prior;
  }

  blkftr *fp=find_fit(newsize);
  if(fp==NULL)
  {
    fp=mem_sbrk(newsize);
    if((long)fp==-1)
    {
      return NULL;
    }
    else
    {
      fp->size=newsize|1;
    }
  }
  else{
    fp->size|=1;
  }
  /*int newsize = ALIGN(size + MIN_BLK_SIZE);
  free_blk_header_t *new_block=find_fit(newsize);
  if((new_block)==NULL){
<<<<<<< HEAD
    free_blk_header_t *p = find_fit(newsize);
    p=mem_sbrk(newsize);
    //((free_blk_header_t *)p)->size=newsize|1;
    if ((long)p == -1)
    {
      return NULL;
    }
    else
    {
        new_block->size=newsize|1;
      
    }

    
  }
  else {
    return (void *)((char *)new_block + MIN_BLK_SIZE);
    return NULL;
    free(new_block);
    new_block->size|=1;
    new_block->prior->next=new_block->next;
    new_block->next->prior=new_block->prior;
  }
 return (char *)new_block + MIN_BLK_SIZE;*/
  /*return (char*) bp + MIN_BLK_HDR_SIZE;
    void *p = mem_sbrk(newsize);
    if ((long)p == -1)
      return NULL;
    ((free_blk_header_t *)p)->size=newsize|1;
    return (void *)((char *)p + MIN_BLK_SIZE);
  }
  else {
    return (void *)((char *)new_block + MIN_BLK_SIZE);
    }*/

    return (char *)bp + MIN_BLK_HDR_SIZE;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
  blkhdr *bp=ptr-MIN_BLK_HDR_SIZE,
         *head=mem_heap_lo();
  bp->size&=~1;
  bp->next=head->next;
  bp->prior=head;
  head->next=bp;
  bp->next->prior=bp;

  blkftr *fp=ptr-MIN_BLK_FTR_SIZE;
  fp->size&=~1;


  //Check the prior and next block and see if they're free


    /*
    ptr->f_block;
    free(f_block);*/
  
/*blkHdr *header = (free_blk_header_t *) ((char *)ptr - SIZE_T_SIZE),
=======
  free_blk_header_t *header = (free_blk_header_t *) ((char *)ptr - SIZE_T_SIZE),
>>>>>>> refs/remotes/origin/master
 *free_list_head = mem_heap_lo();
 // add freed block to free list after head
 header->size = *(size_t *)header & ~1L;
 // add freed block to free list after head
 header->next = free_list_head->next;
 header->prior = free_list_head;
 free_list_head->next = free_list_head->next->prior = header;*/



/*
  free_blk_header_t *header = (free_blk_header_t *) ((char *)ptr - SIZE_T_SIZE);
 // add freed block to free list after head

 header->size = *(size_t *)header & ~1L;


 int i;
 for (i=0; i<NUM_SIZE_CLASSES; i++) {
 // locate the first suitable list that isn’t empty
    if (min_class_size[i] >= header->size) {
      // add freed block to free list after head
      header->next = free_lists[i].next;
      header->prior = &free_lists[i];
      free_lists[i].next = free_lists[i].next->prior = header;*/
  }




/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{

  blkhdr *bp=ptr-MIN_BLK_HDR_SIZE;
  void *newptr=mm_malloc(size);
  if(newptr==NULL)
    return NULL;
  int copySize=bp->size-MIN_BLK_HDR_SIZE;
  if(size < copySize)
     copySize=size;
  memcpy(newptr, ptr, copySize);
  mm_free(ptr);
  return newptr;

  blkhdr *fp=ptr-MIN_BLK_FTR_SIZE;
  if(newptr==NULL)
    return NULL;
  int copySize_f=fp->size-MIN_BLK_FTR_SIZE;
  if(size < copySize_f)
     copySize_f=size;
  memcpy(newptr, ptr, copySize_f);
  mm_free(ptr);
  return newptr;

  

 /*
  void *oldptr = ptr;
  void *newptr;
  size_t copySize;

  newptr = mm_malloc(size);
  if (newptr == NULL)
    return NULL;
  copySize = *(size_t *)((char *)oldptr - SIZE_T_SIZE);
  if (size < copySize)
    copySize = size;
  memcpy(newptr, oldptr, copySize);
  mm_free(oldptr);
  return newptr;*/
  //return NULL;
}

/*void try_split(blkhdr *fp, size_t needed) {
  int i, remaining = fp->size - needed;
  blkhdr *sp;
  if (remaining < MIN_BLK_HDR_SIZE)
    return;
 // split the block ...
  fp->size = needed;
  sp = (blkhdr *)((char *)fp + needed);
  sp->size = remaining;
 // ... and put the leftover free block in the correct list
 for (i=NUM_SIZE_CLASSES-1; i>0; i--)
  if (min_class_size[i] <= remaining) {
    sp->prior = &free_lists[i];
    sp->next = free_lists[i].next;
    free_lists[i].next = free_lists[i].next->prior = sp;
    break;
  }
}*/

void *find_fit(size_t size) {
 blkhdr *p;
 for(p=((blkhdr *)mem_heap_lo())->next;
     p != mem_heap_lo() && p->size<size;
     p=p->next);
  if (p != mem_heap_lo())
      return p;
  else
    return NULL;

 /*free_blk_header_t *p
 for(p=((free_blk_header_t *)mem_heap_lo())->next;
    p!=mem_heap_lo()&&p->size<size;
    p=p->next);
  if(p != mem_heap_lo())
  {
    return p;
  }
  else
  {
    return NULL;
  }*/   
 /*int i;
 free_blk_header_t *fp;
 for (i=0; i<NUM_SIZE_CLASSES; i++) {
 // locate the first suitable list that isn’t empty
    if (min_class_size[i] >= size
        && free_lists[i].next != &free_lists[i]) {
 // take the first block (no searching!)
      fp = free_lists[i].next;
 // remove it from the free list
      free_lists[i].next = fp->next;
      fp->next->prior = &free_lists[i];
 // and try to split it
      try_split(fp, size);
      return fp;
    }
  }
  return NULL;*/


}

void *coalesce (blkhdr *hp){
  blkhdr *next, *prev;
  //hp->size=size_c;
  //int next_alloc, prev_alloc;
  if((blkhdr *)mem_heap_hi()<hp)
  {
    
    prev=(blkhdr *)hp-(*(size_t *) ((char *)hp-SIZE_T_SIZE) & ~1L);
    prev_alloc=*prev & 1;
    hp->prior->next=hp->next;
    hp->next->prior=hp->prior;
    
  }
  /*else
  {
    prev_alloc=1;
  }*/
  if((blkhdr *)mem_heap_hi()>hp)
  {
    next=(char *)hp-(*(size_t *) ((char *)hp+SIZE_T_SIZE) & ~1L);
    next_alloc=*prev & 1;
  }
  /*else
  {
    next_alloc=1;
  }*/
}
