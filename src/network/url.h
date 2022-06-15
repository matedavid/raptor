#include <iostream>
#include <string>
#include <regex>

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
};

URL parse_url(const std::string& url);

}