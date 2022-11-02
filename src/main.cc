#include <iostream>

#include "html/tokenizer.h"

int main() {
    std::string html_content = R"(
    <!DOCTYPE HTML public "-//W3C//DTD HTML 4.0 Frameset//EN">

    <html>
        <head>
        </head>

        <body>
            <p>Hello world</p>
        </body>
    </html>)";

    // Remove newlines and \r character
    html_content.erase(std::remove_if(html_content.begin(), html_content.end(),
                                      [](auto c) { return c == '\n' || c == '\r'; }),
                       html_content.end());

    auto tokenizer = liquid::html::Tokenizer(html_content);
}
