//
// Scope source file
// Wiktor Franus, WUT 2017
//

#include "Scope.h"

Descriptor::Descriptor(const std::string &name, DescType type)
: name(name)
, type(type)
{}

BuiltInTypeDescriptor::BuiltInTypeDescriptor(const std::string &name)
: Descriptor(name, DescType::BuiltInType)
{}

void BuiltInTypeDescriptor::accept(DescVisitor &v) const {
   v.visit(this);
}


VarDescriptor::VarDescriptor(const std::string &name, BuiltInTypeDescriptor *type)
: Descriptor(name, DescType::Var)
, typeDesc(type)
{}

void VarDescriptor::accept(DescVisitor &v) const {
    v.visit(this);
}

ProcDescriptor::ProcDescriptor(const std::string &name)
: Descriptor(name, DescType::Proc)
{}

void ProcDescriptor::accept(DescVisitor &v) const {
    v.visit(this);
}

ProcDescriptor::ProcDescriptor(const std::string &name, const std::string &retType, std::vector<VarDescriptor*> &params)
: Descriptor(name, DescType::Proc)
, retType(retType)
, params(params)
{}

std::ostream& operator<<(std::ostream& stream, const BuiltInTypeDescriptor &d) {
    stream << "<BuildInTypeDescriptor(name=" << d.name << ")>";
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const VarDescriptor &d) {
    stream << "<VarDescriptor(name='" << d.name << "', type='";
    if (d.typeDesc) {
        stream << d.typeDesc->name;
    } else {
        stream << "UNKNOWN!";
    }
    stream << "')>";
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const ProcDescriptor &d) {
    stream << "<ProcDescriptor(name=" << d.name << ", parameters=[";

    for (VarDescriptor* param : d.params) {
        stream << *param << ",";
    }
    stream << "])>";
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const Descriptor* d) {
    PrintDescVisitor v(stream);
    d->accept(v);
    return stream;
}

PrintDescVisitor::PrintDescVisitor(std::ostream &stream)
: stream(stream)
{}

void PrintDescVisitor::visit(const BuiltInTypeDescriptor *n) {
    stream << *n;
}

void PrintDescVisitor::visit(const VarDescriptor *n) {
    stream << *n;
}
void PrintDescVisitor::visit(const ProcDescriptor *n) {
    stream << *n;
}

Scope::Scope(const std::string &name, unsigned int level, Scope *extscope)
: scopeName(name)
, level(level)
, enclosingScope(extscope)
{}

Descriptor* Scope::insert(Descriptor *symbol) {
    //std::cout << "Insert: " << symbol->name << std::endl;
    return (*(symbols.insert({symbol->name, symbol}).first)).second;
}

Descriptor* Scope::lookup(const std::string &name, bool currentScopeOnly) {
    //std::cout << "Lookup: " << name << ". (Scope name: " << scopeName << ")" << std::endl;
    auto it = symbols.find(name);
    if (it != symbols.end()) {
        //std::cout << "FOUND";
        return it->second;
    }

    if (currentScopeOnly) {
        return nullptr;
    }

    // if not found, search in enclosing scope
    if (enclosingScope != nullptr) {
        //std::cout << "hop";
        return enclosingScope->lookup(name);
    }
    //std::cout << "OUT";
    return nullptr;
}

Scope *Scope::getEnclosingScope() const {
    return enclosingScope;
}

unsigned int Scope::getLevel() const {
    return level;
}

void Scope::initializeBuiltInTypes() {
    insert(new BuiltInTypeDescriptor("integer"));
    //insert(new BuiltInTypeDescriptor("false"));
    //insert(new BuiltInTypeDescriptor("true"));
    BuiltInTypeDescriptor *boolDesc = new BuiltInTypeDescriptor("boolean");
    BuiltInTypeDescriptor *fractionDesc = new BuiltInTypeDescriptor("fraction");
    BuiltInTypeDescriptor *stringDesc = new BuiltInTypeDescriptor("string");
    insert(boolDesc);
    insert(stringDesc);
    insert(fractionDesc);

    std::vector<VarDescriptor*> printParams;
    printParams.push_back(new VarDescriptor("s", stringDesc));
    insert(new ProcDescriptor("print", "void", printParams));

    std::vector<VarDescriptor*> readParams;
    readParams.push_back(new VarDescriptor("f", fractionDesc));
    insert(new ProcDescriptor("read", "void", readParams));

    VarDescriptor *falseDesc = new VarDescriptor("false", boolDesc);
    VarDescriptor *trueDesc = new VarDescriptor("true", boolDesc);
    insert(falseDesc);
    insert(trueDesc);
}

