#ifndef VSC_SITE_ADD_H
#define VSC_SITE_ADD_H

#include <QWidget>
#include "ui_vscsiteadd.h"
#include "utility.hpp"
#include "factory.hpp"
#include "vscvwidget.h"

class VSCSiteAdd : public QWidget
{
    Q_OBJECT

public:
    VSCSiteAdd(VSCVmsDataItem &Param, QWidget *parent);
    ~VSCSiteAdd();
public:
    void mouseDoubleClickEvent(QMouseEvent *e);
    void createContentMenu();
    void setupDefaultValue();
    void updateParamValue(QLineEdit *ld, s8 * pParam);
	
private slots:

    void radioButtonClicked();
    void applyConfig();
signals:
    void RecorderTreeUpdated();

public:
    void SetupConnections();

	
public:
    Ui::VSCSiteAdd ui;
private:
    QWidget *m_pParent;
    VSCVmsDataItem m_Param;
    u32 m_nId;
};

#endif // VSC_SITE_ADD_H
