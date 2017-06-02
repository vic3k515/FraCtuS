//
// Scope source file
// Wiktor Franus, WUT 2017
//

#include "Scope.h"

Descriptor::Descriptor(const std::string &name, const std::string &type)
: name(name)
, type(type)
{}

BuiltInTypeDescriptor::BuiltInTypeDescriptor(const std::string &name)
: Descriptor(name)
{}

void BuiltInTypeDescriptor::accept(DescVisitor &v) const {
   v.visit(this);
}


VarDescriptor::VarDescriptor(const std::string &name, BuiltInTypeDescriptor *type)
: Descriptor(name)
, typeDesc(type)
{}

void VarDescriptor::accept(DescVisitor &v) const {
    v.visit(this);
}

ProcDescriptor::ProcDescriptor(const std::string &name)
: Descriptor(name)
{}

void ProcDescriptor::accept(DescVisitor &v) const {
    v.visit(this);
}

ProcDescriptor::ProcDescriptor(const std::string &name, const std::string &retType, std::vector<VarDescriptor*> &params)
: Descriptor(name, retType)
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
    std::cout << "Insert: " << symbol->name << std::endl;
    return (*(symbols.insert({symbol->name, symbol}).first)).second;
}

Descriptor* Scope::lookup(const std::string &name, bool currentScopeOnly) {
    std::cout << "Lookup: " << name << ". (Scope name: " << scopeName << ")" << std::endl;
    auto it = symbols.find(name);
    if (it != symbols.end()) {
        return it->second;
    }

    if (currentScopeOnly) {
        return nullptr;
    }

    // if not found, search in enclosing scope
    if (enclosingScope != nullptr) {
        return enclosingScope->lookup(name);
    }
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
    insert(new BuiltInTypeDescriptor("boolean"));
    insert(new BuiltInTypeDescriptor("false"));
    insert(new BuiltInTypeDescriptor("true"));
    BuiltInTypeDescriptor *fractionDesc = new BuiltInTypeDescriptor("fraction");
    BuiltInTypeDescriptor *stringDesc = new BuiltInTypeDescriptor("string");
    insert(stringDesc);
    insert(fractionDesc);

    std::vector<VarDescriptor*> printParams;
    printParams.push_back(new VarDescriptor("s", stringDesc));
    insert(new ProcDescriptor("print", "void", printParams));

    std::vector<VarDescriptor*> readParams;
    readParams.push_back(new VarDescriptor("f", fractionDesc));
    insert(new ProcDescriptor("read", "void", readParams));
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