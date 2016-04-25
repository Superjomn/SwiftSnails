#include <climits>
#include "../../utils/Buffer.h"
#include "../../core/Message.h"
#include "gtest/gtest.h"
using namespace std;
using namespace swift_snails;

TEST(Message, create_and_destroy) { Message message; }

TEST(Message, init_from_BinaryBuffer) {
  BinaryBuffer bb;
  int a = 123;
  float b = 1.23;
  bb << a;
  bb << b;

  // create Message
  Message message(*bb.buffer(), bb.size());

  // decode data in message
  BinaryBuffer cc;
  cc.set(message.buffer(), message.size());

  int _a;
  float _b;
  cc >> _a;
  cc >> _b;

  ASSERT_EQ(a, _a);
  ASSERT_EQ(b, _b);
}

TEST(Message, construct_from_BasicBuffer) {
  BinaryBuffer bb;
  int a = 123;
  float b = 1.23;
  bb << a;
  bb << b;

  // create Message
  Message message(bb);
}

TEST(Message, construct_move_BasicBuffer) {
  BinaryBuffer bb;
  int a = 123;
  float b = 1.23;
  bb << a;
  bb << b;

  // create Message
  Message message(std::move(bb));

  // decode data in message
  BinaryBuffer cc;
  cc.set(message.buffer(), message.size());

  int _a;
  float _b;
  cc >> _a;
  cc >> _b;

  ASSERT_EQ(a, _a);
  ASSERT_EQ(b, _b);
}

TEST(MetaMessage, add_IP) {
  MetaMessage meta;
  meta.message_class = -1;
  meta.client_id = 23;
  meta.message_id = 1232;
  meta.addr.from_string("tcp://127.0.0.1:8080");

  MetaMessage other = meta;

  ASSERT_EQ(meta.addr.port, 8080);
  ASSERT_EQ(meta.addr.addr[0], 127);
  ASSERT_EQ(meta.addr.addr[1], 0);
  ASSERT_EQ(meta.addr.addr[2], 0);
  ASSERT_EQ(meta.addr.addr[3], 1);

  ASSERT_EQ(meta.addr.port, other.addr.port);

  for (int i = 0; i < 4; i++) {
    ASSERT_EQ(meta.addr.addr[i], other.addr.addr[i]);
  }
}
