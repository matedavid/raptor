#pragma once

#include <iostream>

#include "node.h"
#include "document_type.h"
#include "text.h"
#include "comment.h"
#include "attr.h"

class HTMLCollection;

class Document : public Node 
{
private:

  std::string URL;
  std::string document_URI;
  std::string compat_mode;
  std::string character_set;
  std::string content_type;

  DocumentType doctype;
  Element* document_element;


public:
  Document();

  Element* element();
  HTMLCollection get_elements_by_tag_name(const std::string& name);
  HTMLCollection get_elements_by_class_name(const std::string& name);

  Element* create_element(const std::string& name);
  // DocumentFragment* create_document_fragment():
  Text* create_text_node(const std::string& data);
  Comment* create_comment(const std::string& data);
  // ProcessingInstruction* create_processing_instruction(const std::string& target, const std::string& data);

  Node* import_node(const std::string& data);
  Node* adopt_node(Node* node);

  Attr* create_attribute(const std::string& name);

  

};
