#ifndef __VSC_LANGUAGE_H__
#define __VSC_USER_H__

#include <QWidget>
#include "ui_vsclanguage.h"
#include "utility.hpp"
#include <QTimer>
#include "factory.hpp"


class VSCLanguage : public QWidget
{
	Q_OBJECT

public:
	VSCLanguage(QWidget *parent = 0);
	~VSCLanguage(){}
public:
	void SetupValue();

public slots:   
	void applyConfig();
	
public:
	Ui::VSCLanguage ui;
	
private:
	VSCLangType m_lang;
    
};

#endif // __VSC_USER_H__
