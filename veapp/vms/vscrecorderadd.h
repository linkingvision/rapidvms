#ifndef VSC_RECORDER_ADD_H
#define VSC_RECORDER_ADD_H

#include <QWidget>
#include "ui_vscrecorderadd.h"
#include "utility.hpp"
#include "factory.hpp"
#include "vscvwidget.h"

class VSCRecorderAdd : public QWidget
{
    Q_OBJECT

public:
    VSCRecorderAdd(DeviceParam &Param, QWidget *parent);
    ~VSCRecorderAdd();
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
    Ui::VSCRecorderAdd ui;
private:
    QWidget *m_pParent;
    DeviceParam m_Param;
    u32 m_nId;
};

#endif // VSC_RECORDER_ADD_H
