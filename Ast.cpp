//
// Ast source file
// Wiktor Franus, WUT 2017
//

#include "Ast.h"
#include "Interpreter.h"

/**
 * Accept visitor methods
 */

void BinOpNode::accept(Visitor &v) const {
    v.visit(this);
}

void LogicalOp::accept(Visitor &v) const {
    v.visit(this);
}

void IntNode::accept(Visitor &v) const {
    v.visit(this);
}

void FractNode::accept(Visitor &v) const {
    v.visit(this);
}

void BoolNode::accept(Visitor &v) const {
    v.visit(this);
}

void StringNode::accept(Visitor &v) const {
    v.visit(this);
}

void UnaryOpNode::accept(Visitor &v) const {
    v.visit(this);
}

void CompoundNode::accept(Visitor &v) const {
    v.visit(this);
}

void AssignNode::accept(Visitor &v) const {
    v.visit(this);
}

void IfNode::accept(Visitor &v) const {
    v.visit(this);
}

void WhileNode::accept(Visitor &v) const {
    v.visit(this);
}

void ProcCallNode::accept(Visitor &v) const {
    v.visit(this);
}

void ReturnNode::accept(Visitor &v) const {
    v.visit(this);
}

void VarNode::accept(Visitor &v) const {
    v.visit(this);
}

//void NoOpNode::accept(Visitor &v) const {
//    v.visit(this);
//}

void ProgramNode::accept(Visitor &v) const {
    v.visit(this);
}

void BlockNode::accept(Visitor &v) const {
    v.visit(this);
}

void VarDeclNode::accept(Visitor &v) const {
    v.visit(this);
}

void TypeNode::accept(Visitor &v) const {
    v.visit(this);
}

void ProcDeclNode::accept(Visitor &v) const {
    v.visit(this);
}

void ParamNode::accept(Visitor &v) const {
    v.visit(this);
}


/**
 * Evaluation methods
 */

ValType Node::evaluate(Interpreter *interpreter) {
    return std::make_pair(Value(), Type::Void);
}

ValType IntNode::evaluate(Interpreter *interpreter) {
    Value v;
    v.intVal = value;
    return std::make_pair(v, Type::Int);
}

ValType FractNode::evaluate(Interpreter *interpreter) {
    Value v;
    v.fractVal = value;
    return std::make_pair(v, Type::Fraction);
}

ValType BoolNode::evaluate(Interpreter *interpreter) {
    Value v;
    v.boolVal = value;
    return std::make_pair(v, Type::Bool);
}

ValType StringNode::evaluate(Interpreter *interpreter) {
    Value v;
    v.stringVal = value;
    return std::make_pair(v, Type::String);
}

ValType BinOpNode::evaluate(Interpreter *interpreter) {
    ValType leftRes = left->evaluate(interpreter);
    ValType rightRes = right->evaluate(interpreter);
    Value v;

    if (leftRes.second != rightRes.second) {
        throw std::runtime_error("Incompalible types");
    }
    switch (op) {
        case PLUS:
            // int, fraction or string operands are accepted
            if ((leftRes.second == Type::Int && rightRes.second == Type::Int)
                || (leftRes.second == Type::String && rightRes.second == Type::String)
                || (leftRes.second == Type::Fraction && rightRes.second == Type::Fraction)) {
                return ValType(leftRes + rightRes);
            } else {
                throw std::runtime_error("Operands must be two numbers, fractions or strings.");
            }
        case MINUS:
            interpreter->checkNumberOperands(leftRes, rightRes);
            return ValType(leftRes - rightRes);
        case MULTSIGN:
            interpreter->checkNumberOperands(leftRes, rightRes);
            return ValType(leftRes * rightRes);
        case DIVSIGN:
            interpreter->checkNumberOperands(leftRes, rightRes);
            return ValType(leftRes / rightRes);
        case EQOP:
            v.boolVal = leftRes == rightRes;
            return std::make_pair(v, Type::Bool);
        case NEQOP:
            v.boolVal = leftRes != rightRes;
            return std::make_pair(v, Type::Bool);
        case LTOP:
            interpreter->checkNumberOperands(leftRes, rightRes);
            v.boolVal = leftRes < rightRes;
            return std::make_pair(v, Type::Bool);
        case LEOP:
            interpreter->checkNumberOperands(leftRes, rightRes);
            v.boolVal = leftRes <= rightRes;
            return std::make_pair(v, Type::Bool);
        case GTOP:
            interpreter->checkNumberOperands(leftRes, rightRes);
            v.boolVal = leftRes > rightRes;
            return std::make_pair(v, Type::Bool);
        case GEOP:
            interpreter->checkNumberOperands(leftRes, rightRes);
            v.boolVal = leftRes >= rightRes;
            return std::make_pair(v, Type::Bool);
    }
    return std::make_pair(Value(), Type::Void); //should not happen
}

