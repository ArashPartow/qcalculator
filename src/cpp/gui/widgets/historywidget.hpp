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

#ifndef QCALC_HISTORYWIDGET_HPP
#define QCALC_HISTORYWIDGET_HPP

#include <QScrollArea>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollBar>
#include <QTimer>
#include <QCoreApplication>

#include "gui/widgets/historylabel.hpp"

class HistoryWidget : public QWidget {
Q_OBJECT
public:
    explicit HistoryWidget(QWidget *parent = nullptr) : QWidget(parent) {
        setLayout(new QVBoxLayout());
        layout()->setMargin(0);

        scroll = new QScrollArea(this);

        container = new QWidget(this);
        container->setLayout(new QVBoxLayout());
        container->layout()->setMargin(6);
        container->layout()->setSpacing(12);

        scroll->setWidget(container);
        scroll->setWidgetResizable(true);
        scroll->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        layout()->addWidget(scroll);

        container->layout()->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    }

public slots:

    void clear() {
        for (auto *p : rows) {
            delete p;
        }
        rows.clear();
    }

    void setContent(const QList<QPair<QString, QString>> &c) {
        clear();
        for (auto &pair : c) {
            addContent(pair.first, pair.second);
        }
    }

    void addContent(const QString &expression, const QString &value) {
        auto *row = new QWidget(container);
        auto *expressionLabel = new HistoryLabel(row);
        auto *equalsLabel = new QLabel(row);
        auto *resultLabel = new HistoryLabel(row);

        expressionLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
        resultLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

        auto *layout = new QHBoxLayout();

        expressionLabel->setAlignment(Qt::AlignLeft);
        equalsLabel->setAlignment(Qt::AlignHCenter);
        resultLabel->setAlignment(Qt::AlignRight);

        row->setLayout(layout);

        row->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

        layout->addWidget(expressionLabel, 4);
        layout->addWidget(equalsLabel, 1);
        layout->addWidget(resultLabel, 4);

        container->layout()->addWidget(row);
        container->layout()->invalidate();
        container->layout()->activate();

        // Call to processEvents is required here so that a scroll to the bottom can be done immediately.
        QCoreApplication::processEvents();

        expressionLabel->setTextElided(expression);
        equalsLabel->setText("=");
        resultLabel->setTextElided(value);

        connect(expressionLabel,
                SIGNAL(onDoubleClick(const QString &)),
                this,
                SIGNAL(onTextDoubleClicked(const QString &)));
        connect(resultLabel,
                SIGNAL(onDoubleClick(const QString &)),
                this,
                SIGNAL(onTextDoubleClicked(const QString &)));

        scroll->verticalScrollBar()->setValue(scroll->verticalScrollBar()->maximum());

        rows.emplace_back(row);
    }

signals:

    void onTextDoubleClicked(const QString &text);

private:
    QScrollArea *scroll;
    QWidget *container;
    std::vector<QWidget *> rows;
};

#endif //QCALC_HISTORYWIDGET_HPP
