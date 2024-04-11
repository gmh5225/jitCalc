#pragma once

#include "ast.h"
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/LLVMContext.h>

using namespace llvm;


class ModuleBuilder {
public:
    ModuleBuilder(LLVMContext &context, const std::string &name);

    void createExtern(const std::string &name, Type *returnType);
    void createFunction(const std::string &name, Type *returnType);
    void setCurrentFunction(const std::string &name);

    void   emitPrint(Value *value);
    void   emitReturn(Value *value);
    Value* emitExpression(const ast::Expr &);
    Value* emitInfix(const ast::Infix &);
    Value* emitPrefix(const ast::Prefix &);
    Value* emitConvertToFloat(Value *value);

    void printModule();

    Type *getInt32Ty() { return irBuilder.getInt32Ty(); }
    Type *getFloatTy() { return irBuilder.getFloatTy(); }
    Value *getInt32(int n)  { return irBuilder.getInt32(n); }
    Function *getFunction(const std::string &name) { return irModule->getFunction(name); }
    Module *getModule() { return irModule.get(); }
    std::unique_ptr<Module> moveModule() { return std::move(irModule); }

private:
    IRBuilder<>             irBuilder;
    std::unique_ptr<Module> irModule;
    Function                *curFn;
};