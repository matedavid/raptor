#include "url.h"

namespace network {

URL parse_url(const std::string& url) 
{
  if (url.empty())
    return URL{};

  std::regex url_regex = std::regex("([a-zA-Z]*):\\/\\/(.*)");

  std::smatch match;
  bool result = std::regex_match(url, match, url_regex);
  if (result and match.size() == 3) 
  {
    std::string protocol = match[1];
    std::string path = match[2];

    URLProtocol p;
    if (protocol == "file")
      p = URLProtocol::File;
    else if (protocol == "http")
      p = URLProtocol::Http;
    else if (protocol == "http")
      p = URLProtocol::Https;

    return URL{p, path};
  }

  if (url[0] == '/')
  {
    // Identical to 'file://'
    return URL{URLProtocol::File, url};
  }
  else if (url[0] != '/') 
  {
    // Should default to http protocol if no '/' in the beginning 
    // e.g. localhost:8000 == http://localhost:8000
    return URL{URLProtocol::Http, url};
  }
}

}