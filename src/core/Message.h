#ifndef SwiftSnails_core_Message_h_
#define SwiftSnails_core_Message_h_
#include "../utils/all.h"
#include "common.h"

namespace swift_snails {

typedef std::function<void(int shared_id, BinaryBuffer& ibb, BinaryBuffer& obb)> ShardCallBack;

// every MessageMeta should have a message_class
struct BasicMetaMessage {
    int message_class = 0; // server-side function
    Addr addr;    // to register new worker nodes
};

// meta message of MasterServer and MasterClient
struct MetaMessage : public BasicMetaMessage {
    int client_id = -3;
    int message_id = -1;    // TODO this type ok?

    explicit MetaMessage() { }

    MetaMessage(const MetaMessage &other) {
    	message_class = other.message_class;
    	client_id = other.client_id;
    	message_id = other.message_id;
        addr = other.addr;
    }

    MetaMessage& operator= (const MetaMessage &other) {
    	message_class = other.message_class;
    	client_id = other.client_id;
    	message_id = other.message_id;
        addr = other.addr;
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


struct Request;

/*
 * higher level message package
 */
struct Package : public VirtualObject {
    explicit Package() { };
    Package(Request&);
    Message meta;
    Message cont;

    std::string status() {
        using namespace std;
        stringstream ss;
        ss << "meta.size:\t" << meta.size() << endl;
        ss << "cont.size:\t" << cont.size() << endl;
        return std::move(ss.str());
    }   
};


struct Request {

    typedef std::function<void(std::shared_ptr<Request>)> response_call_back_t;

    explicit Request() { }
    Request(const Request&) = delete;

    Request(Package &&pkg) {
        //LOG(INFO) << "int Request pkg.status:\t" << pkg.status();
        CHECK(pkg.meta.size() == sizeof(MetaMessage));
        // TODO avoid this memory copy
        memcpy(&meta, &pkg.meta.zmg(), sizeof(MetaMessage));
        // copy content
        CHECK(cont.size() == 0);
        cont.set(pkg.cont.buffer(), pkg.cont.size());
        //pkg.cont.moveTo(cont);
    }

    Request(Request &&other) {
        meta = std::move(other.meta);
        cont = std::move(other.cont);
        call_back_handler = std::move(other.call_back_handler);
    }

    int message_id() const {
        return meta.message_id;
    }

    void set_msg_id(int id) {
        meta.message_id = id;
    }
    // to tell whether this message
    // is a request from other node 
    // or response from the receiver
    bool is_response() const {
        return meta.message_class == -1;
    }
    void set_response() {
        meta.message_class = -1;
    }
    /*
     * set node's type: worker / server
     */
    void set_worker() {
        meta.client_id = -1;
    }
    void set_server() {
        meta.client_id = -2;
    }
    bool is_worker() {
        return meta.client_id == -1;
    }
    bool is_server() {
        return meta.client_id == -2;
    }

    ~Request() {
        //LOG(INFO) << "deconstruct Request!";
    }
    // datas
    MetaMessage meta;
    BinaryBuffer cont;
    response_call_back_t call_back_handler;
};

/*
 * zmq network package
 */
Package::Package(Request& request) {
    meta.assign((char*)&request.meta, sizeof(MetaMessage));
    cont.assign(request.cont.buffer(), request.cont.size());
}





};  // end namespace swift_snails
#endif
