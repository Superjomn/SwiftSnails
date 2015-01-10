#ifndef SwiftSnails_SwiftSnails_Message_h_
#define SwiftSnails_SwiftSnails_Message_h_
#include "../utils/common.h"
#include "../utils/Buffer.h"

namespace swift_snails {

typedef std::function<void(int shared_id, BinaryBuffer& ibb, BinaryBuffer& obb)> ShardCallBack;

// every MessageMeta should have a message_class
struct BasicMetaMessage {
    index_t message_class = 0; // server-side function
};

// meta message of MasterServer and MasterClient
struct MetaMessage : public BasicMetaMessage {
    index_t client_id = 0;
    index_t message_id = 0;

    MetaMessage(const MetaMessage &other) {
    	message_class = other.message_class;
    	client_id = other.client_id;
    	message_id = other.message_id;
    }

    MetaMessage& operator= (const MetaMessage &other) {
    	message_class = other.message_class;
    	client_id = other.client_id;
    	message_id = other.message_id;
    	return *this;
    }
};


class Message {
public:
    Message() {
        PCHECK(0 == zmq_msg_init(&_zmg));
    }
    Message(const Message&) = delete;

    Message(char& buf, size_t size) {
        PCHECK(0 == zmq_msg_init_size(&_zmg, size));
        memcpy((void*)buffer(), &buf, size);
    }

    Message(BasicBuffer &b) {
    	PCHECK(0 == zmq_msg_init_size(&_zmg, b.size()));
    	memcpy((void*)buffer(), b.buffer(), b.size());
    }

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

    size_t size() {
        return zmq_msg_size(&_zmg);
    }

    bool empty() {
        return size() <= 0;
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
