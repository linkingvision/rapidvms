#include "vschddone.h"
#include "hdddevice.hpp"
#include "debug.hpp"


VSCHddOne::VSCHddOne(bool enableChange, s32 id, astring strHdd, s64 nTotal, s64 nLeft, 
	QWidget *parent)
    : QWidget(parent), m_nTotal(nTotal), m_nLeft(nLeft), m_strHdd(strHdd), m_Id(id), 
    m_bEnableChange(enableChange)
{
   s8 DiskTitle[1024];
   m_nNewUsedInM = 0;
   ui.setupUi(this);
   m_nTotalInM = m_nTotal;
   m_nLeftInM = m_nLeft;
   m_nUsedInM = m_nTotalInM - m_nLeftInM;
   ui.labelDiskName->setText(m_strHdd.c_str());
   if (m_nTotal/1024 > 0)
   {
       sprintf(DiskTitle, "%lld G",  m_nTotal /1024);
   }else
   {
       sprintf(DiskTitle, "%lld M",  m_nTotal );
   }
   ui.labelTotal->setText(DiskTitle);
   if (m_nLeft/ 1024 > 0)
   {
       sprintf(DiskTitle, "%lld G",  m_nLeft /1024);
   }else
   {
       sprintf(DiskTitle, "%lld M",  m_nLeft);
   }
   ui.labelFree->setText(DiskTitle);

   //ui.labelUsed->setText("0G");

   ui.horizontalSlider->setMinimum(0); 
   ui.horizontalSlider->setMaximum(m_nTotalInM); 
   ui.horizontalSlider->setValue(m_nUsedInM); 

   connect(ui.horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(setFreeValue(int)));  
  
   connect(ui.checkBox, SIGNAL( stateChanged(int) ), this, SLOT(diskSelected(int)));
}

void VSCHddOne::diskSelected(int value)
{
    if (m_nLeftInM < 1024 * 6)
    {
        ui.checkBox->setChecked(false);
		return;
    }
	if (value != Qt::Checked)
	{
	    return;
	}
		
	emit diskSelectedUpdated(m_Id);
}

void VSCHddOne::diskSetChecked(bool checked)
{
    if (m_nLeftInM < 1024 * 6)
    {
        ui.checkBox->setChecked(false);
    }else
    {
        ui.checkBox->setChecked(checked);
    }
}

void VSCHddOne::setFreeValue(int value)
{
    s8 DiskTitle[1024];
    if (value <= m_nUsedInM  || m_bEnableChange == false)
    {
        ui.horizontalSlider->setValue(m_nUsedInM); 
        //ui.labelUsed->setText("0G");
         if (m_nLeftInM/1024 > 0)
        {
            sprintf(DiskTitle, "%d G",  m_nLeftInM / 1024);
        }else
        {
            sprintf(DiskTitle, "%d M",  m_nLeftInM);
        }
        ui.labelFree->setText(DiskTitle);
    }else
    {
        m_nNewUsedInM = value - m_nUsedInM;
        s32 newLeft = m_nLeftInM - m_nNewUsedInM;
        if (newLeft/1024 > 0)
        {
            sprintf(DiskTitle, "%d G",  newLeft / 1024);
        }else
        {
            sprintf(DiskTitle, "%d M",  newLeft);
        }
        ui.labelFree->setText(DiskTitle);
        if (m_nNewUsedInM/1024 > 0)
        {
            sprintf(DiskTitle, "%d G",  m_nNewUsedInM / 1024);
        }else
        {
            sprintf(DiskTitle, "%d M",  m_nNewUsedInM);
        }
        //ui.labelUsed->setText(DiskTitle);
    }
}



