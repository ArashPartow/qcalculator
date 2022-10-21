/**
 *  QCalc - Extensible programming calculator
 *  Copyright (C) 2022  Julian Zampiccoli
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "widgets/symbolseditor.hpp"

#include <QTabWidget>
#include <QVBoxLayout>
#include <QMessageBox>

std::map<QString, QString> convertMap(const std::map<std::string, decimal::Decimal> &map) {
    std::map<QString, QString> ret;
    for (auto &p: map) {
        ret[QString(p.first.c_str())] = p.second.format("f").c_str();
    }
    return ret;
}

SymbolsEditor::SymbolsEditor(QWidget *parent) : QWidget(parent) {
    setLayout(new QVBoxLayout(this));

    layout()->setContentsMargins(3, 3, 3, 3);

    auto *tabs = new QTabWidget(this);

    variablesEditor = new NamedValueEditor(tabs);
    constantsEditor = new NamedValueEditor(tabs);
    functionsEditor = new FunctionsEditor(tabs);
    scriptsEditor = new ScriptsEditor(tabs);
    builtInsEditor = new BuiltInsEditor(tabs);

    tabs->addTab(variablesEditor, "Variables");
    tabs->addTab(constantsEditor, "Constants");
    tabs->addTab(functionsEditor, "Functions");
    tabs->addTab(scriptsEditor, "Scripts");
    tabs->addTab(builtInsEditor, "Built-Ins");

    layout()->addWidget(tabs);

    connect(variablesEditor,
            SIGNAL(onNamedValueAdded(const QString &, const QString &)),
            this,
            SLOT(onVariableAdded(const QString &, const QString &)));
    connect(variablesEditor,
            SIGNAL(onNameChanged(const QString &, const QString &)),
            this,
            SLOT(onVariableNameChanged(const QString &, const QString &)));
    connect(variablesEditor,
            SIGNAL(onValueChanged(const QString &, const QString &)),
            this,
            SLOT(onVariableValueChanged(const QString &, const QString &)));

    connect(constantsEditor,
            SIGNAL(onNamedValueAdded(const QString &, const QString &)),
            this,
            SLOT(onConstantAdded(const QString &, const QString &)));
    connect(constantsEditor,
            SIGNAL(onNameChanged(const QString &, const QString &)),
            this,
            SLOT(onConstantNameChanged(const QString &, const QString &)));
    connect(constantsEditor,
            SIGNAL(onValueChanged(const QString &, const QString &)),
            this,
            SLOT(onConstantValueChanged(const QString &, const QString &)));

    connect(functionsEditor,
            SIGNAL(onFunctionAdded(const QString &)),
            this,
            SLOT(onFunctionAdded(const QString &)));
    connect(functionsEditor,
            SIGNAL(onFunctionNameChanged(const QString &, const QString &)),
            this,
            SLOT(onFunctionNameChanged(const QString &, const QString &)));
    connect(functionsEditor,
            SIGNAL(onFunctionBodyChanged(const QString &, const QString &)),
            this,
            SLOT(onFunctionBodyChanged(const QString &, const QString &)));
    connect(functionsEditor,
            SIGNAL(onFunctionArgsChanged(const QString &, const std::vector<std::string> &)),
            this,
            SLOT(onFunctionArgsChanged(const QString &, const std::vector<std::string> &)));
    connect(functionsEditor,
            SIGNAL(onCurrentFunctionChanged(const QString &)),
            this,
            SLOT(onCurrentFunctionChanged(const QString &)));

    connect(builtInsEditor,
            SIGNAL(onUseBuiltInConstantChanged(bool)),
            this,
            SLOT(onUseBuiltInsChanged(bool)));
}

void SymbolsEditor::setSymbols(const SymbolTable &symtable) {
    symbolTable = symtable;

    variablesEditor->setValues(convertMap(symbolTable.getVariables()));
    constantsEditor->setValues(convertMap(symbolTable.getConstants()));
    functionsEditor->setFunctions(symbolTable.getFunctions());
    functionsEditor->setCurrentFunction(currentFunction);
    scriptsEditor->setScripts(symbolTable.getScripts());
    builtInsEditor->setUseBuiltInConstants(symbolTable.getUseBuiltInConstants());
}

void SymbolsEditor::onVariableAdded(const QString &name, const QString &value) {
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Failed to add variable", "Variable name cannot be empty.");
    } else if (symbolTable.hasVariable(name.toStdString())) {
        QMessageBox::warning(this, "Failed to add variable", "A variable with the name already exists.");
    } else if (symbolTable.hasConstant(name.toStdString())) {
        QMessageBox::warning(this, "Failed to add variable", "A constant with the name already exists.");
    } else if (symbolTable.hasFunction(name.toStdString())) {
        QMessageBox::warning(this, "Failed to add variable", "A function with the name already exists.");
    } else if (symbolTable.hasScript(name.toStdString())) {
        QMessageBox::warning(this, "Failed to add variable", "A script with the name already exists.");
    } else if (std::find(name.begin(), name.end(), ' ') != name.end()) {
        QMessageBox::warning(this, "Failed to add variable", "Variable name cannot contain whitespace.");
    } else if (name.at(0) >= '0' && name.at(0) <= '9') {
        QMessageBox::warning(this, "Failed to add variable", "Variable name cannot start with number.");
    } else {
        decimal::Decimal valueConverted;;
        if (value.isEmpty()) {
            valueConverted = 0;
        } else {
            try {
                valueConverted = decimal::Decimal(value.toStdString());
            } catch (const std::exception &e) {
                valueConverted = 0;
                QMessageBox::warning(this, "Failed to convert value", "Failed to parse value as decimal.");
            }
        }
        symbolTable.setVariable(name.toStdString(), valueConverted);
        emit onSymbolsChanged(symbolTable);
    }
}

void SymbolsEditor::onVariableNameChanged(const QString &originalName, const QString &name) {
    if (name.isEmpty()) {
        if (QMessageBox::question(this, "Delete variable",
                                  "Do you want to delete the variable " + originalName + " ?")) {
            symbolTable.remove(originalName.toStdString());
        }
        emit onSymbolsChanged(symbolTable);
    } else if (symbolTable.hasVariable(name.toStdString())) {
        QMessageBox::warning(this, "Failed to changed variable name", "A variable with the name already exists.");
        variablesEditor->setValues(convertMap(symbolTable.getVariables()));
    } else if (symbolTable.hasConstant(name.toStdString())) {
        QMessageBox::warning(this, "Failed to changed variable name", "A constant with the name already exists.");
        variablesEditor->setValues(convertMap(symbolTable.getVariables()));
    } else if (symbolTable.hasFunction(name.toStdString())) {
        QMessageBox::warning(this, "Failed to changed variable name", "A function with the name already exists.");
        variablesEditor->setValues(convertMap(symbolTable.getVariables()));
    } else if (symbolTable.hasScript(name.toStdString())) {
        QMessageBox::warning(this, "Failed to changed variable name", "A script with the name already exists.");
        variablesEditor->setValues(convertMap(symbolTable.getVariables()));
    } else if (std::find(name.begin(), name.end(), ' ') != name.end()) {
        QMessageBox::warning(this, "Failed to add variable", "Variable name cannot contain whitespace.");
        variablesEditor->setValues(convertMap(symbolTable.getVariables()));
    } else if (name.at(0) >= '0' && name.at(0) <= '9') {
        QMessageBox::warning(this, "Failed to add variable", "Variable name cannot start with number.");
        variablesEditor->setValues(convertMap(symbolTable.getVariables()));
    } else {
        decimal::Decimal value = symbolTable.getVariables().at(originalName.toStdString());
        symbolTable.setVariable(name.toStdString(), value);
        symbolTable.remove(originalName.toStdString());
        emit onSymbolsChanged(symbolTable);
    }
}

void SymbolsEditor::onVariableValueChanged(const QString &name, const QString &value) {
    decimal::Decimal originalValue = symbolTable.getVariables().at(name.toStdString());
    decimal::Decimal newValue;
    try {
        newValue = decimal::Decimal(value.toStdString());
    } catch (const std::exception &e) {
        newValue = originalValue;
        QMessageBox::warning(this, "Failed to convert value", "Failed to parse value as decimal.");
    }
    symbolTable.setVariable(name.toStdString(), newValue);
    emit onSymbolsChanged(symbolTable);
}

void SymbolsEditor::onConstantAdded(const QString &name, const QString &value) {
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Failed to add constant", "Constant name cannot be empty.");
    } else if (symbolTable.hasVariable(name.toStdString())) {
        QMessageBox::warning(this, "Failed to add constant", "A variable with the name already exists.");
    } else if (symbolTable.hasConstant(name.toStdString())) {
        QMessageBox::warning(this, "Failed to add constant", "A constant with the name already exists.");
    } else if (symbolTable.hasFunction(name.toStdString())) {
        QMessageBox::warning(this, "Failed to add constant", "A function with the name already exists.");
    } else if (symbolTable.hasScript(name.toStdString())) {
        QMessageBox::warning(this, "Failed to add constant", "A script with the name already exists.");
    } else if (std::find(name.begin(), name.end(), ' ') != name.end()) {
        QMessageBox::warning(this, "Failed to add constant", "Constant name cannot contain whitespace.");
    } else if (name.at(0) >= '0' && name.at(0) <= '9') {
        QMessageBox::warning(this, "Failed to add constant", "Constant name cannot start with number.");
    } else {
        decimal::Decimal valueConverted;
        if (value.isEmpty()) {
            valueConverted = 0;
        } else {
            try {
                valueConverted = decimal::Decimal(value.toStdString());
            } catch (const std::exception &e) {
                valueConverted = 0;
                QMessageBox::warning(this, "Failed to convert value", "Failed to parse value as decimal.");
            }
        }
        symbolTable.setConstant(name.toStdString(), valueConverted);
        emit onSymbolsChanged(symbolTable);
    }
}

void SymbolsEditor::onConstantNameChanged(const QString &originalName, const QString &name) {
    if (name.isEmpty()) {
        if (QMessageBox::question(this, "Delete constant",
                                  "Do you want to delete the constant " + originalName + " ?")) {
            symbolTable.remove(originalName.toStdString());
        }
        emit onSymbolsChanged(symbolTable);
    } else if (symbolTable.hasVariable(name.toStdString())) {
        QMessageBox::warning(this, "Failed to change constant name", "A variable with the name already exists.");
        constantsEditor->setValues(convertMap(symbolTable.getConstants()));
    } else if (symbolTable.hasConstant(name.toStdString())) {
        QMessageBox::warning(this, "Failed to change constant name", "A constant with the name already exists.");
        constantsEditor->setValues(convertMap(symbolTable.getConstants()));
    } else if (symbolTable.hasFunction(name.toStdString())) {
        QMessageBox::warning(this, "Failed to change constant name", "A function with the name already exists.");
        constantsEditor->setValues(convertMap(symbolTable.getConstants()));
    } else if (symbolTable.hasScript(name.toStdString())) {
        QMessageBox::warning(this, "Failed to change constant name", "A script with the name already exists.");
        constantsEditor->setValues(convertMap(symbolTable.getConstants()));
    } else if (std::find(name.begin(), name.end(), ' ') != name.end()) {
        QMessageBox::warning(this, "Failed to add constant", "Constant name cannot contain whitespace.");
        constantsEditor->setValues(convertMap(symbolTable.getConstants()));
    } else if (name.at(0) >= '0' && name.at(0) <= '9') {
        QMessageBox::warning(this, "Failed to add constant", "Constant name cannot start with number.");
        constantsEditor->setValues(convertMap(symbolTable.getConstants()));
    } else {
        decimal::Decimal value = symbolTable.getConstants().at(originalName.toStdString());
        symbolTable.setConstant(name.toStdString(), value);
        symbolTable.remove(originalName.toStdString());
        emit onSymbolsChanged(symbolTable);
    }
}

void SymbolsEditor::onConstantValueChanged(const QString &name, const QString &value) {
    decimal::Decimal originalValue = symbolTable.getConstants().at(name.toStdString());
    decimal::Decimal newValue;
    try {
        newValue = decimal::Decimal(value.toStdString());
    } catch (const std::exception &e) {
        newValue = originalValue;
        QMessageBox::warning(this, "Failed to convert value", "Failed to parse value as decimal.");
    }
    symbolTable.setConstant(name.toStdString(), newValue);
    emit onSymbolsChanged(symbolTable);
}

void SymbolsEditor::onFunctionAdded(const QString &name) {
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Failed to add function", "Function name cannot be empty.");
    } else if (symbolTable.hasVariable(name.toStdString())) {
        QMessageBox::warning(this, "Failed to add function", "A variable with the name already exists.");
    } else if (symbolTable.hasConstant(name.toStdString())) {
        QMessageBox::warning(this, "Failed to add function", "A constant with the name already exists.");
    } else if (symbolTable.hasFunction(name.toStdString())) {
        QMessageBox::warning(this, "Failed to add function", "A function with the name already exists.");
    } else if (symbolTable.hasScript(name.toStdString())) {
        QMessageBox::warning(this, "Failed to add function", "A script with the name already exists.");
    } else if (std::find(name.begin(), name.end(), ' ') != name.end()) {
        QMessageBox::warning(this, "Failed to add function", "Function name cannot contain whitespace.");
    } else if (name.at(0) >= '0' && name.at(0) <= '9') {
        QMessageBox::warning(this, "Failed to add function", "Function name cannot start with number.");
    } else {
        symbolTable.setFunction(name.toStdString(), {});
        emit onSymbolsChanged(symbolTable);
    }
}

void SymbolsEditor::onFunctionNameChanged(const QString &originalName, const QString &name) {
    if (name.isEmpty()) {
        if (QMessageBox::question(this, "Delete function",
                                  "Do you want to delete the function " + originalName + " ?")) {
            symbolTable.remove(originalName.toStdString());
        }
        emit onSymbolsChanged(symbolTable);
    } else if (symbolTable.hasVariable(name.toStdString())) {
        QMessageBox::warning(this, "Failed to change function name", "A variable with the name already exists.");
        functionsEditor->setFunctions(symbolTable.getFunctions());
        functionsEditor->setCurrentFunction(currentFunction);
    } else if (symbolTable.hasConstant(name.toStdString())) {
        QMessageBox::warning(this, "Failed to change function name", "A constant with the name already exists.");
        functionsEditor->setFunctions(symbolTable.getFunctions());
        functionsEditor->setCurrentFunction(currentFunction);
    } else if (symbolTable.hasFunction(name.toStdString())) {
        QMessageBox::warning(this, "Failed to change function name", "A function with the name already exists.");
        functionsEditor->setFunctions(symbolTable.getFunctions());
        functionsEditor->setCurrentFunction(currentFunction);
    } else if (symbolTable.hasScript(name.toStdString())) {
        QMessageBox::warning(this, "Failed to change function name", "A script with the name already exists.");
        functionsEditor->setFunctions(symbolTable.getFunctions());
        functionsEditor->setCurrentFunction(currentFunction);
    } else if (std::find(name.begin(), name.end(), ' ') != name.end()) {
        QMessageBox::warning(this, "Failed to add function", "Function name cannot contain whitespace.");
        functionsEditor->setFunctions(symbolTable.getFunctions());
        functionsEditor->setCurrentFunction(currentFunction);
    } else if (name.at(0) >= '0' && name.at(0) <= '9') {
        QMessageBox::warning(this, "Failed to add function", "Function name cannot start with number.");
        functionsEditor->setFunctions(symbolTable.getFunctions());
        functionsEditor->setCurrentFunction(currentFunction);
    } else {
        Function f = symbolTable.getFunctions().at(originalName.toStdString());
        symbolTable.remove(originalName.toStdString());
        symbolTable.setFunction(name.toStdString(), f);
        emit onSymbolsChanged(symbolTable);
    }
}

void SymbolsEditor::onFunctionBodyChanged(const QString &name, const QString &body) {
    Function f = symbolTable.getFunctions().at(name.toStdString());
    f.expression = body.toStdString();
    symbolTable.setFunction(name.toStdString(), f);
    emit onSymbolsChanged(symbolTable);
}

void SymbolsEditor::onFunctionArgsChanged(const QString &name, const std::vector<std::string> &args) {
    Function f = symbolTable.getFunctions().at(name.toStdString());
    f.argumentNames = args;
    symbolTable.setFunction(name.toStdString(), f);
    emit onSymbolsChanged(symbolTable);
}

void SymbolsEditor::onCurrentFunctionChanged(const QString &name) {
    currentFunction = name;
}

void SymbolsEditor::onUseBuiltInsChanged(bool useBuiltIns) {
    symbolTable.setUseBuiltInConstants(useBuiltIns);
    emit onSymbolsChanged(symbolTable);
}
