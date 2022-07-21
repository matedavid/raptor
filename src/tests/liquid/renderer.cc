#include <catch2/catch_test_macros.hpp>
#include <string>

#include "liquid/html/html_document.h"
#include "liquid/renderer/viewport.h"
#include "liquid/renderer/render_box.h"

#define WIDTH 960.f
#define HEIGHT 540.f

// ========== UTILS ==========
liquid::RenderBox* get_render_box(const std::string& content, const std::string& style="")
{
  std::string html_content = "<html><head><style rel='stylesheet' type='text/css'>"+style+"</style></head>"+content+"</html>";

  liquid::HTMLDocument document;
  document.from_string(html_content);

  liquid::Viewport viewport(document.body, WIDTH, HEIGHT);
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

TEST_CASE("First child margin collapsing")
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

TEST_CASE("Adjacent siblings margin collapsing")
{
  {
    std::string content = R"(
      <body>
        <p id='p1'>Hello world 1</p>
        <div>
          <p id='p2'>Hello world 2</p>
        </div>
        <p id='p3'>Hello world 3</p>
      </body>
    )";

    auto body = get_render_box(content);
    auto p1 = get_render_box_by_id(body, "p1");
    auto p2 = get_render_box_by_id(body, "p2");
    auto p3 = get_render_box_by_id(body, "p3");

    REQUIRE(p1->get_y() == 16.f);
    REQUIRE(p2->get_y() == p1->get_y() + p1->get_height() + 16.f);
    REQUIRE(p3->get_y() == p2->get_y() + p2->get_height() + 16.f);
  }

  {
    std::string content = R"(
      <body>
        <p id='p1'>Hello world 1</p>
        <div id='div1'>
          <p id='p2'>Hello world 2</p>
        </div>
        <p id='p3'>Hello world 3</p>
      </body>
    )";

    std::string style = R"(
      #div1 {
        margin-top: 20px;
      }

      #p2 {
        margin-bottom: 22px;
      }
    )";

    auto body = get_render_box(content, style);
    auto p1 = get_render_box_by_id(body, "p1");
    auto p2 = get_render_box_by_id(body, "p2");
    auto p3 = get_render_box_by_id(body, "p3");

    REQUIRE(p1->get_y() == 16.f);
    REQUIRE(p2->get_y() == p1->get_y() + p1->get_height() + 20.f);
    REQUIRE(p3->get_y() == p2->get_y() + p2->get_height() + 22.f);
  }
}

TEST_CASE("Inline elements")
{
	std::string content = R"(
		<body>
			<p id='p1'>Hello world</p>
			<p id='p2'>Hello world</p>
			<p id='p3'>Hello world</p>
		</body>
	)";

	std::string style = R"(
		#p1 {
			display: inline;
			margin-right: 10px;
		}

		#p2 {
			display: inline;
			margin-top: 200px;
			margin-left: 11px;
		}
	)";

	auto body = get_render_box(content, style);
	auto p1 = get_render_box_by_id(body, "p1");
	auto p2 = get_render_box_by_id(body, "p2");
	auto p3 = get_render_box_by_id(body, "p3");

	REQUIRE(p1->get_x() == 8.f);
	REQUIRE(p1->get_y() == 8.f);

	REQUIRE(p2->get_x() == 8.f + p1->get_width() + 4.f + 10.f + 11.f);
	REQUIRE(p2->get_y() == p1->get_y());

	REQUIRE(p3->get_y() == 8.f + p1->get_height() + 16.f);
}

TEST_CASE("InlineBlock elements")
{

}

