#include "mean_filter.h"
#include "board_config.h"

int mean_filter_init(mean_filter_pt filter, double *buffer, size_t max_capacity)
{
    if (max_capacity < 2)
    {
        return -1;
    }
    filter->buffer = buffer;
    memset(filter->buffer, 0, max_capacity);
    filter->capacity = max_capacity;
    mean_filter_reset(filter);
    return 0;
}
int mean_filter_add(mean_filter_pt filter, double data)
{
    filter->buffer[filter->position] = data;
    filter->position++;
    filter->position %= filter->capacity;
    filter->size++;
    if (filter->size >= filter->capacity)
    {
        filter->size = filter->capacity;
    }
    return 0;
}

int mean_filter_out(mean_filter_pt filter)
{
    if (filter->size == 0)
    {
        return -1;
    }
    filter->size--;
    return 0;
}
#ifdef SUPPORT_PUMP
int mean_filter_get(mean_filter_pt filter, double *r_mean)
{
    double mean = 0;
    int count = mean_filter_count(filter);
    if (count <= 0)
    {
        return -1;
    }
    int p = 0;
    for (int i = 0; i < count; i++)
    {
        p = ((filter->position - i - 1) + filter->capacity) % filter->capacity;
        mean += filter->buffer[p] / count;
    }
    *r_mean = mean;
    return 0;
}
#else
int mean_filter_get(mean_filter_pt filter, double *r_mean)
{
    double mean = 0;
    int count = mean_filter_count(filter);
    if (count <= 2)
    {
        return -1;
    }
    int p = 0;
    double min = filter->buffer[((filter->position-1)+ filter->capacity)% filter->capacity];
    double max =filter->buffer[((filter->position-1)+ filter->capacity)% filter->capacity];
    for (int i = 0; i < count; i++)
    {
        p = ((filter->position - i - 1) + filter->capacity) % filter->capacity;
        if(min> filter->buffer[p])
        {
            min=filter->buffer[p];    
        }
        if(max< filter->buffer[p])
        {
            max=filter->buffer[p];    
        }
        mean += filter->buffer[p] / count;
    }
    mean = mean-(min/count)-(max/count);
    mean = mean*count/(count-2);
    *r_mean = mean;
    return 0;
}
#endif

int mean_filter_reset(mean_filter_pt filter)
{
    filter->position = 0;
    filter->size = 0;
    return 0;
}

int mean_filter_count(mean_filter_pt filter)
{
    return filter->size;
}

int max_min_get(uint16_t *buffer, uint8_t Len)
{
    uint8_t i;
    uint16_t temp;
    uint16_t min =buffer[0];
    uint16_t max =buffer[0];
    if (Len <= 2)
    {
        return 0;
    }
	for(i=0;i<Len;i++)
    {
        if(min>buffer[i])
        {
            min=buffer[i];    
        }
        if(max< buffer[i])
        {
            max=buffer[i];    
        }
    }
    temp=max-min; 
    return  temp;
}
