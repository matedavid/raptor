#pragma once

#include <iostream>
#include <vector>

#include "node.h"
#include "named_node_map.h"

#include "liquid/html/html_collection.h"

class Element : public Node 
{
private:
  std::string namespace_URI;
  std::string prefix;
  std::string local_name;
  std::string tag_name;

  DOMTokenList class_list; // TODO(david): Implement DOMTokenList
  NamedNodeMap attributes;

public:
  std::string id;
  std::string class_name;
  std::string slot;

public:
  Element();

  bool has_attributes();
  std::vector<std::string> get_attribute_names();
  std::string get_attribute(std::string name);
  void set_attribute(std::string name, std::string value);
  void remove_attribute(std::string name);
  bool toggle_attribute(std::string name);
  bool toggle_attribute(std::string name, bool force);
  bool has_attribute(std::string name);

  Attr* get_attribute_node(std::string name);
  Attr* set_attribute_node(Attr* attr);
  Attr* remove_attribute_node(Attr* attr);

  // Node(david): ShadowRoot attributes not implemented

  Element* closest(std::string selectors);
  bool matches(std::string selectors);
  
  HTMLCollection get_elements_by_tag_name(std::string name);
  HTMLCollection get_elements_by_class_name(std::string name);
};