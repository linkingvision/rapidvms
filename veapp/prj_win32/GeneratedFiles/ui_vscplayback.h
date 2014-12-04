/********************************************************************************
** Form generated from reading UI file 'vscplayback.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VSCPLAYBACK_H
#define UI_VSCPLAYBACK_H

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

class Ui_VSCPlayback
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_2;
    QWidget *widget;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout_3;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton1;
    QPushButton *pushButton2x2;
    QPushButton *pushButton3x3;
    QPushButton *pushButton6;
    QPushButton *pushButton4x4;
    QPushButton *pushButton12p1;
    QPushButton *pushButton5x5;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *VSCPlayback)
    {
        if (VSCPlayback->objectName().isEmpty())
            VSCPlayback->setObjectName(QStringLiteral("VSCPlayback"));
        VSCPlayback->resize(524, 371);
        QIcon icon;
        icon.addFile(QStringLiteral(":/logo/resources/vscsmall.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        VSCPlayback->setWindowIcon(icon);
        horizontalLayout = new QHBoxLayout(VSCPlayback);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        widget = new QWidget(VSCPlayback);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setAutoFillBackground(true);
        widget->setStyleSheet(QStringLiteral(""));

        verticalLayout_2->addWidget(widget);

        widget_2 = new QWidget(VSCPlayback);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        widget_2->setMinimumSize(QSize(0, 100));
        widget_2->setMaximumSize(QSize(16777215, 100));
        widget_2->setStyleSheet(QStringLiteral("background-color: rgb(42, 42, 42)"));
        verticalLayout_3 = new QVBoxLayout(widget_2);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(widget_2);
        label->setObjectName(QStringLiteral("label"));
        label->setMaximumSize(QSize(16777215, 10));
        label->setStyleSheet(QStringLiteral("background-color: rgb(0, 255, 0)"));

        verticalLayout_3->addWidget(label);

        label_2 = new QLabel(widget_2);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setMaximumSize(QSize(16777215, 10));
        label_2->setStyleSheet(QStringLiteral("background-color: rgb(0, 255, 0)"));

        verticalLayout_3->addWidget(label_2);

        label_3 = new QLabel(widget_2);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setMaximumSize(QSize(16777215, 10));
        label_3->setStyleSheet(QStringLiteral("background-color: rgb(255, 0, 0)"));

        verticalLayout_3->addWidget(label_3);

        label_4 = new QLabel(widget_2);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setMaximumSize(QSize(16777215, 10));
        label_4->setStyleSheet(QStringLiteral("background-color:rgb(255, 255, 0)"));

        verticalLayout_3->addWidget(label_4);


        verticalLayout_2->addWidget(widget_2);


        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        pushButton1 = new QPushButton(VSCPlayback);
        pushButton1->setObjectName(QStringLiteral("pushButton1"));
        pushButton1->setMinimumSize(QSize(25, 25));
        pushButton1->setMaximumSize(QSize(30, 30));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/view/resources/1x1.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton1->setIcon(icon1);
        pushButton1->setIconSize(QSize(25, 25));

        verticalLayout->addWidget(pushButton1);

        pushButton2x2 = new QPushButton(VSCPlayback);
        pushButton2x2->setObjectName(QStringLiteral("pushButton2x2"));
        pushButton2x2->setMinimumSize(QSize(25, 25));
        pushButton2x2->setMaximumSize(QSize(30, 30));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/view/resources/2x2.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton2x2->setIcon(icon2);
        pushButton2x2->setIconSize(QSize(25, 25));

        verticalLayout->addWidget(pushButton2x2);

        pushButton3x3 = new QPushButton(VSCPlayback);
        pushButton3x3->setObjectName(QStringLiteral("pushButton3x3"));
        pushButton3x3->setMinimumSize(QSize(25, 25));
        pushButton3x3->setMaximumSize(QSize(30, 30));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/view/resources/3x3.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton3x3->setIcon(icon3);
        pushButton3x3->setIconSize(QSize(25, 25));

        verticalLayout->addWidget(pushButton3x3);

        pushButton6 = new QPushButton(VSCPlayback);
        pushButton6->setObjectName(QStringLiteral("pushButton6"));
        pushButton6->setMinimumSize(QSize(25, 25));
        pushButton6->setMaximumSize(QSize(30, 30));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/view/resources/6.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton6->setIcon(icon4);
        pushButton6->setIconSize(QSize(25, 25));

        verticalLayout->addWidget(pushButton6);

        pushButton4x4 = new QPushButton(VSCPlayback);
        pushButton4x4->setObjectName(QStringLiteral("pushButton4x4"));
        pushButton4x4->setMinimumSize(QSize(25, 25));
        pushButton4x4->setMaximumSize(QSize(30, 30));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/view/resources/4x4.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton4x4->setIcon(icon5);
        pushButton4x4->setIconSize(QSize(25, 25));

        verticalLayout->addWidget(pushButton4x4);

        pushButton12p1 = new QPushButton(VSCPlayback);
        pushButton12p1->setObjectName(QStringLiteral("pushButton12p1"));
        pushButton12p1->setMaximumSize(QSize(30, 30));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/view/resources/12+1.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton12p1->setIcon(icon6);
        pushButton12p1->setIconSize(QSize(25, 25));

        verticalLayout->addWidget(pushButton12p1);

        pushButton5x5 = new QPushButton(VSCPlayback);
        pushButton5x5->setObjectName(QStringLiteral("pushButton5x5"));
        pushButton5x5->setMaximumSize(QSize(30, 30));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/view/resources/5x5.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton5x5->setIcon(icon7);
        pushButton5x5->setIconSize(QSize(25, 25));

        verticalLayout->addWidget(pushButton5x5);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout->addLayout(verticalLayout);


        retranslateUi(VSCPlayback);

        QMetaObject::connectSlotsByName(VSCPlayback);
    } // setupUi

    void retranslateUi(QWidget *VSCPlayback)
    {
        VSCPlayback->setWindowTitle(QApplication::translate("VSCPlayback", "VSCView", 0));
        label->setText(QString());
        label_2->setText(QString());
        label_3->setText(QString());
        label_4->setText(QString());
        pushButton1->setText(QString());
        pushButton2x2->setText(QString());
        pushButton3x3->setText(QString());
        pushButton6->setText(QString());
        pushButton4x4->setText(QString());
        pushButton12p1->setText(QString());
        pushButton5x5->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class VSCPlayback: public Ui_VSCPlayback {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VSCPLAYBACK_H
