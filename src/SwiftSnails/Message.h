#ifndef SwiftSnails_SwiftSnails_Message_h_
#define SwiftSnails_SwiftSnails_Message_h_
#include "../utils/Buffer.h"

namespace swift_snails {

typedef std::function<void(int shared_id, BinaryBuffer& ibb, BinaryBuffer& obb)> ShardCallBack;

// every MessageMeta should have a message_class
struct MessageMetaBase {
    index_t message_class; // server-side function
};


class Message {
public:
    Message() {
        PCHECK(0 == zmq_msg_init(&_zmg));
    }
    Message(const Message&) = delete;

    Message(char& buf, size_t size) {
        PCHECK(0 == zmq_msg_init_size(&_zmg, size));
        memcpy(buffer(), buf, size);
    }

    Message(BasicBuffer &b) : 
        Message(b.buffer(), b.length())
    {}

    ~Message() {
        PCHECK(0 == zmq_msg_close(&_zmg));
    }

    Message& operator=(const Message&) = delete;
    Message& operator=(Message&& other) {
        if(this != &other) {
            PCHECK(0 == zmq_msg_move(&_zmg, &other._zmg));
        }
        return *this;
    }

    size_t length() {
        return zmq_msg_size(&_zmg);
    }

    void reset() {
        PCHECK(0 == zmq_msg_close(&_zmg));
        PCHECK(0 == zmq_msg_init(&_zmg));
    }

    void resize(size_t newsize) {
        PCHECK(0 == zmq_msg_close(&_zmg));
        PCHECK(0 == zmq_msg_init_size(&_zmg, newsize));
    }

    void assign(char* buf, size_t size) {
        resize(size);
        memcpy(buffer(), buf, size);
    }

    char* buffer() {
        return (char*)zmq_msg_data(&_zmg);
    }

    zmq_msg_t& zmg() {
        return _zmg;
    }

private:
    zmq_msg_t _zmg;
};  // end class Message

};  // end namespace swift_snails
#endif
