#pragma once

#include <iostream>

#include "node.h"

class DocumentType : public Node 
{
private:
  std::string m_name;
  std::string m_public_id;
  std::string m_system_id;
public:
  DocumentType();

  std::string name();
  std::string public_id();
  std::string system_id();
};