std::ostream& operator<<(std::ostream& os, const Scope& obj) {
    os << std::endl << "SCOPE SYMBOL TABLE" << std::endl;
    os << "==================" << std::endl;
    os << "Scope name : " << obj.scopeName << std::endl;
    os << "Scope level : " << obj.level << std::endl;
    if (obj.enclosingScope) {
        os << "Enclosing scope : " << obj.enclosingScope->scopeName << std::endl;
    } else {
        os << "Enclosing scope : " << "None" << std::endl;
    }
    os << "Scope symbol table contents" << std::endl;
    os << "---------------------------" << std::endl;

    std::cout << "number of symbols: " << obj.symbols.size() << std::endl;
    for (auto symbPair : obj.symbols) {
        os << (Descriptor*)symbPair.second << "\n";
    }
    os << "---------------------------";

    return os;
}

const std::string &Scope::getScopeName() const {
    return scopeName;
}

Scope::Symbols const &Scope::getSymbolTable() const {
    return symbols;
}

ValType operator+(const ValType &left, const ValType &right) {
    Value ret;
    Type opType = left.second;
    switch (opType) {
        case Type::Int:
            ret.intVal = left.first.intVal + right.first.intVal;
            break;
        case Type::String:
            ret.stringVal = left.first.stringVal + right.first.stringVal;
            break;
        case Type::Fraction:
            ret.fractVal = left.first.fractVal + right.first.fractVal;
            break;
        default:
            break;
    }
    return std::make_pair(ret, opType);
}

ValType operator-(const ValType &left, const ValType &right) {
    Value ret;
    Type opType = left.second;
    switch (opType) {
        case Type::Int:
            ret.intVal = left.first.intVal - right.first.intVal;
            break;
        case Type::Fraction:
            ret.fractVal = left.first.fractVal - right.first.fractVal;
            break;
        default:
            break;
    }
    return std::make_pair(ret, opType);
}

ValType operator*(const ValType &left, const ValType &right) {
    Value ret;
    Type opType = left.second;
    switch (opType) {
        case Type::Int:
            ret.intVal = left.first.intVal * right.first.intVal;
            break;
        case Type::Fraction:
            ret.fractVal = left.first.fractVal * right.first.fractVal;
            break;
        default:
            break;
    }
    return std::make_pair(ret, opType);
}

ValType operator/(const ValType &left, const ValType &right) {
    Value ret;
    Type opType = left.second;
    switch (opType) {
        case Type::Int:
            ret.intVal = left.first.intVal / right.first.intVal;
            break;
        case Type::Fraction:
            ret.fractVal = left.first.fractVal / right.first.fractVal;
            break;
        default:
            break;
    }
    return std::make_pair(ret, opType);
}

bool operator==(const ValType &left, const ValType &right) {
    if (left.second == Type::Void || right.second == Type::Void) {
        return false;
    }
    switch (left.second) {
        case Type::Bool:
            return left.first.boolVal == right.first.boolVal;
        case Type::Int:
            return left.first.intVal == right.first.intVal;
        case Type ::String:
            return left.first.stringVal == right.first.stringVal;
        case Type::Fraction:
            return left.first.fractVal == right.first.fractVal;
        default:
            return false;
    }
}

bool operator!=(const ValType &left, const ValType &right) {
    if (left.second == Type::Void || right.second == Type::Void) {
        return false;
    }
    return !(left == right);
}

bool operator<(const ValType &left, const ValType &right) {
    switch (left.second) {
        case Type::Int:
            return left.first.intVal < right.first.intVal;
        case Type::Fraction:
            return left.first.fractVal < right.first.fractVal;
        default:
            false;
    }
}

bool operator>(const ValType &left, const ValType &right) {
    switch (left.second) {
        case Type::Int:
            return left.first.intVal > right.first.intVal;
        case Type::Fraction:
            return left.first.fractVal > right.first.fractVal;
        default:
            false;
    }
}

