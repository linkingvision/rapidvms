/********************************************************************************
** Form generated from reading UI file 'vscplaycontrol.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VSCPLAYCONTROL_H
#define UI_VSCPLAYCONTROL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCalendarWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VSCPlayControl
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QSpacerItem *horizontalSpacer;
    QCalendarWidget *calendarWidget;

    void setupUi(QWidget *VSCPlayControl)
    {
        if (VSCPlayControl->objectName().isEmpty())
            VSCPlayControl->setObjectName(QStringLiteral("VSCPlayControl"));
        VSCPlayControl->resize(500, 60);
        VSCPlayControl->setMaximumSize(QSize(500, 60));
        QIcon icon;
        icon.addFile(QStringLiteral(":/logo/resources/vscsmall.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        VSCPlayControl->setWindowIcon(icon);
        gridLayout = new QGridLayout(VSCPlayControl);
        gridLayout->setSpacing(0);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        pushButton = new QPushButton(VSCPlayControl);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setMinimumSize(QSize(45, 45));
        pushButton->setMaximumSize(QSize(45, 45));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/view/resources/media-playback.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton->setIcon(icon1);
        pushButton->setIconSize(QSize(45, 45));

        horizontalLayout->addWidget(pushButton);

        pushButton_2 = new QPushButton(VSCPlayControl);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setMinimumSize(QSize(45, 45));
        pushButton_2->setMaximumSize(QSize(45, 45));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/view/resources/playback-pause.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_2->setIcon(icon2);
        pushButton_2->setIconSize(QSize(45, 45));

        horizontalLayout->addWidget(pushButton_2);

        pushButton_3 = new QPushButton(VSCPlayControl);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setMinimumSize(QSize(45, 45));
        pushButton_3->setMaximumSize(QSize(45, 45));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/view/resources/playback-stop.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_3->setIcon(icon3);
        pushButton_3->setIconSize(QSize(45, 45));

        horizontalLayout->addWidget(pushButton_3);

        pushButton_4 = new QPushButton(VSCPlayControl);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setMinimumSize(QSize(45, 45));
        pushButton_4->setMaximumSize(QSize(45, 45));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/view/resources/media-r-playback.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_4->setIcon(icon4);
        pushButton_4->setIconSize(QSize(45, 45));

        horizontalLayout->addWidget(pushButton_4);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        calendarWidget = new QCalendarWidget(VSCPlayControl);
        calendarWidget->setObjectName(QStringLiteral("calendarWidget"));
        calendarWidget->setMaximumSize(QSize(16777215, 40));

        horizontalLayout->addWidget(calendarWidget);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);


        retranslateUi(VSCPlayControl);

        QMetaObject::connectSlotsByName(VSCPlayControl);
    } // setupUi

    void retranslateUi(QWidget *VSCPlayControl)
    {
        VSCPlayControl->setWindowTitle(QApplication::translate("VSCPlayControl", "VSC", 0));
        pushButton->setText(QString());
        pushButton_2->setText(QString());
        pushButton_3->setText(QString());
        pushButton_4->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class VSCPlayControl: public Ui_VSCPlayControl {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VSCPLAYCONTROL_H
