//
// Created by lg on 18-3-13.
//

#include "TcpClient.h"
#include"TcpConnection.h"
#include "Log.h"

namespace net{
    using std::placeholders::_1;
    using std::placeholders::_2;
    std::atomic<uint64_t> TcpClient::id{0};

    TcpClient::TcpClient(EventLoop *loop, const InetAddress &serverAddr, const std::string &nameArg)
    :_loop(loop)
     ,_connector(loop,serverAddr)
    ,_peer_addr(serverAddr)
    ,_name(nameArg)
    ,_connection(nullptr)
    ,_retry(false)
    ,_status(Disconnected)
    {
        _connector.set_new_connection_cb(std::bind(&TcpClient::on_new_connection,this,_1,_2));
    }

    TcpClient::~TcpClient() {

    }

    void TcpClient::connect() {
        assert(_status==Disconnected);

        Status t=Disconnected;
        if(_status.compare_exchange_strong(t,Connecting)){
            _connector.start();
        }
    }

    void TcpClient::disconnect() {
        //assert(_status==Connecting);

        Status t=Connecting;
        if(_status.compare_exchange_strong(t,Disconnected)){
            _connection->close();
        }
    }

    void TcpClient::stop() {
        Status t=Connected;
        if(_status.compare_exchange_strong(t,Disconnecting)){
            _connector.cancel();
        }
    }

    void TcpClient::retry() {

    }

    void TcpClient::on_new_connection(int fd, InetAddress addr) {

        _connection.reset(new TcpConnection(++id,_loop, fd,addr,_peer_addr));

        _connection->set_message_cb(_message_cb);
        _connection->set_connection_cb(_connecting_cb);
        _connection->set_close_cb(std::bind(&TcpClient::on_remove_connection, this, _1));

        _loop->run_in_loop(std::bind(&TcpConnection::attach_to_loop, _connection));

        LOG_TRACE<<"Client new conn";
    }

    void TcpClient::on_remove_connection(const TCPConnPtr &conn) {
        _status=Disconnected;
    }
}