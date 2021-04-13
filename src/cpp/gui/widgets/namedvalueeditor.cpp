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

#include "gui/widgets/namedvalueeditor.hpp"

#include <QVBoxLayout>
#include <QHeaderView>

NamedValueEditor::NamedValueEditor(QWidget *parent) : QWidget(parent) {
    setLayout(new QVBoxLayout());
    list = new QTableWidget(this);

    list->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    list->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    list->horizontalHeader()->hide();
    list->verticalHeader()->hide();
    list->horizontalHeader()->setStretchLastSection(true);
    list->horizontalHeader()->setDefaultSectionSize(150);

    addLineEditName = new QLineEdit(this);
    addLineEditValue = new QLineEdit(this);
    addPushButton = new QPushButton(this);

    addPushButton->setText("Add");

    connect(addPushButton, SIGNAL(pressed()), this, SLOT(onAddPressed()));

    auto *widgetAdd = new QWidget(this);
    widgetAdd->setLayout(new QHBoxLayout(widgetAdd));
    widgetAdd->layout()->addWidget(addLineEditName);
    widgetAdd->layout()->addWidget(addLineEditValue);
    widgetAdd->layout()->addWidget(addPushButton);
    widgetAdd->layout()->setContentsMargins(0, 0, 0, 0);

    layout()->addWidget(widgetAdd);
    layout()->addWidget(list);

    connect(list, SIGNAL(cellChanged(int, int)), this, SLOT(onTableCellChanged(int, int)));
}

void NamedValueEditor::setValues(const std::map<QString, QString> &values) {
    disconnect(list, SIGNAL(cellChanged(int, int)), this, SLOT(onTableCellChanged(int, int)));

    addLineEditName->setText("");
    addLineEditValue->setText("");

    list->clear();

    list->setColumnCount(2);
    list->setRowCount(values.size());

    mapping.clear();

    int i = 0;
    for (auto &p : values) {
        auto *itemName = new QTableWidgetItem(p.first);
        auto *itemValue = new QTableWidgetItem(p.second);

        list->setItem(i, 0, itemName);
        list->setItem(i, 1, itemValue);
        list->update();

        mapping[i] = p.first.toStdString();

        i++;
    }

    connect(list, SIGNAL(cellChanged(int, int)), this, SLOT(onTableCellChanged(int, int)));
}

void NamedValueEditor::onAddPressed() {
    emit onNamedValueAdded(addLineEditName->text(), addLineEditValue->text());
}

void NamedValueEditor::onTableCellChanged(int row, int column) {
    auto *nameItem = list->item(row, 0);
    auto *valueItem = list->item(row, 1);

    std::string originalName = mapping.at(row);

    if (column == 0) {
        if (nameItem->text().toStdString() != originalName) {
            emit onNameChanged(originalName.c_str(), nameItem->text());
        }
    } else {
        emit onValueChanged(nameItem->text(), valueItem->text());
    }
}