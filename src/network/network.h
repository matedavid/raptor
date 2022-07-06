#include <filesystem>
#include <cpr/cpr.h>

#include "url.h"

namespace network {

struct ResolveResult 
{
  // Indicates if the content argument is a file path
  bool from_file;

  // Could be either the content of the file as a string,
  // or the path to a local file
  std::string content;

  // Path to append to the start of img, video... (src/href tags)
  // which have been downloaded from the server
  std::string base_path;
};

ResolveResult resolve(const std::string& url_string);

}