#pragma once

#include <SFML/Graphics.hpp>

#include "liquid/renderer/render_box.h"
#include "liquid/renderer/render_box_text.h"
#include "liquid/renderer/render_box_image.h"

#include "liquid/html/html_document.h"
#include "liquid/html/text.h"


void paint(sf::RenderWindow& window, liquid::RenderBox* render_tree);