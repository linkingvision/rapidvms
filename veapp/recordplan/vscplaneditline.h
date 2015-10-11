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

	void* GetPlan();
	void InitTimeofPlan(int pos);//初始化TimeofPlan链表

signals:
	void focusIn(int LineId);

public slots:
	void on_Clear();
	void SetAlldayMod(bool fal);//把编辑条设为全日模式
	void SetWeekDayMod(bool fal);//把编辑条设为工作日模式
	void InitPlanLine(int ModId);//初始化该编辑条
	//void RecieveModInfo(QList<zexabox::PlanMod>* PlanModlist);
	void UndoLastStep(int ModId);

public:
	Ui::VSCPlanEditLine ui;

protected:
	virtual void paintEvent(QPaintEvent *);
	virtual void mousePressEvent(QMouseEvent *);
	virtual void mouseReleaseEvent(QMouseEvent *);
	virtual void mouseMoveEvent(QMouseEvent *);

	void contextMenuEvent(QContextMenuEvent* e) ;

private:
	int m_Width;
	int m_Height;
	bool moving;

	int mouseX_0;//画绿条的起始点
	int mouseX_1;//画绿条的终止点

	int begin_mouseX;
	int movingX;

	bool m_Editable;//控件是否可以编辑录像时间
	int m_LineId;

	QList<int>* TimeofPlan;//记录此次录像时间节点
	//QList<zexabox::PlanMod>* m_PlanModList;
	int m_Tnum;//TimeofPlan在最初的长度

	bool EraserEnable;


	int m_planList[VSC_PLAN_UNIT_LEN];
	int m_startMoving;

	QAction *m_pClear;
	QMenu * popupMenu;
};

typedef std::map<int, VSCPlanEditLine*> PlanEditLineMap;
#endif //VSC_PLAN_EDITLINE_H
