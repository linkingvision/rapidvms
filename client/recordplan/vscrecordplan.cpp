#include "vscrecordplan.h"
#include "vsclineedit.h"
#include "vscplaneditline.h"
#include <QPainter>
#include <QTimer>
#include <QMessageBox>
#include <QEventLoop>
#include <QIcon>

VSCRecordPlan::VSCRecordPlan(QWidget *parent /* = 0 */)
	:QWidget(parent), m_currentTmplId(-1), m_currentPlanLineId(-1)
{
	ui.setupUi(this);

	updateUi();
	SetupConnection();

}

VSCRecordPlan::~VSCRecordPlan()
{

}

void VSCRecordPlan::updateUi()
{
	int i;

	ui.ModComboBox->clear();
	ui.IPCComboBox->clear();

	m_LineEditMap.insert( std::map<int, VSCLineEdit*>::value_type(0, ui.lineEdit) );
	m_LineEditMap.insert( std::map<int, VSCLineEdit*>::value_type(1, ui.lineEdit_2) );
	m_LineEditMap.insert(std::map<int, VSCLineEdit*>::value_type(2, ui.lineEdit_3));
	m_LineEditMap.insert(std::map<int, VSCLineEdit*>::value_type(3, ui.lineEdit_4));
	m_LineEditMap.insert(std::map<int, VSCLineEdit*>::value_type(4, ui.lineEdit_5));
	m_LineEditMap.insert(std::map<int, VSCLineEdit*>::value_type(5, ui.lineEdit_6));

	for (i=0; i<6; i++)
	{
		m_LineEditMap[i]->setId(i+3);
	}

	m_PlanEditLineMap.insert( std::map<int, VSCPlanEditLine*>::value_type(0, ui.Monwidget) );
	m_PlanEditLineMap.insert( std::map<int, VSCPlanEditLine*>::value_type(1, ui.Tuewidget) );
	m_PlanEditLineMap.insert( std::map<int, VSCPlanEditLine*>::value_type(2, ui.Wedwidget) );
	m_PlanEditLineMap.insert( std::map<int, VSCPlanEditLine*>::value_type(3, ui.Thuwidget) );
	m_PlanEditLineMap.insert( std::map<int, VSCPlanEditLine*>::value_type(4, ui.Friwidget) );
	m_PlanEditLineMap.insert( std::map<int, VSCPlanEditLine*>::value_type(5, ui.Satwidget) );
	m_PlanEditLineMap.insert( std::map<int, VSCPlanEditLine*>::value_type(6, ui.Sunwidget) );

	for (i=0; i<7; i++)
	{
		m_PlanEditLineMap[i]->SetId(i);
	}


}

void VSCRecordPlan::SetupConnection()
{

	int i,j;
	for (i = 0; i<7; i++)
	{
		/*显示全日模版*/
		connect(ui.radioButton, SIGNAL(toggled(bool)), m_PlanEditLineMap[i], SLOT(SetAlldayMod(bool)));
		/*显示工作时间模版*/
		connect(ui.radioButton_2, SIGNAL(toggled(bool)), m_PlanEditLineMap[i], SLOT(SetWeekDayMod(bool)));
		/*记录现在被编辑的计划条编号*/
		connect(m_PlanEditLineMap[i], SIGNAL(focusIn(int)), this, SLOT(FocusInPlanLine(int)));
	}

	connect(ui.radioButton, SIGNAL(toggled(bool)), this, SLOT(FocusOutLineEdit(bool)));
	connect(ui.radioButton_2, SIGNAL(toggled(bool)), this, SLOT(FocusOutLineEdit(bool)));

	/*自定义模版文本编辑条被按下时初始化录像计划编辑条*/
	for (i=0; i<6; i++)
	{
		connect(m_LineEditMap[i], SIGNAL(focusIn(int)), this, SLOT(FocusInLineEdit(int)));

		for (int j=0; j<7; j++)
		{	
			//TODO load the plan
			connect(m_LineEditMap[i], SIGNAL(focusIn(int)), m_PlanEditLineMap[j], SLOT(InitPlanLine(int)));
		}
	}

	/*保存按钮被按下*/
	connect( ui.SaveButton, SIGNAL(clicked()), this, SLOT(SaveButtonClick()) );

	connect(ui.OkPb, SIGNAL(clicked()), this, SLOT(OkButtonClick()));
}

void VSCRecordPlan::FocusInLineEdit(int Id)
{
	m_currentTmplId = Id;

}

void VSCRecordPlan::FocusOutLineEdit(bool radiostate)
{
	if (radiostate)
	{
		m_currentTmplId = -1;
	}
}

void VSCRecordPlan::FocusInPlanLine(int LineId)
{
	if (m_currentTmplId >= 0)
	{
		m_currentPlanLineId = LineId;
	}
}

void VSCRecordPlan::SaveButtonClick()//rest
{
	int i,j,k;
	if (m_currentTmplId > 0)
	{
	}

	return;
}


void VSCRecordPlan::applyPlanTmpl(int IpcIndex)
{
	int templateID = ui.ModComboBox->currentIndex() + 1;
	int i = IpcIndex;
	
}


