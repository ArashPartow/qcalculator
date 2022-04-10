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

#include "addontab.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>

#include "gui/widgets/addonitemwidget.hpp"

void AddonTab::setAddons(const std::map<std::string, bool> &addonState,
                         const std::map<std::string, Addon> &addonMetadata) {
    listWidget->clear();
    for (auto &addon: addonState) {
        auto *itemWidget = new AddonItemWidget(listWidget);
        itemWidget->setModuleName(addon.first.c_str());
        itemWidget->setModuleEnabled(addon.second);
        itemWidget->setModuleDisplayName(addonMetadata.at(addon.first).getDisplayName().c_str());
        itemWidget->setModuleDescription(
                (addonMetadata.at(addon.first).getDescription() + " ( " + addon.first + " )").c_str());

        auto *item = new QListWidgetItem();
        item->setSizeHint(itemWidget->minimumSizeHint());
        listWidget->addItem(item);
        listWidget->setItemWidget(item, itemWidget);

        connect(itemWidget, SIGNAL(onModuleEnabledChanged(bool)), this, SLOT(onAddonEnableChanged()));
        connect(itemWidget, SIGNAL(onUninstallModule(const QString &)), this, SLOT(onModuleUninstall(const QString &)));
    }
}

void AddonTab::setLibraries(const std::set<std::string> &libs) {
    libListWidget->clear();
    for (auto &lib: libs) {
        auto *itemWidget = new LibraryItemWidget(libListWidget);
        itemWidget->setLibrary(lib.c_str());

        connect(itemWidget,
                SIGNAL(onUninstallLibrary(const QString &)),
                this,
                SIGNAL(libraryUninstall(const QString &)));

        auto *item = new QListWidgetItem();
        item->setSizeHint(itemWidget->minimumSizeHint());
        libListWidget->addItem(item);

        libListWidget->setItemWidget(item, itemWidget);
    }
}

AddonTab::AddonTab(QWidget *parent)
        : QWidget(parent) {
    installButton = new QPushButton(this);
    refreshButton = new QPushButton(this);

    listWidget = new QListWidget(this);

    libListWidget = new QListWidget(this);

    installButton->setText("Install");
    refreshButton->setText("Refresh");

    auto *header = new QWidget(this);
    header->setLayout(new QHBoxLayout(header));
    header->layout()->setMargin(0);
    header->layout()->addWidget(refreshButton);
    header->layout()->addWidget(installButton);

    setLayout(new QVBoxLayout(this));
    layout()->addWidget(new QLabel("Addons"));
    layout()->addWidget(listWidget);
    layout()->addWidget(new QLabel("Libraries"));
    layout()->addWidget(libListWidget);
    layout()->addWidget(header);

    connect(installButton, SIGNAL(pressed()), this, SIGNAL(installPressed()));
    connect(refreshButton, SIGNAL(pressed()), this, SIGNAL(refreshPressed()));
}


void AddonTab::onAddonEnableChanged() {
    emit addonEnableChanged(dynamic_cast<AddonItemWidget *>(sender()));
}

void AddonTab::onModuleUninstall(const QString &module) {
    emit addonUninstall(module);
}
