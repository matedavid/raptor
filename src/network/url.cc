#include "url.h"

namespace network {

static std::string get_domain(const std::string& path) 
{
  int delim_pos = path.find("/");
  if (delim_pos < 0 or delim_pos >= path.length())
    return "";

  std::string domain = path.substr(0, delim_pos);
  return domain;
}

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
      p = URLProtocol::Local;
    else if (protocol == "http")
      p = URLProtocol::Http;
    else if (protocol == "https")
      p = URLProtocol::Https;

    std::string domain = get_domain(path);
    return URL{p, path, domain};
  }

  if (url[0] == '/')
  {
    // Identical to 'file://'
    std::string domain = get_domain(url);
    return URL{URLProtocol::Local, url, domain};
  }
  else if (url[0] != '/') 
  {
    // Should default to http protocol if no '/' in the beginning 
    // e.g. localhost:8000 == http://localhost:8000
    std::string domain = get_domain(url);
    return URL{URLProtocol::Http, url, domain};
  }
}

std::string resolve_hostname(const std::string& hostname)
{
  hostent* h = gethostbyname(hostname.c_str());
  char* ip = inet_ntoa(*(struct in_addr*)h->h_addr_list[0]);
  return std::string(ip);
}

}