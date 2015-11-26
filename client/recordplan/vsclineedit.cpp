#include "vsclineedit.h"
#include <QPalette>

VSCLineEdit::VSCLineEdit(QWidget *parent/* =0 */)
	:QLineEdit(parent)
{

}

void VSCLineEdit::focusInEvent(QFocusEvent *e)
{
	QPalette p1=QPalette();
	p1.setColor(QPalette::Base,Qt::blue);
	setPalette(p1);
	setReadOnly(false);
	setFocus();

	emit focusIn(m_ModId);
}

void VSCLineEdit::focusOutEvent(QFocusEvent *e)
{
	QPalette p1=QPalette();
	p1.setColor(QPalette::Base,Qt::white);
	setPalette(p1);
	setReadOnly(true);
	QString str = text();
	setText(str);
}

