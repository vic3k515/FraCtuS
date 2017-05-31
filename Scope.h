//
// Created by franek on 28.05.17.
//

#ifndef FRACTUS_SCOPE_H
#define FRACTUS_SCOPE_H

#include <set>
#include <vector>
#include <unordered_map>
#include <functional>
#include "Parser.h"

class Parser;
class Scope;

enum Type {
    Int,
    Fraction,
    Bool,
    String,
    Proc
};

//enum DescType {
//    VarId,
//    ConstId,
//    TypeId,
//    ProcId,
//    //ProgId
//};

struct Descriptor {
    Descriptor(const std::string &name, /*DescType descType,*/ Type type);
    ~Descriptor() {}

    std::string name;
    //DescType descType;
    Type type;

    //bool operator==(const Descriptor &oth) const;
};

struct ProcDescriptor : public Descriptor {
    ProcDescriptor(const std::string &name, Scope *scope);
    ~ProcDescriptor() {}

    std::vector<Descriptor*> params;
    Type returnType;
    Scope *scope;
    Descriptor *left, *right;
};

struct DescriptorPtrEq {
    bool operator () ( Descriptor const * lhs, Descriptor const * rhs ) const {
        return lhs->name == rhs->name
               //&& lhs->descType == rhs->descType
               && lhs->type == rhs->type;
    }
};

class Scope {
public:
    using ExpectedTypes = std::set<Type>;

    Scope(const std::string& name, Parser& parser, Scope *extscope);
    ~Scope();
    Descriptor* insert(const std::string &name, /*DescType descType,*/ Type type);
    Descriptor* lookup(const std::string &name, ExpectedTypes expTypes);

private:
    std::string scopeName;
    Parser &parser;
    unsigned int level;
    Scope *enclosingScope;
    //bool descComp(Descriptor*, Descriptor*);
    std::unordered_map<std::string, Descriptor*> symbols;
};


#endif //FRACTUS_SCOPE_H
