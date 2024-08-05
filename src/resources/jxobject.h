#ifndef JXOBJECT_H
#define JXOBJECT_H
#include <string>

class JXObject {
public:
  JXObject(JXObject *parent=nullptr, std::string name="noname"){

    this->className = name;
    this->parent = parent;

 }
  std::string className;
  JXObject *parent;

};
#endif // JXOBJECT_H

