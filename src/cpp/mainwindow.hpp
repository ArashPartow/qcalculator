#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "model.hpp" //Has to be the first include because python uses "slots" as a variable name and "slots" is a qt reserved word.

#include <QMainWindow>
#include <bitset>
#include <QPushButton>
#include <QSettings>
#include <QTableWidget>

#include "numeralsystem.hpp"
#include "numpadkey.hpp"

#include "view.hpp"
#include "presenter.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

//TODO: Fix bitview layout for win32

/**
 * For behaviours which cannot be directly linked using the standard qt widgets (Like the QTableWidget logic)
 * the main window binds the widget behaviour to itself and emits
 * custom signals which map to the presenter member functions.
 *
 * In this way there is logic offloaded to the view which is bad.
 *
 * Alternatives are giving the presenter access to the ui pointer (Bad)
 * or making the presenter interface less abstract.
 */
class MainWindow : public QMainWindow, View {
Q_OBJECT

signals:

    void signalDecimalSubmit(QString value);

    void signalHexSubmit(QString value);

    void signalOctalSubmit(QString value);

    void signalBinarySubmit(QString value);


    void signalNumPadKeyPressed(NumPadKey key);

    void signalBitViewKeyPressed(int bitIndex);


    void signalSelectedVariableChanged(int index);

    void signalVariableChanged(std::string name, std::string value);


    void signalSelectedConstantChanged(int index);

    void signalConstantChanged(std::string name, std::string value);


    void signalSelectedFunctionChanged(int index);

    void signalFunctionNameChanged(std::string value);

    void signalFunctionBodyChanged(std::string value);

    void signalFunctionArgsChanged(std::vector<std::string> arguments);


    void signalSelectedScriptChanged(int index);

    void signalScriptNameChanged(std::string value);

    void signalScriptBodyChanged(std::string value);

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

    void closeEvent(QCloseEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

    //+View
    void connectPresenter(const Presenter &target) override;

    void disconnectPresenter(const Presenter &target) override;

    void showAboutDialog() override;

    void showSettingsDialog() override;

    void showWarningDialog(const std::string &title, const std::string &text) override;

    void showInfoDialog(const std::string &title, const std::string &text) override;

    bool showFileChooserDialog(const std::string &title, bool existingFile, std::string &filePathOut) override;

    bool showQuestionDialog(const std::string &title, const std::string &text) override;

    void quit() override;

    void setInputText(const std::string &value) override;

    void setValueText(const std::string &value) override;

    void setKeyPadVisibility(bool visible) override;

    void setBitViewVisibility(bool visible) override;

    void setDockVisibility(bool visible) override;

    void setActiveDockTab(int tab) override;

    void setDockPosition(Qt::DockWidgetArea position) override;

    void setBitViewContents(std::bitset<64> value) override;

    void setBitViewEnabled(bool enabled) override;

    void setNumericSystemsEnabled(bool enabled) override;

    void setDecimalText(const std::string &value) override;

    void setHexText(const std::string &value) override;

    void setOctalText(const std::string &value) override;

    void setBinaryText(const std::string &value) override;

    void setHistory(const std::vector<std::pair<std::string, std::string>> &value) override;

    void setVariableListView(const std::vector<std::pair<std::string, std::string>> &value) override;

    void setSelectedVariable(int index) override;

    void setConstantsListView(const std::vector<std::pair<std::string, std::string>> &value) override;

    void setSelectedConstant(int index) override;

    void setFunctionsListView(const std::vector<std::string> &value) override;

    void setSelectedFunction(int index) override;

    void setFunctionArgs(const std::vector<std::string> &value) override;

    void setFunctionBody(const std::string &value) override;

    void setFunctionArgsSpinBoxEnabled(bool enabled) override;

    void setFunctionBodyEnabled(bool enabled) override;

    void setScriptsListView(const std::vector<std::string> &value) override;

    void setSelectedScript(int index) override;

    void setScriptBody(const std::string &value) override;

    void setScriptBodyEnabled(bool enabled) override;

    void setScriptEnableArgs(bool value) override;

    void setScriptEnableArgsEnabled(bool value) override;

    void setWindowSize(QSize size) override;
    //-View

private slots:

    void onNumericSystemsFinishEditing();

    void onNumPadKeyPressed();

    void onBitViewKeyPressed();


    void onVariablesSelectionChanged();

    void onVariablesCellChanged(int row, int column);


    void onConstantsSelectionChanged();

    void onConstantsCellChanged(int row, int column);


    void onFunctionsSelectionChanged();

    void onFunctionsCellChanged(int row, int column);

    void onFunctionsArgCountValueChanged(int value);

    void onFunctionsArgEditingFinished();

    void onFunctionsBodyTextChanged();


    void onScriptsSelectionChanged();

    void onScriptsCellChanged(int row, int column);

    void onScriptsBodyTextChanged();

private:

    Ui::MainWindow *ui;

    Model model;
    Presenter presenter;

    //Helpers
    QPushButton &getBitButton(int bitIndex);

    static int getBitIndex(const QString &name);

    QPushButton &getKeyPadButton(NumPadKey key);

    static QString getNameFromKey(NumPadKey key);

    static NumPadKey getKeyFromName(const QString &name);
};

#endif // MAINWINDOW_H
