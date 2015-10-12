#ifndef VSCRECORDPLAN_H
#define VSCRECORDPLAN_H

#include <QWidget>
#include "ui_vscrecordplan.h"

class VSCRecordPlan: public QWidget
{
	Q_OBJECT;
public:
	VSCRecordPlan(QWidget *parent = 0);
	~VSCRecordPlan();
	void SetupConnection();
	void updateUi();

public:
	Ui::VSCRecordPlan ui; 

public slots:
	void SaveButtonClick();

	void FocusInLineEdit(int Id);//lineedit被选中
	void FocusOutLineEdit(bool radiostate);//lineedit没有被选中

	void FocusInPlanLine(int LineId);

private:
	void applyPlanTmpl(int IpcIndex);

private:
	QString m_ServerIp;
	LineEditMap m_LineEditMap;
	PlanEditLineMap m_PlanEditLineMap;
	int m_currentTmplId;
	int m_currentPlanLineId;

};

#endif //VSCRECORDPLAN_H