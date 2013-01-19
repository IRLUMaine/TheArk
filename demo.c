#include "ringbuffer.h"
#include <stdio.h>

int main(int argc, char* argv[]) {
    
    int counter;

    // Standard demo
    ringbuffer_t* rb;
    rb = ringbuffer_init(NULL, sizeof(int), 8, NULL, RINGBUFFER_OPT_NONE);
    if (rb == NULL) {
        printf("Ring Buffer Init Failed!");
        exit(-1);
    }

    counter = 0;
    while(ringbuffer_push(rb, &counter) == RINGBUFFER_OK)
        printf("Pushed: %d\n", counter++);

    while(ringbuffer_pop(rb, &counter) == RINGBUFFER_OK)
        printf("Popped: %d\n", counter);

    ringbuffer_free(rb);

    
    // Demo over overwrite
    rb = ringbuffer_init(NULL, sizeof(int), 8, NULL, RINGBUFFER_OPT_OVERWRITE);
    if (rb == NULL) {
        printf("Ring Buffer Init Failed!");
        exit(-1);
    }

    for(counter = 0; counter < 12; ++counter) {
        if (ringbuffer_push(rb, &counter) != RINGBUFFER_OK) {
            printf("Ring Buffer Push Failed!");
            exit(-1);
        }
        printf("Pushed: %d\n", counter);
    }
    
    while(ringbuffer_pop(rb, &counter) == RINGBUFFER_OK)
        printf("Popped: %d\n", counter);

    ringbuffer_free(rb);

    return 0;
}
