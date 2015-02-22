#include "../../../core/client/BasicClient.h"
#include "../../../core/server/BasicServer.h"
using namespace std;
using namespace swift_snails;

/*
 * client's data structure
 */
struct IP_msg {
    IP ip;
    friend BinaryBuffer& operator<< (BinaryBuffer& bb, IP_msg &ip) {
    	bb << ip.ip;
        return bb;
    }
    friend BinaryBuffer& operator>> ( BinaryBuffer& bb , IP_msg &ip) {
    	bb >> ip.ip;
        return bb;
    }
};

BasicServer& local_server() {
    static BasicServer server;
    return server;
}

BasicClient& local_client() {
    static BasicClient client;
    return client;
}

void init_client() {
    // create channel
    AsynExec as(2);
    auto global_channel = as.open();
    LOG(INFO) << "init client";
    local_client().set_recv_ip(get_local_ip());
    // response handler
    AsynExec asyn_exec(4);
    local_client().set_async_channel(asyn_exec.channel());
    local_client().listen();
    // register server's address and connect
    std::string server_addr;
    format_string(server_addr, "tcp://%s:%d", get_local_ip().c_str(), 80805);
    local_client().register_node(0, server_addr);
    local_client().connect_all();
    local_client().set_async_channel(global_channel);

    local_client().set_listen_thread_num(2);
    local_client().initialize();
}

void client_send_IP() {
	// prepare ip message content
	IP_msg ip;
	std::string ip_port;
    std::string recv_ip = local_client().recv_ip();
	format_string(ip_port, "%s:%d", recv_ip.c_str(), local_client().recv_port());
    ip.ip.from_string(ip_port);
	// package request
	Request request;
    request.meta.message_class = 1;
    request.cont << ip;
    // prepare response handler
    request.call_back_handler = [] (std::shared_ptr<Response> response) {
    	LOG(INFO) << "receive response: get client_id" << response->meta.client_id;
    	LOG(INFO) << "receive response: get message_id" << response->meta.message_id;
    };
    local_client().send(request, 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    LOG(INFO) << "client sended request";
}

void init_server() {
    AsynExec as(2);
    auto global_channel = as.open();
    LOG(INFO) << "init server";
    int port = 80805;
    std::string addr;
    std::string ip = get_local_ip();
    format_string(addr, "tcp://%s:%d",  ip.c_str(), port);
    local_server().listen(addr);
    local_server().set_listen_thread_num(2);
    local_server().initialize();
    local_server().set_async_channel(global_channel);
}

// server should register the message class
// and use the corresponding handler to process request
void server_register_message_class() {
	BasicServer::Handler handler = [](std::shared_ptr<Request> request) {
		LOG(INFO) << "handle a request";
        LOG(INFO) << "try to get meta";
        LOG(INFO) << "message_class:\t" << request->meta.message_class;
		IP_msg ip;
        LOG(INFO) << "request cont info:" << request->cont.status();
		request->cont >> ip;
		int client_id = 1;
		std::string ip_port = ip.ip.to_string();
		LOG(INFO) << "client's ip/port is\t" << ip_port;
		// TODO should add an id controller
		std::string addr = "tcp://" + ip_port;
		LOG(INFO) << "client's address is\t" << addr;
		LOG(INFO) << "to register client";
		local_server().register_node(client_id, addr);
		LOG(INFO) << "to connect client";
		local_server().connect(client_id);

		// prepare the reply
		Request reply;
		reply.meta.client_id = client_id;
		reply.meta.message_class = request->meta.message_class;
		reply.meta.message_id = request->meta.message_id;
		reply.cont << (int) client_id;

		// send message
		LOG(INFO) << "to send reply to client";
		local_server().send(std::move(reply), client_id);
	};
	LOG(INFO) << "server register message_class: 1/ip_connect";
	local_server().register_message_class(1, std::move(handler));
}
