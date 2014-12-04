#ifndef VSC_PLAY_CONTROL_H
#define VSC_PLAY_CONTROL_H

#include <QWidget>
#include "ui_vscplaycontrol.h"
#include "utility.hpp"

class VSCPlayControl : public QWidget
{
    Q_OBJECT

public:
    VSCPlayControl(QWidget *parent = 0);
    ~VSCPlayControl(){}
public:
    Ui::VSCPlayControl ui;
    
};

#endif // VSCVIEW_H