ValType LogicalOp::evaluate(Interpreter *interpreter) {
    ValType leftRes = left->evaluate(interpreter);

    if (op == OROP && interpreter->isTruthy(leftRes)) {
        return leftRes;
    }
    if (op == ANDOP && !interpreter->isTruthy(leftRes)) {
        return leftRes;
    }

    return right->evaluate(interpreter);
}

ValType UnaryOpNode::evaluate(Interpreter *interpreter) {
    ValType expRes = expression->evaluate(interpreter);

    switch (op) {
        case MINUS:
            interpreter->checkNumberOperand(expRes);
            if (expRes.second == Type::Int) {
                expRes.first.intVal = -expRes.first.intVal;
            }
            if (expRes.second == Type::Fraction) {
                expRes.first.fractVal.whole = -expRes.first.fractVal.whole;
            }
            return expRes;
        case NOTSIGN:
            Value v;
            v.boolVal = !interpreter->isTruthy(expRes);
            return std::make_pair(v, Type::Bool);
    }
    return std::make_pair(Value(), Type::Void); //should not happen
}

ValType CompoundNode::evaluate(Interpreter *interpreter) {
    ValType res;
    ValType prevRetVal = interpreter->currContext().getReturnValue();
    for (auto &&node : children) {
        res = node->evaluate(interpreter);
        //std::cout <<"sep"<< std::endl;
        ValType currRetVal = interpreter->currContext().getReturnValue();
        if ((prevRetVal.second == Type::Void && currRetVal.second != Type::Void)
            || (prevRetVal != currRetVal)) {
            //std::cout << "RETURN" << std::endl;
            break;
        }
    }
    return res;
}

ValType AssignNode::evaluate(Interpreter *interpreter) {
    ValType expRes = right->evaluate(interpreter);

    if (expRes.second == Type::Void) {
        throw std::runtime_error("Cannot assign expression of type void.");
    }

    Descriptor *leftDescriptor = interpreter->currContext().getVariableDescriptor(left->name);
    if (leftDescriptor->type != DescType::Var) {
        throw std::runtime_error("Symbol before equal sign is not a valid variable.");
    }

    VarDescriptor *varDescriptor = static_cast<VarDescriptor*>(leftDescriptor);
    const std::string &varTypeName = varDescriptor->typeDesc->name;

    if (interpreter->typeNames[expRes.second] != varTypeName) {
        throw std::runtime_error("Cannot assign expression, because types do not match.");
    }

    interpreter->currContext().setVariableValue(left->name, expRes.first);
    return expRes;
}

ValType IfNode::evaluate(Interpreter *interpreter) {
    ValType cond = condition->evaluate(interpreter);
    if (interpreter->isTruthy(cond)) {
        return thenNode->evaluate(interpreter);
    } else if (elseNode){
        return elseNode->evaluate(interpreter);
    }
    return cond;
}

ValType WhileNode::evaluate(Interpreter *interpreter) {
    ValType res;
    while (interpreter->isTruthy(condition->evaluate(interpreter))) {
        res = statement->evaluate(interpreter);
    }
    return res;
}

ValType ReturnNode::evaluate(Interpreter *interpreter) {
    ValType retVal = expr->evaluate(interpreter);
    interpreter->currContext().setReturnValue(retVal);
    //std::cout << "New return value set!" << std::endl;
    return retVal;
}

ValType VarNode::evaluate(Interpreter *interpreter) {
    ValType &val = interpreter->currContext().getVariableValue(name);
//    Descriptor *desc = interpreter->currContext().getVariableDescriptor(name);
//    const std::string &typeName = static_cast<VarDescriptor*>(desc)->typeDesc->name;
    return val;
}

ValType ProgramNode::evaluate(Interpreter *interpreter) {
    return block->evaluate(interpreter);
}

