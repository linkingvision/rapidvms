#ifndef VSCLINEEDIT_H
#define VSCLINEEDIT_H

#include <QLineEdit>

class VSCLineEdit: public QLineEdit
{
	Q_OBJECT

public:
	VSCLineEdit(QWidget *parent=0);
	~VSCLineEdit(){};

	void setId(int Id)
	{
		m_ModId = Id;
	}
signals:
	void focusIn(int Id);
	

protected:
	virtual void focusInEvent(QFocusEvent *e);
	virtual void focusOutEvent(QFocusEvent *e);

private:
	int m_ModId;

};

typedef std::map<int, VSCLineEdit*> LineEditMap;
#endif //VSCLINEEDIT_H