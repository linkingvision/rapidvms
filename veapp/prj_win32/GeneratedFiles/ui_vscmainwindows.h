/********************************************************************************
** Form generated from reading UI file 'vscmainwindows.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VSCMAINWINDOWS_H
#define UI_VSCMAINWINDOWS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VSCMainWindowsClass
{
public:
    QWidget *centralWidget;

    void setupUi(QMainWindow *VSCMainWindowsClass)
    {
        if (VSCMainWindowsClass->objectName().isEmpty())
            VSCMainWindowsClass->setObjectName(QStringLiteral("VSCMainWindowsClass"));
        VSCMainWindowsClass->resize(800, 600);
        QIcon icon;
        icon.addFile(QStringLiteral(":/logo/resources/vscsmall.png"), QSize(), QIcon::Normal, QIcon::Off);
        VSCMainWindowsClass->setWindowIcon(icon);
        VSCMainWindowsClass->setStyleSheet(QLatin1String("* {font: 10pt \"Arial Unicode MS\"; }\n"
""));
        centralWidget = new QWidget(VSCMainWindowsClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        VSCMainWindowsClass->setCentralWidget(centralWidget);

        retranslateUi(VSCMainWindowsClass);

        QMetaObject::connectSlotsByName(VSCMainWindowsClass);
    } // setupUi

    void retranslateUi(QMainWindow *VSCMainWindowsClass)
    {
        VSCMainWindowsClass->setWindowTitle(QApplication::translate("VSCMainWindowsClass", "VdcEye Manager  2015 R1", 0));
    } // retranslateUi

};

namespace Ui {
    class VSCMainWindowsClass: public Ui_VSCMainWindowsClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VSCMAINWINDOWS_H
