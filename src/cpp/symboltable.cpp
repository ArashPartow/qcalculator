#include "symboltable.hpp"

SymbolTable::SymbolTable() = default;

const std::map<UUID, Variable> &SymbolTable::getVariables() const {
    return variables;
}

const std::map<UUID, Constant> &SymbolTable::getConstants() const {
    return constants;
}

const std::map<UUID, Function> &SymbolTable::getFunctions() const {
    return functions;
}

const std::map<UUID, Script> &SymbolTable::getScripts() const {
    return scripts;
}

UUID SymbolTable::addVariable(const Variable &variable) {
    std::string prefix = "Cannot add variable, ";
    if (hasVariable(variable.name))
        throw std::runtime_error(prefix + "Variable with name " + variable.name + " already exists.");
    if (hasConstant(variable.name))
        throw std::runtime_error(prefix + "Constant with name " + variable.name + " already exists");
    if (hasFunction(variable.name))
        throw std::runtime_error(prefix + "Function with name " + variable.name + " already exists");
    if (hasScript(variable.name))
        throw std::runtime_error(prefix + "Script with name " + variable.name + " already exists");

    UUID id = generator.get();
    variables[id] = variable;
    return id;
}

const Variable &SymbolTable::getVariable(UUID id) const {
    return variables.at(id);
}

void SymbolTable::setVariable(UUID id, const Variable &variable) {
    std::string prefix = "Cannot set variable, ";
    if (hasVariable(variable.name) && variables.at(id).name != variable.name)
        throw std::runtime_error(prefix + "Variable with name " + variable.name + " already exists.");
    if (hasConstant(variable.name))
        throw std::runtime_error(prefix + "Constant with name " + variable.name + " already exists");
    if (hasFunction(variable.name))
        throw std::runtime_error(prefix + "Function with name " + variable.name + " already exists");
    if (hasScript(variable.name))
        throw std::runtime_error(prefix + "Script with name " + variable.name + " already exists");

    variables.at(id) = variable;
}

void SymbolTable::removeVariable(UUID id) {
    variables.erase(id);
    generator.put(id);
}

bool SymbolTable::hasVariable(const std::string &name) const {
    for (auto &v : variables) {
        if (v.second.name == name)
            return true;
    }
    return false;
}

void SymbolTable::clearVariables() {
    for (auto &v : variables) {
        generator.put(v.first);
    }
    variables.clear();
}

UUID SymbolTable::addConstant(const Constant &constant) {
    std::string prefix = "Cannot add constant, ";
    if (hasVariable(constant.name))
        throw std::runtime_error(prefix + "Variable with name " + constant.name + " already exists.");
    if (hasConstant(constant.name))
        throw std::runtime_error(prefix + "Constant with name " + constant.name + " already exists");
    if (hasFunction(constant.name))
        throw std::runtime_error(prefix + "Function with name " + constant.name + " already exists");
    if (hasScript(constant.name))
        throw std::runtime_error(prefix + "Script with name " + constant.name + " already exists");

    UUID id = generator.get();
    constants[id] = constant;
    return id;
}

const Constant &SymbolTable::getConstant(UUID id) const {
    return constants.at(id);
}

void SymbolTable::setConstant(UUID id, const Constant &constant) {
    std::string prefix = "Cannot set constant, ";
    if (hasVariable(constant.name))
        throw std::runtime_error(prefix + "Variable with name " + constant.name + " already exists.");
    if (hasConstant(constant.name) && constants.at(id).name != constant.name)
        throw std::runtime_error(prefix + "Constant with name " + constant.name + " already exists");
    if (hasFunction(constant.name))
        throw std::runtime_error(prefix + "Function with name " + constant.name + " already exists");
    if (hasScript(constant.name))
        throw std::runtime_error(prefix + "Script with name " + constant.name + " already exists");

    constants.at(id) = constant;
}

void SymbolTable::removeConstant(UUID id) {
    constants.erase(id);
    generator.put(id);
}

