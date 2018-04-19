#include <iostream>
#include"EventLoop.h"
#include"TcpServer.h"
#include"Buffer.h"
#include"TcpConnection.h"
#include"Log.h"

using namespace std;
using namespace net;
using namespace std::placeholders;

class ChatServer{
public:
    ChatServer(EventLoop*loop,const InetAddress&addr)
            :loop(loop)
            ,ser(loop,addr,"ChatServer",0){
        ser.set_connection_cb(std::bind(&ChatServer::onConnection,this,_1));
        ser.set_message_cb(std::bind(&ChatServer::onMessage,this,_1,_2));
    }

    void start(){
        ser.run();
    }

    void stop(){
        ser.stop();
    }

private:
    void onConnection(const TCPConnPtr &conn){
        LOG_INFO<<conn->get_peer_addr().toIp()<<":"<<conn->get_peer_addr().toPort()<<" is "
                <<((conn->is_connected())?"up":"down");

        if(conn->is_connected()) {
            conn->set_tcp_no_delay(true);
            conns.insert(conn);
        }
        else
            conns.erase(conn);
    }

    void onMessage(const TCPConnPtr &conn,Buffer*message){
        
        for(auto&t:conns) {
            if (t != conn) {
                t->send(message->get_read_ptr(),message->get_readable_size());
            }
        }
        message->clear();
    }

    using ConnectionList= std::set<TCPConnPtr >;
    EventLoop*loop;
    TcpServer ser;
    ConnectionList conns;
};

int main()
{
    Log::set_rank(2);

    EventLoop loop;
    InetAddress addr("127.0.0.1", 55555);
    ChatServer c(&loop, addr);

    c.start();

    loop.run();

std::this_thread::sleep_for(5s);
    c.start();
    loop.run_after(8s,[&c,&loop]{c.stop();
        loop.stop();
    });
    loop.run();

}