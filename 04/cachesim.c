#include <stdio.h>
#include <stdlib.h>
int log_base_2 (int lg);




//Create one line in char
//Using index, valid, tag, and data
typedef struct {
int index;
int valid;
int tag;
int lru; 
} cache_line;

//Create cache
//Pointer to a cache_line struct
typedef cache_line *cache;

void miss_cache(cache cac, int index, int tag, int number_lines);
void update(cache cach, int index, int location, int number_lines);


int main (int argc, char *argv[]) {
    int num_lines = atoi(argv[1]),
        lines_per_set = atoi(argv[2]),
        bytes_per_block = atoi(argv[3]);
    //Creates a cache variable
    //Size of cache=the number of lines
    cache c=malloc(sizeof(cache_line) * num_lines);
    int i;
    //Iterates through the cache
    //Sets each valid, tag, and data field to 0.
    //Iterates throu
    int count=0;
    for(i=0;i<num_lines;i++)
    {
        
        if((i!=0) && (i%lines_per_set)==0)
        {        
            count++;
        }
        c[i].index=count;
        c[i].valid=0;
        c[i].tag=0;
        

    }




    char line[80];

    long addr_req;

    /*printf("Simulating cache with:\n");
    printf(" - Total lines   = %d\n", num_lines);
    printf(" - Lines per set = %d\n", lines_per_set);
    printf(" - Block size    = %d bytes\n", bytes_per_block);*/

    int n = 0;
    //int value=0;
    int block_size;
    int index_size;
    int index;
    int tag;
    int mask;
    int hit_counter=0;
    int miss_counter=0;
    char hit;
    while (fgets(line, 80, stdin)) {
        //Uses log_base_2 to create the block_size and index_size
        //Shifts the address request by the block size
        //Creates the mask and shits it to the left, and then subtracts it by 1
        //Uses the mask and the address request to create the index
        //The tage equals the address request, shifted by the index_size, to the right. 
        addr_req = strtol(line, NULL, 0);
        block_size=log_base_2(bytes_per_block);
        index_size=log_base_2(num_lines/lines_per_set);
        addr_req >>= block_size;
        mask=1;
        mask <<=index_size;
        mask--;
        index=mask & addr_req;
        tag=addr_req>>index_size;
        hit=0;
        //printf("%x, %x, %x \n", addr_req, index, tag);
        int j;
        for(j=0;j<num_lines;j++)
        {
            if(c[j].valid)
            {
                if(c[j].index==index)
                {
                    if(c[j].tag==tag)
                    {
                           hit_counter++; 
                           update(c, index, j, num_lines);
                           hit=1;
                    }
                }

            }
        }
        if(hit==0)
        {

                miss_cache(c, index, tag, num_lines);
                miss_counter++;
        }
        n++;

        /* simulate cache fetch with address `addr_req` */
        //printf("Processing request: 0x%lX\n", addr_req);

    }
    printf("Hit: %d Miss: %d \n", hit_counter, miss_counter);
    double total=hit_counter+miss_counter;
    printf("Hit Rate: %.2f%% \n", (((double)hit_counter/total)*100.0));
    printf("Miss Rate: %.2f%% \n", (((double)miss_counter/total)*100.0));
    return 0;

}

int log_base_2(int lg)
{
    int i=0;
    while(lg >>=1)
    {
        i++;
    }
    return i;
}

void miss_cache(cache cac, int index, int tag, int number_lines)
{
    int i;
    int small_lru=1<<30;
    int location;
    //cache c=malloc(sizeof(cache_line) * number_lines); 
    for(i=0;i<number_lines;i++)
    {
        if(cac[i].index==index)
        {

           //Use lru to place things in the cache
           //Use lru
           //cac[i].lru++;

           if(cac[i].lru<small_lru)
           {
                small_lru=cac[i].lru;
                location=i;
                

                //replace cache_line with add_req
                



           }

            

        }
        cac[location].tag=tag;
        cac[location].valid=1;
        update(cac, index, location, number_lines);
        //cac[location].lru=;



    }


}

void update(cache cach, int index, int location, int number_lines)
{
    int i;
    int big_lru=-1;
    //Goes to location
    //Goes through everything that matches taht index
     for(i=0;i<number_lines;i++)
    {
        if(cach[i].index==index)
        {

           //Use lru to place things in the cache
           //Use lru
           //cac[i].lru++;

           if(cach[i].lru>big_lru)
           {
                big_lru=cach[i].lru;
                

                //replace cache_line with add_req
            

           }

            

        }
            cach[location].lru=big_lru+1;
   }
}

