#include "network.h"

namespace network {

static ResolveResult resolve_local_url(const URL& url)
{
  auto p = std::filesystem::path(url.path);
  std::string extension = p.extension();

  if (extension != ".html")
  {
    std::cerr << "At the moment, Local files only support .html" << std::endl;
    exit(1);
  }

  return ResolveResult{true, p, ""};
}

static ResolveResult resolve_http_url(const URL& url)
{
  cpr::Response r = cpr::Get(cpr::Url{url.path});

  if (r.status_code != 200)
  {
    std::cerr << "Error fetching url: " << url.path << std::endl;
    exit(1);
  }

  return ResolveResult{
    .from_file=false,
    .content=r.text,
    .base_path=""
  };
}

ResolveResult resolve(const std::string& url_string)
{
  URL url = parse_url(url_string);

  switch (url.protocol)
  {
    case URLProtocol::Local:
      return resolve_local_url(url);
    case URLProtocol::Http:
      return resolve_http_url(url);
    case URLProtocol::Https:
      return resolve_http_url(url);
    default:
      std::cerr << "Trying to resolve unknown URLProtocol" << std::endl;
      exit(1);
  }
}

}