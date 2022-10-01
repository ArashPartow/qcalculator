/**
 *  QCalc - Extensible programming calculator
 *  Copyright (C) 2021  Julian Zampiccoli
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

#include "settingsdialog.hpp"

#include <QSettings>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>

#include <fstream>

#include "gui/widgets/addonitemwidget.hpp"
#include "gui/dialog/addoninstalldialog.hpp"

#include "addon/addonmanager.hpp"

#include "io/archive.hpp"

#include "io/paths.hpp"

SettingsDialog::SettingsDialog(AddonManager &addonManager, QWidget *parent) :
        QDialog(parent), addonManager(addonManager) {
    setWindowTitle("Settings");

    setLayout(new QVBoxLayout(this));

    okButton = new QPushButton(this);
    cancelButton = new QPushButton(this);

    tabWidget = new QTabWidget(this);

    generalTab = new GeneralTab(this);
    addonTab = new AddonTab(this);

    okButton->setText("Ok");
    cancelButton->setText("Cancel");

    tabWidget->addTab(generalTab, "General");
    tabWidget->addTab(addonTab, "Addons");

    tabWidget->setTabPosition(QTabWidget::West);

    auto *footer = new QWidget();
    auto *footerLayout = new QHBoxLayout(footer);
    footerLayout->setMargin(0);
    footerLayout->addWidget(new QWidget(), 1);
    footer->layout()->addWidget(okButton);
    footer->layout()->addWidget(cancelButton);
    footer->setLayout(footerLayout);

    layout()->addWidget(tabWidget);
    layout()->addWidget(footer);

    connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(onSettingsTabChanged(int)));

    connect(okButton, SIGNAL(pressed()), this, SLOT(onDialogAccepted()));
    connect(cancelButton, SIGNAL(pressed()), this, SLOT(onDialogRejected()));

    connect(addonTab, SIGNAL(installPressed()), this, SLOT(onInstallAddonPressed()));
    connect(addonTab, SIGNAL(refreshPressed()), this, SLOT(onRefreshAddonsPressed()));
    connect(addonTab, SIGNAL(addonEnableChanged(AddonItemWidget * )), this,
            SLOT(onModuleEnableChanged(AddonItemWidget * )));
    connect(addonTab, SIGNAL(addonUninstall(const QString &)), this, SLOT(onAddonUninstall(const QString &)));

    resize({700, 500});
}

SettingsDialog::~SettingsDialog() = default;

void SettingsDialog::setEnabledAddons(const std::set<std::string> &addons) {
    enabledAddons = addons;

    addonTab->setAddons(addonManager.getAvailableAddons());
}

std::set<std::string> SettingsDialog::getEnabledAddons() {
    return enabledAddons;
}

void SettingsDialog::setPrecision(int precision) {
    generalTab->setPrecision(precision);
}

int SettingsDialog::getPrecision() {
    return generalTab->getPrecision();
}

void SettingsDialog::setExponentMax(int max) {
    generalTab->setExponentMax(max);
}

int SettingsDialog::getExponentMax() {
    return generalTab->getExponentMax();
}

void SettingsDialog::setExponentMin(int min) {
    generalTab->setExponentMin(min);
}

int SettingsDialog::getExponentMin() {
    return generalTab->getExponentMin();
}

void SettingsDialog::setRoundingMode(decimal::round rounding) {
    generalTab->setRounding(rounding);
}

decimal::round SettingsDialog::getRoundingMode() {
    return generalTab->getRounding();
}

void SettingsDialog::setShowInexactWarning(bool showWarning) {
    generalTab->setShowInexactWarning(showWarning);
}

bool SettingsDialog::getShowInexactWarning() {
    return generalTab->getShowInexactWarning();
}

void SettingsDialog::setPythonModPaths(const std::set<std::string> &paths) {
    generalTab->setPythonModPaths(paths);
}

std::set<std::string> SettingsDialog::getPythonModPaths() {
    return generalTab->getPythonModPaths();
}

void SettingsDialog::setPythonPath(const std::string &path) {
    generalTab->setPythonPath(path);
}

std::string SettingsDialog::getPythonPath() {
    return generalTab->getPythonPath();
}

void SettingsDialog::onModuleEnableChanged(AddonItemWidget *item) {
    std::string name = item->getModuleName().toStdString();
    bool enabled = item->getModuleEnabled();

    auto it = enabledAddons.find(name);
    if (it != enabledAddons.end()) {
        if (!enabled) {
            enabledAddons.erase(name);
        }
    } else {
        if (enabled) {
            enabledAddons.insert(name);
        }
    }

    addonManager.setActiveAddons(enabledAddons);

    setEnabledAddons(addonManager.getActiveAddons());
}

void SettingsDialog::onDialogAccepted() {
    accept();
}

void SettingsDialog::onDialogRejected() {
    reject();
}

void SettingsDialog::onSettingsTabChanged(int tab) {}

void SettingsDialog::onRefreshAddonsPressed() {
    addonManager.reloadModules();

    enabledAddons = addonManager.getActiveAddons();

    std::map<std::string, Addon> adds = addonManager.getAvailableAddons();

    std::set<std::string> rMod;
    for (auto &mod: enabledAddons)
        if (adds.find(mod) == adds.end())
            rMod.insert(mod);
    for (auto &mod: rMod)
        enabledAddons.erase(mod);

    addonTab->setAddons(adds);
}

void SettingsDialog::onInstallAddonPressed() {
    auto *d = new QFileDialog();
    d->setWindowTitle("Select addon bundle file");
    d->setFileMode(QFileDialog::ExistingFile);
    if (d->exec()) {
        auto file = d->selectedFiles().first().toStdString();
        delete d;
        try {
            std::ifstream ifs(file);
            auto installedAddonCount = addonManager.installAddonBundle(ifs, [this](const std::string &title,
                                                                                   const std::string &text) {
                                                                           return QMessageBox::question(this, title.c_str(), text.c_str()) == QMessageBox::Yes;
                                                                       },
                                                                       [this](const std::string &title,
                                                                              const std::string &text,
                                                                              std::vector<std::string> &value) {
                                                                           auto dialog = new AddonInstallDialog(this);
                                                                           dialog->setWindowTitle(title.c_str());
                                                                           dialog->setText(text.c_str());
                                                                           dialog->setItems(value);

                                                                           auto ret = dialog->exec();

                                                                           if (ret == QDialog::Accepted) {
                                                                               value = dialog->getCheckedItems();
                                                                               return true;
                                                                           } else {
                                                                               return false;
                                                                           }
                                                                       });
            if (installedAddonCount > 0) {
                addonManager.unloadAddonLibraryPaths();
                addonManager.loadAddonLibraryPaths();

                QMessageBox::information(this,
                                         "Installation Successful",
                                         ("Successfully installed " + std::to_string(installedAddonCount) +
                                          " addons.").c_str());
            } else {
                QMessageBox::information(this,
                                         "Installation Finished",
                                         "No addons were installed.");
            }
        } catch (const std::exception &e) {
            QMessageBox::critical(this,
                                  "Installation Failed",
                                  ("Failed to install addon package from " + file + "\n" + e.what()).c_str());
        }
        onRefreshAddonsPressed();
    } else {
        delete d;
    }
}

void SettingsDialog::onAddonUninstall(const QString &name) {
    if (QMessageBox::question(this, "Uninstall Addon",
                              "Do you want to uninstall " + name + " ?") == QMessageBox::Yes) {
        addonManager.uninstallAddon(name.toStdString());
        onRefreshAddonsPressed();
    }
}