ValType BlockNode::evaluate(Interpreter *interpreter) {
    return compundStatement->evaluate(interpreter);
}

ValType TypeNode::evaluate(Interpreter *interpreter) {
    Value v;
    v.stringVal = typeName;
    return std::make_pair(v, Type::String);
}

ValType ParamNode::evaluate(Interpreter *interpreter) {
    return varNode->evaluate(interpreter);
}

// TODO: move these methods somewhere else
ValType print(ProcCallNode *node, Interpreter *interpreter);
ValType read(ProcCallNode *node, Interpreter *interpreter);

ValType ProcCallNode::evaluate(Interpreter *interpreter) {
    Descriptor *desc = interpreter->currContext().getVariableDescriptor(proc->name);
    if (desc->type != DescType::Proc) {
        std::runtime_error("Callable is not a defined procedure.");
    }

    ProcDescriptor *procDesc = static_cast<ProcDescriptor*>(desc);

    // call builtin procedures
    if (procDesc->name == "print") {
        return print(this, interpreter);
    } else if (procDesc->name == "read") {
        return read(this, interpreter);
    };

    if (arguments.size() != procDesc->params.size()) {
        std::runtime_error("Incorrect number of parameters: " + proc->name);
    }

    interpreter->createNewContextFrame(procDesc->name);

    for (size_t i = 0; i<arguments.size(); ++i) {
        ValType val = arguments[i]->evaluate(interpreter);
        const std::string &parTypeName = procDesc->params[i]->typeDesc->name;
        if (interpreter->typeNames[val.second] != parTypeName) {
            std::runtime_error("Incorrect procedure argument type. Expected: " + parTypeName +
            ", got: " + interpreter->typeNames[val.second]);
        }
        interpreter->currContext().setVariableValue(procDesc->params[i]->name, val.first);
    }

    BlockNode *procBody = interpreter->getProcNodes(procDesc->name)->blockNode;
    procBody->evaluate(interpreter);

    ValType retVal = interpreter->currContext().getReturnValue();

    if (interpreter->typeNames[retVal.second] != procDesc->retType) {
        std::runtime_error("Incorrect procedure return type. Expected: " + procDesc->retType +
                           ", got: " + interpreter->typeNames[retVal.second]);
    }

    interpreter->popContextFrame();
    return retVal;
}

ValType print(ProcCallNode *node, Interpreter *interpreter) {
    std::cout << node->arguments[0]->evaluate(interpreter) << std::endl;
    return std::make_pair(Value(), Type::Void);
}

ValType read(ProcCallNode *node, Interpreter *interpreter) {
    //auto valType = node->arguments[0]->evaluate(interpreter);
    VarNode *varNode = static_cast<VarNode*>(node->arguments[0]);
    ValType &val = interpreter->currContext().getVariableValue(varNode->name);
    std::cin >> val;
    //interpreter->currContext().setVariableValue(procDesc->params[i]->name, val.first);
    //std::cout << "Wczytano : " << val << std::endl;
    return std::make_pair(Value(), Type::Void);
}


/**
 * Node d-tors
 */
BinOpNode::~BinOpNode() {
    delete left;
    delete right;
}

UnaryOpNode::~UnaryOpNode() {
    delete expression;
}

CompoundNode::~CompoundNode() {
    for (auto &&node : children) {
        delete node;
    }
}

AssignNode::~AssignNode() {
    delete left;
    delete right;
}

IfNode::~IfNode() {
    delete condition;
    delete thenNode;
    delete elseNode;
}

WhileNode::~WhileNode() {
    delete condition;
    delete statement;
}

ReturnNode::~ReturnNode() {
    delete expr;
}

ProgramNode::~ProgramNode() {
    delete block;
}

BlockNode::~BlockNode() {
    for (auto &&node : varDeclarations) {
        delete node;
    }
    for (auto &&node : procDeclarations) {
        delete node;
    }
    delete compundStatement;
}

VarDeclNode::~VarDeclNode() {
    delete varNode;
}

ParamNode::~ParamNode() {
    delete varNode;
}

ProcDeclNode::~ProcDeclNode() {
    delete returnType;
    delete blockNode;
    for (auto &&node : params) {
        delete node;
    }
}

ProcCallNode::~ProcCallNode() {
    delete proc;
    for (auto &&node : arguments) {
        delete node;
    }
}