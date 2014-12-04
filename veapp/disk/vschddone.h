#ifndef VSC_HDD_ONE_H
#define VSC_HDD_ONE_H

#include <QDialog>
#include "ui_vschddone.h"
#include "utility.hpp"
#include <QTimer>
#include "hdddevice.hpp"

class VSCHddOne : public QWidget
{
    Q_OBJECT

public:
    VSCHddOne(bool enableChange, s32 id, astring strHdd, s64 nTotal, s64 nLeft,  QWidget *parent = 0);
    ~VSCHddOne(){}
public slots:  
  void setFreeValue(int value);  
  void diskSelected(int value);  
public:
  void diskSetChecked(bool checked);
  BOOL getStrHdd(astring &strHdd)
  {
      strHdd = m_strHdd;
      return TRUE;
  }
  BOOL getNewUsed(s64 & newUsed)
  {
      s64 newUsedInM = m_nNewUsedInM;
      if (m_nNewUsedInM <= 0)
      {
           newUsed = (s64)4 * 1024;
      }else
      {
          newUsed = newUsedInM;
      }
      return TRUE;
  }
  bool isChecked()
  {
  	return ui.checkBox->isChecked();
  }
signals:
    void diskSelectedUpdated(int id);
public:
    Ui::VSCHddOne ui;
private:
    s64 m_nTotal;
    s64 m_nLeft;
    s32 m_nTotalInM;
    s32 m_nLeftInM;
    s32 m_nUsedInM;
    s32 m_nNewUsedInM;
    astring m_strHdd;
    s32 m_Id;
    bool m_bEnableChange;
};

#endif // VSC_HDD_ONE_H
