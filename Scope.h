//
// Representation of scope and symbols (descriptors)
// Wiktor Franus, WUT 2017
//

#ifndef FRACTUS_SCOPE_H
#define FRACTUS_SCOPE_H

#include <iostream>

#include <set>
#include <vector>
#include <unordered_map>
#include <map>
#include <functional>

#include "Fraction.h"

class DescVisitor;

enum class DescType {
    BuiltInType,
    Var,
    Proc
};

struct Descriptor {
    Descriptor(const std::string &name, DescType type = DescType::Var);
    virtual ~Descriptor() {}
    virtual void accept(DescVisitor &v) const = 0;

    std::string name;
    DescType type;
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

    std::string retType;
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


class Scope {
    using Symbols = std::unordered_map<std::string, Descriptor*>;
public:
    Scope(const std::string& name, unsigned int level, Scope *extscope);
    ~Scope() {}
    Descriptor *insert(Descriptor *symbol);
    Descriptor *lookup(const std::string &name, bool currentScopeOnly = false);
    void initializeBuiltInTypes();
    const std::string &getScopeName() const;
    unsigned int getLevel() const;
    Scope *getEnclosingScope() const;
    Symbols const &getSymbolTable() const;

    friend std::ostream& operator<<(std::ostream& os, const Scope& obj);

private:
    std::string scopeName;
    unsigned int level;
    Scope *enclosingScope;
    Symbols symbols;
};

struct Value {
    bool boolVal;
    int intVal;
    std::string stringVal;
    Fraction fractVal;
};

enum class Type {
    Bool,
    Int,
    String,
    Fraction,
    Void
};

using ValType = std::pair<Value, Type>;

ValType operator+(const ValType &left, const ValType &right);
ValType operator-(const ValType &left, const ValType &right);
ValType operator*(const ValType &left, const ValType &right);
ValType operator/(const ValType &left, const ValType &right);
bool operator==(const ValType &left, const ValType &right);
bool operator!=(const ValType &left, const ValType &right);
bool operator<(const ValType &left, const ValType &right);
bool operator>(const ValType &left, const ValType &right);
bool operator<=(const ValType &left, const ValType &right);
bool operator>=(const ValType &left, const ValType &right);
std::ostream& operator<<(std::ostream &os, const ValType &obj);
std::istream& operator>>(std::istream &is, ValType &obj);


class Context {
    using Environment = std::map<std::string, ValType>;

public:
    Context(Scope *contextScope, Context *globalContext);
    Context(Context &&oth);
    Descriptor *getVariableDescriptor(const std::string &name);
    ValType &getVariableValue(const std::string &varName);
    void setVariableValue(const std::string &varName, const Value &value);
    ValType &getReturnValue();
    void setReturnValue(const ValType &val);

private:
    void initializeVariables();
    Scope *contextScope;
    Context *globalContext;
    Environment environment;
    ValType returnValue;
};

#endif //FRACTUS_SCOPE_H
