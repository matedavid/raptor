#include "network.h"

namespace network {

static ResolveResult resolve_local_url(const std::string& path)
{
  auto p = std::filesystem::path(path);
  std::string extension = p.extension();

  if (extension != ".html")
  {
    std::cout << "At the moment, Local files only support .html" << std::endl;
    exit(1);
  }

  return ResolveResult{true, p, ""};
}

static ResolveResult resolve_http_url(const std::string& path)
{
}

static ResolveResult resolve_https_url(const std::string& path)
{
}


ResolveResult resolve(const std::string& url_string)
{
  URL url = parse_url(url_string);

  switch (url.protocol)
  {
    case URLProtocol::Local:
      return resolve_local_url(url.path);
    case URLProtocol::Http:
      return resolve_http_url(url.path);
    case URLProtocol::Https:
      return resolve_https_url(url.path);
  }
}

}