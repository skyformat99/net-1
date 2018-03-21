//
// Created by lg on 17-4-18.
//

#include <arpa/inet.h>

#include "Log.h"
#include <cstring>
#include "InetAddress.h"

namespace net {



    InetAddress::InetAddress(const std::string & ip, in_port_t port,sa_family_t family)
    {
        std::memset(&_addr,0, get_sockaddr_size());

        _addr.sin_family = family;
        _addr.sin_port = ::htons(port);

        if (::inet_pton(AF_INET, ip.c_str(), &_addr.sin_addr) <= 0)
        {
            LOG_ERROR << "inet_pton 失败";
        }
    }

    std::string InetAddress::toIp() const
    {
        return std::string();
    }

    std::string InetAddress::toIpPort() const
    {
        return std::string();
    }

    in_port_t InetAddress::toPort() const
    {
        return in_port_t();
    }

    uint32_t InetAddress::ipNetEndian() const
    {
        return uint32_t();
    }



}