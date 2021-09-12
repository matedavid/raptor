#pragma once

#include <iostream>
#include <vector>

typedef std::vector<Node*> NodeList;

enum NodeType
{
  Element,
  Attribute,
  Text,
  CDataSection,
  ProcessingInstruction,
  CommentNode,
  DocumentNode,
  DocumentTypeNode,
  DocumentFragmentNode,
};

class Node
{
private:
  enum DocumentPosition
  {
    Disconnected,
    Preceding,
    Following,
    Contains,
    ContainedBy,
    ImplementationSpecific
  };

  NodeType node_type;
  std::string node_name;
  std::string node_value;
  std::string text_context;

  Document owner_document;
  bool is_connected;

public:
  Node* parent_node;
  Element* parent_element;
  NodeList child_nodes;
  Node* first_child;
  Node* last_child;
  Node* previous_sibilings;
  Node* next_sibiling;

  Node* insert_before(Node* node, Node* child);
  Node* replace_child(Node* node, Node* child);
  Node* remove_child(Node* child);
  Node* append_child(Node* node);

  Node* clone_node(bool deep=false);

  bool has_child_nodes();
  bool is_equal_node(Node* other_node);
  bool contains(Node* node);

  DocumentPosition compare_document_position(Node* node);


  void normalize();
};