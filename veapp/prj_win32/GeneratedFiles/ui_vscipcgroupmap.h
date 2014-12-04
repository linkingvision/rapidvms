/********************************************************************************
** Form generated from reading UI file 'vscipcgroupmap.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VSCIPCGROUPMAP_H
#define UI_VSCIPCGROUPMAP_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VSCVGroupMap
{
public:
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;
    QSpacerItem *horizontalSpacer_2;
    QLabel *name;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QWidget *VSCVGroupMap)
    {
        if (VSCVGroupMap->objectName().isEmpty())
            VSCVGroupMap->setObjectName(QStringLiteral("VSCVGroupMap"));
        VSCVGroupMap->resize(300, 245);
        VSCVGroupMap->setMinimumSize(QSize(0, 0));
        VSCVGroupMap->setMaximumSize(QSize(16777215, 16777215));
        VSCVGroupMap->setStyleSheet(QStringLiteral(""));
        verticalLayout = new QVBoxLayout(VSCVGroupMap);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        label_2 = new QLabel(VSCVGroupMap);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton = new QPushButton(VSCVGroupMap);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setMinimumSize(QSize(128, 128));
        pushButton->setMaximumSize(QSize(128, 128));
        QIcon icon;
        icon.addFile(QStringLiteral(":/device/resources/camgroup.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton->setIcon(icon);
        pushButton->setIconSize(QSize(120, 120));

        horizontalLayout->addWidget(pushButton);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout);

        name = new QLabel(VSCVGroupMap);
        name->setObjectName(QStringLiteral("name"));
        name->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(name);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);


        retranslateUi(VSCVGroupMap);

        QMetaObject::connectSlotsByName(VSCVGroupMap);
    } // setupUi

    void retranslateUi(QWidget *VSCVGroupMap)
    {
        VSCVGroupMap->setWindowTitle(QApplication::translate("VSCVGroupMap", "Form", 0));
        label_2->setText(QString());
        pushButton->setText(QString());
        name->setText(QApplication::translate("VSCVGroupMap", "Group", 0));
    } // retranslateUi

};

namespace Ui {
    class VSCVGroupMap: public Ui_VSCVGroupMap {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VSCIPCGROUPMAP_H
