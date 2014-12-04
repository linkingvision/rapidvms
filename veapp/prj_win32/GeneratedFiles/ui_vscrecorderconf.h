/********************************************************************************
** Form generated from reading UI file 'vscrecorderconf.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VSCRECORDERCONF_H
#define UI_VSCRECORDERCONF_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VSCRecorderConf
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;

    void setupUi(QWidget *VSCRecorderConf)
    {
        if (VSCRecorderConf->objectName().isEmpty())
            VSCRecorderConf->setObjectName(QStringLiteral("VSCRecorderConf"));
        VSCRecorderConf->resize(570, 360);
        VSCRecorderConf->setMinimumSize(QSize(0, 0));
        VSCRecorderConf->setMaximumSize(QSize(16777215, 16777215));
        QIcon icon;
        icon.addFile(QStringLiteral(":/logo/resources/vscsmall.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        VSCRecorderConf->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(VSCRecorderConf);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));

        verticalLayout->addLayout(gridLayout);


        retranslateUi(VSCRecorderConf);

        QMetaObject::connectSlotsByName(VSCRecorderConf);
    } // setupUi

    void retranslateUi(QWidget *VSCRecorderConf)
    {
        VSCRecorderConf->setWindowTitle(QApplication::translate("VSCRecorderConf", "Master Hard Disk Selecting", 0));
    } // retranslateUi

};

namespace Ui {
    class VSCRecorderConf: public Ui_VSCRecorderConf {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VSCRECORDERCONF_H
