/********************************************************************************
** Form generated from reading UI file 'vscsetting.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VSCSETTING_H
#define UI_VSCSETTING_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VSCSetting
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QTreeWidget *treeWidget;
    QWidget *widget;

    void setupUi(QWidget *VSCSetting)
    {
        if (VSCSetting->objectName().isEmpty())
            VSCSetting->setObjectName(QStringLiteral("VSCSetting"));
        VSCSetting->resize(614, 240);
        VSCSetting->setStyleSheet(QStringLiteral(""));
        verticalLayout = new QVBoxLayout(VSCSetting);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(2);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        treeWidget = new QTreeWidget(VSCSetting);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem(treeWidget);
        new QTreeWidgetItem(__qtreewidgetitem);
        new QTreeWidgetItem(__qtreewidgetitem);
        new QTreeWidgetItem(__qtreewidgetitem);
        QTreeWidgetItem *__qtreewidgetitem1 = new QTreeWidgetItem(treeWidget);
        new QTreeWidgetItem(__qtreewidgetitem1);
        treeWidget->setObjectName(QStringLiteral("treeWidget"));
        treeWidget->setMinimumSize(QSize(200, 0));
        treeWidget->setMaximumSize(QSize(200, 16777215));
        treeWidget->setHeaderHidden(true);

        horizontalLayout->addWidget(treeWidget);

        widget = new QWidget(VSCSetting);
        widget->setObjectName(QStringLiteral("widget"));

        horizontalLayout->addWidget(widget);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(VSCSetting);

        QMetaObject::connectSlotsByName(VSCSetting);
    } // setupUi

    void retranslateUi(QWidget *VSCSetting)
    {
        VSCSetting->setWindowTitle(QApplication::translate("VSCSetting", "Form", 0));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("VSCSetting", "1", 0));

        const bool __sortingEnabled = treeWidget->isSortingEnabled();
        treeWidget->setSortingEnabled(false);
        QTreeWidgetItem *___qtreewidgetitem1 = treeWidget->topLevelItem(0);
        ___qtreewidgetitem1->setText(0, QApplication::translate("VSCSetting", "System", 0));
        QTreeWidgetItem *___qtreewidgetitem2 = ___qtreewidgetitem1->child(0);
        ___qtreewidgetitem2->setText(0, QApplication::translate("VSCSetting", "Version", 0));
        QTreeWidgetItem *___qtreewidgetitem3 = ___qtreewidgetitem1->child(1);
        ___qtreewidgetitem3->setText(0, QApplication::translate("VSCSetting", "License", 0));
        QTreeWidgetItem *___qtreewidgetitem4 = ___qtreewidgetitem1->child(2);
        ___qtreewidgetitem4->setText(0, QApplication::translate("VSCSetting", "User", 0));
        QTreeWidgetItem *___qtreewidgetitem5 = treeWidget->topLevelItem(1);
        ___qtreewidgetitem5->setText(0, QApplication::translate("VSCSetting", "Media", 0));
        QTreeWidgetItem *___qtreewidgetitem6 = ___qtreewidgetitem5->child(0);
        ___qtreewidgetitem6->setText(0, QApplication::translate("VSCSetting", "RTSP/RTP", 0));
        treeWidget->setSortingEnabled(__sortingEnabled);

    } // retranslateUi

};

namespace Ui {
    class VSCSetting: public Ui_VSCSetting {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VSCSETTING_H
