#include <string.h>
#include "ringbuffer.h"

#define NEED_TO_FREE_SELF       0x00000001
#define NEED_TO_FREE_BUFFER     0x00000002


ringbuffer_t* ringbuffer_init(ringbuffer_t* rb, size_t data_sz, size_t capacity, void* storage, uint32_t options) {

  if (rb == NULL) {
    rb = malloc(sizeof(ringbuffer_t));
    if (rb == NULL) {
        return NULL;
    }
    rb->_flags = NEED_TO_FREE_SELF;
  } else {
    if (rb->_buffer != NULL)
      return NULL;  // Why are you trying to initialize a ringbuffer that already has storage?
    rb->_flags = 0;
  }

  if (storage != NULL) {
    rb->_buffer = storage;
  } else {
    rb->_buffer = malloc(data_sz * capacity);
    if (rb->_buffer == NULL) {
        ringbuffer_free(rb);
        return NULL;
    }
    rb->_flags |= NEED_TO_FREE_BUFFER;
  }

  rb->_head = rb->_buffer;
  rb->_tail = rb->_buffer;
  rb->_buffer_end = (char *)rb->_buffer + (capacity * data_sz);

  rb->_count = 0;
  rb->_capacity = capacity;
  rb->_data_sz = data_sz;

  rb->_options = options;

  return rb;
}

void ringbuffer_free(ringbuffer_t *rb) {

  // Nothing to free here...
  if ( rb == NULL ) {
    return;
  }

  // Cleanup buffer storage.
  if ( (rb->_buffer != NULL) && (rb->_flags & NEED_TO_FREE_BUFFER) )  {
    free(rb->_buffer);
    rb->_buffer = NULL;
  }

  // Clean up the data structure holding the buffer.
  if (rb->_flags & NEED_TO_FREE_SELF) {
    free(rb);
  }

  return;
}

void ringbuffer_clear(ringbuffer_t *rb) {
  if (rb) {
    rb->_count = 0;
    rb->_head = rb->_buffer;
    rb->_tail = rb->_buffer;
  }
  return;
}


int ringbuffer_push(ringbuffer_t *rb, void *item) {
  if (!rb || !item) {
    return -1;
  }

  if ( (rb->_count == rb->_capacity) && !(rb->_options & RINGBUFFER_OVERWRITE) ) {
    return 0;
  }

  memcpy(rb->_head, item, rb->_data_sz);

  rb->_head = (char*)rb->_head + rb->_data_sz;
  if (rb->_head == rb->_buffer_end) {
    rb->_head = rb->_buffer;
  }

  if (rb->_count == rb->_capacity) {  // If we just overwrote the tail, update tail pointer.
    rb->_tail = (char*)rb->_tail + rb->_data_sz;
    if (rb->_tail == rb->_buffer_end) {
      rb->_tail = rb->_buffer;
    }
  } else {
    ++rb->_count;  // Count must be less than capacity.
  }

  return 1;
}



int ringbuffer_pop(ringbuffer_t *rb, void *item){
  if (!rb || !item) {
    return 0;
  }

  if (rb->_count == 0) {
    return 0;
  }

  if (item != NULL) {
    memcpy(item, rb->_tail, rb->_data_sz);
  }

  rb->_tail = (char*)rb->_tail + rb->_data_sz;

  if (rb->_tail == rb->_buffer_end) {
    rb->_tail = rb->_buffer;
  }

  --rb->_count;

  return 1;
}

size_t ringbuffer_count(ringbuffer_t *rb) {
  if (!rb) {
    return 0;
  }
  return rb->_count;
}
