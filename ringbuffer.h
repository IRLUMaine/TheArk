#ifndef RING_BUFFER_H_
#define RING_BUFFER_H_

#include <stdlib.h>
#include <stdint.h>

#define RINGBUFFER_OPT_NONE        0x00000000
#define RINGBUFFER_OPT_OVERWRITE   0x00000001

#define RINGBUFFER_OK           0
#define RINGBUFFER_EMPTY        1
#define RINGBUFFER_FULL         2
#define RINGBUFFER_INVALID      3

typedef struct ringbuffer {
  void      *_buffer;
  void      *_buffer_end;
  void      *_head;
  void      *_tail;

  uint32_t  _options;
  uint32_t  _flags;

  size_t    _capacity;
  size_t    _count;
  size_t    _data_sz;

} ringbuffer_t;



ringbuffer_t* ringbuffer_init(ringbuffer_t *rb, size_t const data_sz, size_t const capacity, void const * const storage, uint32_t const options);

void ringbuffer_free(ringbuffer_t *rb);

void ringbuffer_clear(ringbuffer_t * const rb);

int ringbuffer_push(ringbuffer_t * const rb, void const * const item);

int ringbuffer_pop(ringbuffer_t * const rb, void * const item);

size_t ringbuffer_count(ringbuffer_t const * const rb);

#endif
