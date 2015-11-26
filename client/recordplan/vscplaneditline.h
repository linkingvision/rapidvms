#ifndef VSC_PLAN_EDITLINE_H
#define VSC_PLAN_EDITLINE_H

#include <QWidget>
#include <QAction>
#include <QAction>
#include <QDesktopWidget>
#include <QApplication>
#include <QMenu>
#include "ui_vscplaneditline.h"
#include "factory.hpp"


#define VSC_PLAN_UNIT_LEN 24 /* every 30min is a unit  */
/*Record plan line item*/
class VSCPlanEditLine: public QWidget
{
	Q_OBJECT
public:
	VSCPlanEditLine(QWidget *parent = 0);
	~VSCPlanEditLine();
	void ShowPlan();

	void SetId(int LineId)
	{
		m_LineId = LineId;
	}

signals:
	void focusIn(int LineId);

public slots:
	void on_Clear();
	void SetAlldayMod(bool fal);//把编辑条设为全日模式
	void SetWeekDayMod(bool fal);//把编辑条设为工作日模式

public:
	Ui::VSCPlanEditLine ui;

protected:
	virtual void paintEvent(QPaintEvent *);
	virtual void mousePressEvent(QMouseEvent *);
	virtual void mouseReleaseEvent(QMouseEvent *);
	virtual void mouseMoveEvent(QMouseEvent *);

	void contextMenuEvent(QContextMenuEvent* e) ;

private:
	bool m_Editable;
	int m_planList[VSC_PLAN_UNIT_LEN];
	int m_startMoving;
	int m_LineId;

	QAction *m_pClear;
	QMenu * popupMenu;
};

typedef std::map<int, VSCPlanEditLine*> PlanEditLineMap;
#endif //VSC_PLAN_EDITLINE_H
