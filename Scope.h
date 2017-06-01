//
// Created by franek on 28.05.17.
//

#ifndef FRACTUS_SCOPE_H
#define FRACTUS_SCOPE_H

#include <iostream>

#include <set>
#include <vector>
#include <unordered_map>
#include <functional>
//#include "Parser.h"

//class Parser;
//class Scope;

//enum class Type {
//    Int,
//    Fraction,
//    Bool,
//    String,
//    //Proc
//    None
//};

//enum DescType {
//    VarId,
//    ConstId,
//    TypeId,
//    ProcId,
//    //ProgId
//};

class DescVisitor;

struct Descriptor {
    Descriptor(const std::string &name, /*DescType descType,*/ const std::string &type = std::string(""));
    virtual ~Descriptor() {}
    virtual void accept(DescVisitor &v) const = 0;

    std::string name;
    //DescType descType;
    //Type type;
    std::string type;

    //bool operator==(const Descriptor &oth) const;
};

struct BuiltInTypeDescriptor : public Descriptor {
    BuiltInTypeDescriptor(const std::string &name);
    ~BuiltInTypeDescriptor() {}
    void accept(DescVisitor &v) const;
};

struct VarDescriptor : public Descriptor {
    VarDescriptor(const std::string &name, BuiltInTypeDescriptor* typeDesc);
    ~VarDescriptor() {}
    void accept(DescVisitor &v) const;

    BuiltInTypeDescriptor *typeDesc;
};

struct ProcDescriptor : public Descriptor {
    ProcDescriptor(const std::string &name);
    ProcDescriptor(const std::string &name, const std::string &retType, std::vector<VarDescriptor*> &params);
    ~ProcDescriptor() {}
    void accept(DescVisitor &v) const;

    std::vector<VarDescriptor*> params;
};

std::ostream& operator<<(std::ostream &os, const BuiltInTypeDescriptor &obj);
std::ostream& operator<<(std::ostream &os, const VarDescriptor &obj);
std::ostream& operator<<(std::ostream &os, const ProcDescriptor &obj);
std::ostream& operator<<(std::ostream &os, const Descriptor &obj);


class DescVisitor {
public:
    virtual void visit(const BuiltInTypeDescriptor *n) = 0;
    virtual void visit(const VarDescriptor *n) = 0;
    virtual void visit(const ProcDescriptor *n) = 0;
};

class PrintDescVisitor : public DescVisitor {
public:
    PrintDescVisitor(std::ostream &stream);
    void visit(const BuiltInTypeDescriptor *n);
    void visit(const VarDescriptor *n);
    void visit(const ProcDescriptor *n);
private:
    std::ostream& stream;
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
    using ExpectedTypes = std::set<std::string>;

    Scope(const std::string& name, /*Parser& parser,*/ unsigned int level, Scope *extscope);
    ~Scope();
    Descriptor *insert(Descriptor *symbol);
    Descriptor *lookup(const std::string &name, /*ExpectedTypes expTypes*/ bool currentScopeOnly = false);
    Scope *getEnclosingScope() const;
    unsigned int getLevel() const;

    void initializeBuiltInTypes();
    friend std::ostream& operator<<(std::ostream& os, const Scope& obj);

private:
    std::string scopeName;
    //Parser &parser;
    unsigned int level;
    Scope *enclosingScope;
    std::unordered_map<std::string, Descriptor*> symbols;
};


#endif //FRACTUS_SCOPE_H
