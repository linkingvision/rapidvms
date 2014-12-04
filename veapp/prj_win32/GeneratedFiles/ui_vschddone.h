/********************************************************************************
** Form generated from reading UI file 'vschddone.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VSCHDDONE_H
#define UI_VSCHDDONE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VSCHddOne
{
public:
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton;
    QVBoxLayout *verticalLayout;
    QSlider *horizontalSlider;
    QHBoxLayout *horizontalLayout;
    QLabel *labelDiskName;
    QSpacerItem *horizontalSpacer_3;
    QLabel *labelTotal1;
    QLabel *labelTotal;
    QSpacerItem *horizontalSpacer_2;
    QLabel *labelFree1;
    QLabel *labelFree;
    QSpacerItem *horizontalSpacer;
    QCheckBox *checkBox;

    void setupUi(QWidget *VSCHddOne)
    {
        if (VSCHddOne->objectName().isEmpty())
            VSCHddOne->setObjectName(QStringLiteral("VSCHddOne"));
        VSCHddOne->resize(720, 64);
        VSCHddOne->setMinimumSize(QSize(0, 64));
        VSCHddOne->setMaximumSize(QSize(720, 64));
        QIcon icon;
        icon.addFile(QStringLiteral(":/logo/resources/vscsmall.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        VSCHddOne->setWindowIcon(icon);
        horizontalLayout_2 = new QHBoxLayout(VSCHddOne);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        pushButton = new QPushButton(VSCHddOne);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setMinimumSize(QSize(64, 64));
        pushButton->setMaximumSize(QSize(64, 64));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/device/resources/harddisk.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton->setIcon(icon1);
        pushButton->setIconSize(QSize(65, 65));

        horizontalLayout_2->addWidget(pushButton);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalSlider = new QSlider(VSCHddOne);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setStyleSheet(QLatin1String("QSlider::groove:horizontal {\n"
"border: 1px solid #999999;\n"
"height: 8px; /* the groove expands to the size of the slider by default. by giving it a height, it has a fixed size */\n"
"background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #B1B1B1, stop:1 #c4c4c4);\n"
"}\n"
"QSlider::handle:horizontal {\n"
"background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #b4b4b4, stop:1 #8f8f8f);\n"
"border: 1px solid #5c5c5c;\n"
"width: 18px;\n"
"margin: -2px 0; /* handle is placed by default on the contents rect of the groove. Expand outside the groove */\n"
"border-radius: 3px;\n"
"}"));
        horizontalSlider->setOrientation(Qt::Horizontal);

        verticalLayout->addWidget(horizontalSlider);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        labelDiskName = new QLabel(VSCHddOne);
        labelDiskName->setObjectName(QStringLiteral("labelDiskName"));
        labelDiskName->setMinimumSize(QSize(0, 30));
        labelDiskName->setMaximumSize(QSize(16777215, 30));
        labelDiskName->setStyleSheet(QStringLiteral("font: 75 16pt \"Arial\";"));

        horizontalLayout->addWidget(labelDiskName);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        labelTotal1 = new QLabel(VSCHddOne);
        labelTotal1->setObjectName(QStringLiteral("labelTotal1"));
        labelTotal1->setMinimumSize(QSize(0, 30));
        labelTotal1->setMaximumSize(QSize(16777215, 30));
        labelTotal1->setStyleSheet(QStringLiteral("font: 12pt \"Arial\";"));

        horizontalLayout->addWidget(labelTotal1);

        labelTotal = new QLabel(VSCHddOne);
        labelTotal->setObjectName(QStringLiteral("labelTotal"));
        labelTotal->setMinimumSize(QSize(0, 30));
        labelTotal->setMaximumSize(QSize(16777215, 30));
        labelTotal->setStyleSheet(QStringLiteral("font: 12pt \"Arial\";"));

        horizontalLayout->addWidget(labelTotal);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        labelFree1 = new QLabel(VSCHddOne);
        labelFree1->setObjectName(QStringLiteral("labelFree1"));
        labelFree1->setMinimumSize(QSize(0, 30));
        labelFree1->setMaximumSize(QSize(16777215, 30));
        labelFree1->setStyleSheet(QStringLiteral("font: 12pt \"Arial\";"));

        horizontalLayout->addWidget(labelFree1);

        labelFree = new QLabel(VSCHddOne);
        labelFree->setObjectName(QStringLiteral("labelFree"));
        labelFree->setMinimumSize(QSize(0, 30));
        labelFree->setMaximumSize(QSize(16777215, 30));
        labelFree->setStyleSheet(QStringLiteral("font: 12pt \"Arial\";"));

        horizontalLayout->addWidget(labelFree);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        checkBox = new QCheckBox(VSCHddOne);
        checkBox->setObjectName(QStringLiteral("checkBox"));
        checkBox->setMinimumSize(QSize(15, 0));
        checkBox->setMaximumSize(QSize(15, 16777215));

        horizontalLayout->addWidget(checkBox);


        verticalLayout->addLayout(horizontalLayout);


        horizontalLayout_2->addLayout(verticalLayout);


        retranslateUi(VSCHddOne);

        QMetaObject::connectSlotsByName(VSCHddOne);
    } // setupUi

    void retranslateUi(QWidget *VSCHddOne)
    {
        VSCHddOne->setWindowTitle(QApplication::translate("VSCHddOne", "Form", 0));
        pushButton->setText(QString());
        labelDiskName->setText(QApplication::translate("VSCHddOne", "C:\\ ", 0));
        labelTotal1->setText(QApplication::translate("VSCHddOne", "Total:", 0));
        labelTotal->setText(QApplication::translate("VSCHddOne", "1T", 0));
        labelFree1->setText(QApplication::translate("VSCHddOne", "Free:", 0));
        labelFree->setText(QApplication::translate("VSCHddOne", "25G", 0));
        checkBox->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class VSCHddOne: public Ui_VSCHddOne {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VSCHDDONE_H
