//
// Created by franek on 28.05.17.
//

#include "Scope.h"

Descriptor::Descriptor(const std::string &name, /*DescType descType,*/ Type type)
: name(name)
//, descType(descType)
, type(type)
{}

ProcDescriptor::ProcDescriptor(const std::string &name, Scope *scope)
: Descriptor(name, /*ProcId,*/ Proc)
{}
//bool Descriptor::operator==(const Descriptor &oth) const {
//    return this->name == oth.name
//           && this->descType == oth.descType
//           && this->type == oth.type;
//}

//bool Scope::descComp(Descriptor * left, Descriptor * right) {
//    return left->name.compare(right);
//}

Scope::Scope(const std::string &name, Parser &parser, Scope *extscope)
: scopeName(name)
, parser(parser)
, enclosingScope(extscope)
{}

Descriptor* Scope::insert(const std::string &name, /*DescType descType,*/ Type type) {
    Descriptor* symbol = new Descriptor(name, /*descType,*/ type);
//    if (symbols.insert({name, symbol}).second == false) {
//        throw ParseException("Symbol is already declared!");
//    }

    // insert return pair <iterator tp inserted, bool>
    return (*(symbols.insert({name, symbol}).first)).second;
}

Descriptor* Scope::lookup(const std::string &name, ExpectedTypes expTypes) {
    std::cout << "Lookup: " << name << ". (Scope name: " << scopeName << ")" << std::endl;
    auto it = symbols.find(name);
    if (it != symbols.end()) {
        if (expTypes.find(it->second->type) != expTypes.end()) {
            return it->second;
        }
    }

    // if not found, search in enclosing scope
    if (enclosingScope != nullptr) {
        return enclosingScope->lookup(name, expTypes);
    }
    return nullptr;
}