bool SymbolTable::hasConstant(const std::string &name) const {
    for (auto &v : constants) {
        if (v.second.name == name)
            return true;
    }
    return false;
}

void SymbolTable::clearConstants() {
    for (auto &v : constants) {
        generator.put(v.first);
    }
    constants.clear();
}

UUID SymbolTable::addFunction(const Function &function) {
    std::string prefix = "Cannot add function, ";
    if (hasVariable(function.name))
        throw std::runtime_error(prefix + "Variable with name " + function.name + " already exists.");
    if (hasConstant(function.name))
        throw std::runtime_error(prefix + "Constant with name " + function.name + " already exists");
    if (hasFunction(function.name))
        throw std::runtime_error(prefix + "Function with name " + function.name + " already exists");
    if (hasScript(function.name))
        throw std::runtime_error(prefix + "Script with name " + function.name + " already exists");

    UUID id = generator.get();
    functions[id] = function;
    return id;
}

const Function &SymbolTable::getFunction(UUID id) const {
    return functions.at(id);
}

void SymbolTable::setFunction(UUID id, const Function &function) {
    std::string prefix = "Cannot set function, ";
    if (hasVariable(function.name))
        throw std::runtime_error(prefix + "Variable with name " + function.name + " already exists.");
    if (hasConstant(function.name))
        throw std::runtime_error(prefix + "Constant with name " + function.name + " already exists");
    if (hasFunction(function.name) && functions.at(id).name != function.name)
        throw std::runtime_error(prefix + "Function with name " + function.name + " already exists");
    if (hasScript(function.name))
        throw std::runtime_error(prefix + "Script with name " + function.name + " already exists");

    functions.at(id) = function;
}

void SymbolTable::removeFunction(UUID id) {
    functions.erase(id);
    generator.put(id);
}

bool SymbolTable::hasFunction(const std::string &name) const {
    for (auto &v : functions) {
        if (v.second.name == name)
            return true;
    }
    return false;
}

void SymbolTable::clearFunctions() {
    for (auto &v : functions) {
        generator.put(v.first);
    }
    functions.clear();
}

UUID SymbolTable::addScript(const Script &script) {
    std::string prefix = "Cannot add script, ";
    if (hasVariable(script.name))
        throw std::runtime_error(prefix + "Variable with name " + script.name + " already exists.");
    if (hasConstant(script.name))
        throw std::runtime_error(prefix + "Constant with name " + script.name + " already exists");
    if (hasFunction(script.name))
        throw std::runtime_error(prefix + "Function with name " + script.name + " already exists");
    if (hasScript(script.name))
        throw std::runtime_error(prefix + "Script with name " + script.name + " already exists");

    UUID id = generator.get();
    scripts[id] = script;
    return id;
}

const Script &SymbolTable::getScript(UUID id) const {
    return scripts.at(id);
}

void SymbolTable::setScript(UUID id, const Script &script) {
    std::string prefix = "Cannot set script, ";
    if (hasVariable(script.name))
        throw std::runtime_error(prefix + "Variable with name " + script.name + " already exists.");
    if (hasConstant(script.name))
        throw std::runtime_error(prefix + "Constant with name " + script.name + " already exists");
    if (hasFunction(script.name))
        throw std::runtime_error(prefix + "Function with name " + script.name + " already exists");
    if (hasScript(script.name) && scripts.at(id).name != script.name)
        throw std::runtime_error(prefix + "Script with name " + script.name + " already exists");

    scripts.at(id) = script;
}

void SymbolTable::removeScript(UUID id) {
    scripts.erase(id);
}

bool SymbolTable::hasScript(const std::string &name) const {
    for (auto &v : scripts) {
        if (v.second.name == name)
            return true;
    }
    return false;
}

void SymbolTable::clearScripts() {
    for (auto &v : scripts) {
        generator.put(v.first);
    }
    scripts.clear();
}
