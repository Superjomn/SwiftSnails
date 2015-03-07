#ifndef SwiftSnails_core_Message_h_
#define SwiftSnails_core_Message_h_
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

    explicit MetaMessage() { }

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

/*
 * a C++ class wrapper of ZMQ message
 */
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

    // move support
    Message(BasicBuffer &&bb) {
        PCHECK(0 == zmq_msg_init_data(&_zmg, bb.buffer(), bb.size(), self_free, NULL));
        // give buffer's owner to Message
        bb.set_buffer(nullptr);
        bb.clear();
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
    /*
    BasicBuffer& moveTo(BasicBuffer &bb) {
        CHECK(&zmg());
        bb.free();
        bb.reserve(size());
        // copy from zmg
        memcpy(bb.buffer(), &zmg(), size());
        LOG(INFO) << "bb.buffer:\t" << (char*)bb.buffer();
        LOG(INFO) << "bb.buffer()[1]:\t" << bb.buffer()[1];
        bb.set_end(bb.buffer() + size());
        LOG(INFO) << "bb.end:\t" << bb.end();
        bb.set_cursor(bb.buffer());
        return bb;
    }
    */

private:
    zmq_msg_t _zmg;
    // support zmq_msg_init_data's free
    static void self_free(void* data, void* hint) {
        CHECK(data);
        LOG(INFO) << "Message call self_free";
        free(data);
    }
};  // end class Message

};  // end namespace swift_snails
#endif
