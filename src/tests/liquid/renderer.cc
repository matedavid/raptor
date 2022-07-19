#include <catch2/catch_test_macros.hpp>
#include <string>

#include "liquid/html/html_document.h"
#include "liquid/renderer/viewport.h"
#include "liquid/renderer/render_box.h"

// ========== UTILS ==========
liquid::RenderBox* get_render_box(const std::string& content, const std::string& style="")
{
  std::string html_content = "<html><head><style rel='stylesheet' type='text/css'>"+style+"</style></head>"+content+"</html>";

  liquid::HTMLDocument document;
  document.from_string(html_content);

  liquid::Viewport viewport(document.body);
  return viewport.render_tree;
}

liquid::RenderBox* get_render_box_by_id(liquid::RenderBox* render_tree, const std::string& id)
{
  if (render_tree->get_node_id() == id)
    return render_tree;

  for (auto child : render_tree->get_children())
  {
    liquid::RenderBox* child_render_box = get_render_box_by_id(child, id);
    if (child_render_box != nullptr)
      return child_render_box;
  }
  
  return nullptr;
}
// ========== ===== ==========

TEST_CASE("Correct layout single body")
{
  std::string single_body = R"(<body></body>)";
  auto render_tree = get_render_box(single_body);

  REQUIRE(render_tree->get_x() == 8.f);
  REQUIRE(render_tree->get_y() == 8.f);
}

TEST_CASE("First-child margin collapsing")
{
  {
    std::string single_p_content = R"(
      <body>
        <p id='p1'>Hello world</p>
      </body>
    )";

    auto body = get_render_box(single_p_content);
    auto p = get_render_box_by_id(body, "p1");

    REQUIRE(body->get_x() == 8.f);
    REQUIRE(body->get_y() == 16.f);

    REQUIRE(p->get_x() == 8.f);
    REQUIRE(p->get_y() == 16.f);
  }

  {
    std::string middle_div_content = R"(
      <body>
        <div id='div1'>
          <p id='p1'>Hello world</p>
        </div>
      </body>
    )";

    std::string style = R"(
      #div1 {
        margin-top: 20px;
      }
    )";

    auto body = get_render_box(middle_div_content, style);
    auto div = get_render_box_by_id(body, "div1");
    auto p = get_render_box_by_id(body, "p1");

    REQUIRE(body->get_x() == 8.f);
    REQUIRE(body->get_y() == 20.f);

    REQUIRE(div->get_x() == 8.f);
    REQUIRE(div->get_y() == 20.f);

    REQUIRE(p->get_x() == 8.f);
    REQUIRE(p->get_y() == 20.f);
  }
}