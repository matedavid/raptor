#include <catch2/catch_test_macros.hpp>
#include <string>

#include "liquid/html/html_document.h"
#include "liquid/renderer/viewport.h"
#include "liquid/renderer/render_box.h"

liquid::RenderBox* get_render_box(const std::string& content)
{
  std::string html_content = "<html><head></head>"+content+"</html>";

  liquid::HTMLDocument document;
  document.from_string(html_content);

  liquid::Viewport viewport(document.body);
  return viewport.render_tree;
}

TEST_CASE("Correct layout single body")
{
  std::string single_body = R"(<body></body>)";
  auto render_tree = get_render_box(single_body);
}
