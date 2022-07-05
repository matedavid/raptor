#include <iostream>
#include <string>
#include <regex>
#include <netdb.h>
#include <iterator>
#include <arpa/inet.h>

namespace network {

enum URLProtocol
{
  Local,
  Http,
  Https
};

struct URL
{
  URLProtocol protocol;
  std::string path;
  std::string domain;
};

URL parse_url(const std::string& url);

std::string resolve_hostname(const std::string& hostname);

}