bool operator<=(const ValType &left, const ValType &right) {
    return !(left > right);
}

bool operator>=(const ValType &left, const ValType &right) {
    return !(left < right);
}

std::ostream& operator<<(std::ostream &os, const ValType &obj) {
    switch (obj.second) {
        case Type::Bool:
            os << obj.first.boolVal ? "true" : "false";
            break;
        case Type::Int:
            os << obj.first.intVal;
            break;
        case Type::String:
            os << obj.first.stringVal;
            break;
        case Type::Fraction:
            os << obj.first.fractVal;
            break;
        default:
            break;
    }
    return os;
}

std::istream& operator>>(std::istream &is, ValType &obj) {
    switch (obj.second) {
        case Type::Bool:
            is >> obj.first.boolVal;
            break;
        case Type::Int:
            is >> obj.first.intVal;
            break;
        case Type::String:
            is >> obj.first.stringVal;
            break;
        case Type::Fraction:
            is >> obj.first.fractVal;
            break;
        default:
            break;
    }
    return is;
}

Context::Context(Scope *ctxScope, Context *globalCtx)
: contextScope(ctxScope)
, globalContext(globalCtx)
, returnValue({Value(), Type::Void}) {
    initializeVariables();
}

Context::Context(Context &&oth) {
    this->contextScope = oth.contextScope;
    this->globalContext = oth.globalContext;
    this->environment = oth.environment;
    this->returnValue = oth.returnValue;

    oth.contextScope = nullptr;
    oth.globalContext = nullptr;
}

Descriptor *Context::getVariableDescriptor(const std::string &name) {
    return contextScope->lookup(name);
}

ValType &Context::getVariableValue(const std::string &varName){
    auto it = environment.find(varName);
    if (it != environment.end()) {
        return (*it).second;
    }

    if (globalContext) {
        return globalContext->getVariableValue(varName);
    }

    throw std::runtime_error("Variable not found in current context: " + varName);
//    try {
//        ValType &val = environment.at(varName);
//        return val;
//    } catch (std::out_of_range e) {
//        throw std::runtime_error("Variable not found in current context: " + varName);
//    }
}

void Context::setVariableValue(const std::string &varName, const Value &value){
    auto it = environment.find(varName);
    if (it != environment.end()) {
        environment[varName].first = value;
        return;
    }

    if (globalContext) {
        return globalContext->setVariableValue(varName, value);
    }

    throw std::runtime_error("Variable not found in current context: " + varName);
//    try {
//        ValType &val = environment.at(varName);
//        val.first = value;
//    } catch (std::out_of_range e) {
//        throw std::runtime_error("Variable not found in current context: " + varName);
//    }
}

ValType &Context::getReturnValue() {
    return returnValue;
}

void Context::setReturnValue(const ValType &val) {
    returnValue = val;
}

void Context::initializeVariables() {
    if (contextScope != nullptr) {
        for (auto nameDescPair : contextScope->getSymbolTable()) {
            DescType descType = nameDescPair.second->type;
            //if (descType == DescType::BuiltInType) {}
            if ( descType == DescType::Var) {
                VarDescriptor *varDesc = static_cast<VarDescriptor*>(nameDescPair.second);
                ValType val;
                if(varDesc->typeDesc->name == "boolean") {
                    std::cout << "bool" << std::endl;
                    if (varDesc->name == "true") { // builtin variable called "true"
                        val.first.boolVal = true;
                    } else {
                        val.first.boolVal = false;
                    }
                    val.second = Type::Bool;
                }
                if(varDesc->typeDesc->name == "integer") {
                    std::cout << "integer" << std::endl;

                    val.first.intVal = 0;
                    val.second = Type::Int;
                }
                if(varDesc->typeDesc->name == "string") {
                    std::cout << "string" << std::endl;

                    val.first.stringVal = std::string();
                    val.second = Type::String;
                }
                if(varDesc->typeDesc->name == "fraction") {
                    std::cout << "fraction" << std::endl;

                    val.first.fractVal = Fraction();
                    val.second = Type::Fraction;
                }
                environment.insert(std::make_pair(varDesc->name, val));
            }
        }
    }
}
