#ifndef QCALC_MODEL_HPP
#define QCALC_MODEL_HPP

#include "calculatorengine.hpp"

#include "valuetype.hpp"
#include "state.hpp"
#include "statelistener.hpp"

/**
 * The model handles the most abstract form of application state.
 *
 * The presenter transforms the abstract form into gui data such as strings,
 * and passes that data to the view.
 */
class Model {
public:
    Model();

    void setStateListener(StateListener &listener);

    void evaluateInput();

    void updateInput(const std::string &input);

    void updateValue(ValueType value);

    void addVariable(const Variable &var);

    void removeVariable(int index);

    void updateVariable(int index, const Variable &value);

    void addConstant(const Constant &value);

    void removeConstant(int index);

    void updateConstant(int index, const Constant &value);

    void addFunction(const Function &value);

    void removeFunction(int index);

    void updateFunction(int index, const Function &value);

    void addScript(const Script &value);

    void removeScript(int index);

    void updateScript(int index, const Script &value);

    void updateShowKeyPad(bool value);

    void updateShowBitView(bool value);

    void updateShowDock(bool value);

    void updateHistoryLimit(int limit);

    void updateDockPosition(Qt::DockWidgetArea position);

    void updateDockSelectedTab(int tab);

    void updateWindowSize(QSize size);

    void updateCurrentVariable(int currentVariable);

    void updateCurrentConstant(int currentConstant);

    void updateCurrentFunction(int currentFunction);

    void updateCurrentScript(int currentScript);

    void importSymbolTable(const std::string &filePath);

    void exportSymbolTable(const std::string &filePath);

    void loadSettings();

    void saveSettings();

    const State &getState();

private:
    StateListener *listener;
    CalculatorEngine engine;
    State state;
};

#endif //QCALC_MODEL_HPP
