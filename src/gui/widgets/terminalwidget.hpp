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

#ifndef QCALC_TERMINALWIDGET_HPP
#define QCALC_TERMINALWIDGET_HPP

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QSizePolicy>
#include <QScrollBar>

class TerminalWidget : public QWidget {
Q_OBJECT
public:
    explicit TerminalWidget(QWidget *parent = nullptr) : QWidget(parent) {
        historyLabel = new QLabel();
        container = new QWidget();
        promptLabel = new QLabel();
        inputEdit = new QLineEdit();
        scroll = new QScrollArea();

        historyLabel->setTextInteractionFlags(Qt::TextInteractionFlag::LinksAccessibleByKeyboard
                                              | Qt::TextInteractionFlag::LinksAccessibleByMouse
                                              | Qt::TextInteractionFlag::TextSelectableByKeyboard
                                              | Qt::TextInteractionFlag::TextSelectableByMouse);

        scroll->setFrameShadow(QFrame::Plain);
        scroll->setFrameShape(QFrame::NoFrame);

        scroll->setWidget(historyLabel);
        scroll->setAlignment(Qt::AlignmentFlag::AlignBottom);
        scroll->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        scroll->setWidgetResizable(true);

        container->setLayout(new QHBoxLayout());
        container->layout()->addWidget(promptLabel);
        container->layout()->addWidget(inputEdit);
        container->layout()->setMargin(0);

        promptLabel->setText(">>>");

        historyLabel->setContentsMargins(0, 0, 0, 0);
        historyLabel->setAlignment(Qt::AlignmentFlag::AlignBottom);
        historyLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        inputEdit->setStyleSheet("QLineEdit { background-color: rgba(0, 0, 0, 0); }");
        inputEdit->setFrame(false);

        connect(inputEdit, SIGNAL(returnPressed()), this, SIGNAL(onReturnPressed()));

        connect(scroll->verticalScrollBar(), &QScrollBar::rangeChanged, [this](int min, int max) {
            scroll->verticalScrollBar()->setValue(max);
        });

        auto l = new QVBoxLayout();
        l->setMargin(0);
        l->addWidget(scroll, 1);
        l->addWidget(container);
        setLayout(l);
    }

signals:

    void onReturnPressed();

public slots:

    void appendHistory(const QString &command, const QString &result) {
        historyLabel->setText(historyLabel->text() + "\n>>> " + command + "\n" + result);
        if (historyLabel->text().endsWith("\n"))
            historyLabel->setText(historyLabel->text().chopped(1));
        inputEdit->setText("");
    }

    QString getInputText() {
        return inputEdit->text();
    }

    void setInputText(const QString &text) {
        inputEdit->setText(text);
    }

    QString getPrompt() {
        return promptLabel->text();
    }

    void setPrompt(const QString &text) {
        promptLabel->setText(text);
    }

    QString getHistoryText() {
        return historyLabel->text();
    }

    void setHistoryText(const QString &text) {
        historyLabel->setText(text);
    }

private:
    QLabel *historyLabel;
    QWidget *container;
    QLabel *promptLabel;
    QScrollArea *scroll;
    QLineEdit *inputEdit;
};

#endif //QCALC_TERMINALWIDGET_HPP
