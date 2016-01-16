#ifndef __VSC_LANGUAGE_H__
#define __VSC_LANGUAGE_H__

#include <QWidget>
#include "ui_vsclanguage.h"
#include "utility.hpp"
#include <QTimer>
#include "client/clientfactory.hpp"


class VSCLanguage : public QWidget
{
	Q_OBJECT

public:
	VSCLanguage(ClientFactory &pFactory, QWidget *parent = 0);
	~VSCLanguage(){}
public:
	void SetupValue();

public slots:   
	void applyConfig();
	
public:
	Ui::VSCLanguage ui;
	
private:
	ClientFactory &m_pFactory;
	VidLanguage m_nLang;
    
};

#endif // __VSC_USER_H__
