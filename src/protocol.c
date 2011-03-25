#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "protocol.h"
#include "network.h"

#include "plugins/obfs2.h"
#include "plugins/dummy.h"

/**
    This function returns a protocol_t structure based on the mode
    of obfsproxy
*/
struct protocol_t *
set_up_protocol(int protocol) {
  struct protocol_t *proto = calloc(1, sizeof(struct protocol_t));

  if (protocol == OBFS2_PROTOCOL)
    proto->new = &obfs2_new;
  else if (protocol == DUMMY_PROTOCOL)
    proto->new = &dummy_new;
  /* elif { other protocols } */

  if (proto->new(proto)>0)
    printf("Protocol constructed\n");

  return proto;
}

void *
proto_init(struct protocol_t *proto, void *arg) {
  assert(proto);
  if (proto->init)
    return proto->init(arg);
  else
    return NULL;
}

int
proto_handshake(struct protocol_t *proto, void *buf) {
  assert(proto);
  if (proto->handshake)
    return proto->handshake(proto->state, buf);
  else /* It's okay with me, protocol didn't have a handshake */
    return 0;
}

int
proto_send(struct protocol_t *proto, void *source, void *dest) {
  assert(proto);
  if (proto->send)
    return proto->send(proto->state, source, dest);
  else 
    return -1;
}

int
proto_recv(struct protocol_t *proto, void *source, void *dest) {
  assert(proto);
  if (proto->recv)
    return proto->recv(proto->state, source, dest);
  else
    return -1;
}

void proto_destroy(struct protocol_t *proto) {
  assert(proto);
  assert(proto->state);

  if (proto->destroy)
    proto->destroy(proto->state);
}