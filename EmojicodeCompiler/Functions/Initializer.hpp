//
//  Initializer.hpp
//  EmojicodeCompiler
//
//  Created by Theo Weidmann on 22/08/2017.
//  Copyright © 2017 Theo Weidmann. All rights reserved.
//

#ifndef Initializer_hpp
#define Initializer_hpp

#include "Function.hpp"
#include <utility>

namespace EmojicodeCompiler {

class Initializer : public Function {
public:
    Initializer(std::u32string name, AccessLevel level, bool final, Type owningType, Package *package,
                SourcePosition p, bool overriding, std::u32string documentationToken, bool deprecated, bool r,
                std::experimental::optional<Type> errorType, FunctionType mode)
    : Function(std::move(name), level, final, std::move(owningType), package, std::move(p), overriding, std::move(documentationToken), deprecated, true, mode),
    required_(r), errorType_(std::move(errorType)) {
        returnType = Type::noReturn();
    }

    /// Whether all subclassess are required to implement this initializer as well. Never true for non-class types.
    bool required() const { return required_; }
    /// Whether this initalizer might return an error.
    bool errorProne() const { return static_cast<bool>(errorType_); }
    const Type& errorType() const { return *errorType_; }

    /// Returns the actual type constructed with this initializer for the given initialized type @c type
    Type constructedType(Type type) {
        type.unbox();
        if (errorType_) {
            Type errorType = Type::error();
            errorType.genericArguments_ = { *errorType_, type };
            return errorType;
        }
        return type;
    }
    void addArgumentToVariable(const std::u32string &string, const SourcePosition &p) {
        auto find = std::find(argumentsToVariables_.begin(), argumentsToVariables_.end(), string);
        if (find != argumentsToVariables_.end()) {
            throw CompilerError(p, "Instance variable initialized with 🍼 more than once.");
        }
        argumentsToVariables_.push_back(string);
    }
    const std::vector<std::u32string>& argumentsToVariables() const { return argumentsToVariables_; }
private:
    bool required_;
    std::experimental::optional<Type> errorType_;
    std::vector<std::u32string> argumentsToVariables_;
};

}  // namespace EmojicodeCompiler

#endif /* Initializer_hpp */
