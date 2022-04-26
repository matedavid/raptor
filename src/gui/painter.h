#pragma once

#include <SFML/Graphics.hpp>

#include "liquid/html/html_document.h"
#include "liquid/html/text.h"
#include "liquid/renderer/render_tree.h"
#include "liquid/renderer/viewport.h"

void paint(sf::RenderWindow& window, liquid::RenderBox* render_tree, liquid::Viewport& viewport);