TEST_CASE("Relative position")
{
	{
		std::string content = R"(
			<body>
				<p id='p1'>Hello world</p>
				<p id='p2'>Hello world</p>
				<p id='p3'>Hello world</p>
			</body>
		)";

		std::string style = R"(
			#p2 {
				position: relative;
				left: 15px;
				right: 7px;
				top: 20px;
				bottom: 10px;
			}
		)";

		auto body = get_render_box(content, style);
		auto p1 = get_render_box_by_id(body, "p1");
		auto p2 = get_render_box_by_id(body, "p2");
		auto p3 = get_render_box_by_id(body, "p3");

		REQUIRE(p2->get_x() == 8.f + 15.f);
		REQUIRE(p2->get_y() == p1->get_y() + p1->get_height() + 16.f + 20.f);

		REQUIRE(p3->get_y() == p1->get_y() + p1->get_height() + 16.f + p2->get_height() + 16.f);
	}

	{
		std::string content = R"(
			<body>
				<p id='p1'>Hello world</p>
				<p id='p2'>Hello world</p>
				<p id='p3'>Hello world</p>
			</body>
		)";

		std::string style = R"(
			#p2 {
				position: relative;
				right: 7px;
				bottom: 10px;
			}
		)";

		auto body = get_render_box(content, style);
		auto p1 = get_render_box_by_id(body, "p1");
		auto p2 = get_render_box_by_id(body, "p2");
		auto p3 = get_render_box_by_id(body, "p3");

		REQUIRE(p2->get_x() == 8.f - 7.f);
		REQUIRE(p2->get_y() == p1->get_y() + p1->get_height() + 16.f - 10.f);

		REQUIRE(p3->get_y() == p1->get_y() + p1->get_height() + 16.f + p2->get_height() + 16.f);
	}
}


TEST_CASE("Absolute position")
{
	{
		std::string content = R"(
			<body>
				<p id='p1'>Hello world</p>
				<p id='p2'>Hello world</p>
				<p id='p3'>Hello world</p>
			</body>
		)";

		std::string style = R"(
			#p2 {
				position: absolute;
			}
		)";

		auto body = get_render_box(content, style);
		auto p1 = get_render_box_by_id(body, "p1");
		auto p2 = get_render_box_by_id(body, "p2");
		auto p3 = get_render_box_by_id(body, "p3");

		REQUIRE(p2->get_x() == 8.f);
		REQUIRE(p2->get_y() == p1->get_y() + p1->get_height() + 16.f + 16.f);

		REQUIRE(p3->get_y() == p1->get_y() + p1->get_height() + 16.f);
	}

	{
		std::string content = R"(
			<body>
				<p id='p1'>Hello world</p>
				<p id='p2'>Hello world</p>
				<p id='p3'>Hello world</p>
			</body>
		)";

		std::string style = R"(
			#p1 {
				position: absolute;
			}
		)";

		auto body = get_render_box(content, style);
		auto p1 = get_render_box_by_id(body, "p1");
		auto p2 = get_render_box_by_id(body, "p2");
		auto p3 = get_render_box_by_id(body, "p3");

		REQUIRE(p1->get_x() == 8.f);
		REQUIRE(p1->get_y() == body->get_y() + 16.f);

		REQUIRE(p2->get_y() == 16.f);

		REQUIRE(p3->get_y() == 16.f + p2->get_height() + 16.f);
	}

	{
		std::string content = R"(
			<body>
				<p id='p1'>top</p>
				<p id='p2'>bottom</p>
				<p id='p3'>left</p>
				<p id='p4'>right</p>
			</body>
		)";

		std::string style = R"(
			#p1 {
				position: absolute;
				top: 10px;
				bottom: 100px;
			}

			#p2 {
				position: absolute;
				bottom: 13px;
			}

			#p3 {
				position: absolute;
				left: 9px;
				right: 30px;
			}

			#p4 {
				position: absolute;
				right: 30px;
			}
		)";

		auto body = get_render_box(content, style);
		auto p1 = get_render_box_by_id(body, "p1");
		auto p2 = get_render_box_by_id(body, "p2");
		auto p3 = get_render_box_by_id(body, "p3");
		auto p4 = get_render_box_by_id(body, "p4");

		REQUIRE(p1->get_y() == 10.f + 16.f);
		REQUIRE(p2->get_y() == HEIGHT - 13.f - 16.f - p2->get_height());
		REQUIRE(p3->get_x() == 9.f);
		REQUIRE(p4->get_x() == WIDTH - 30.f - p4->get_width());
	}
}
