

#ifndef VSCDOCKWIDGET_H
#define VSCDOCKWIDGET_H

#include <QDockWidget>

class VSCDockWidget : public QDockWidget
{
    Q_OBJECT
public:
    QSize sizeHint()
    {
        return QSize(300, 300);
    }


public:
    VSCDockWidget(const QString &title, QWidget *parent = 0, Qt::WindowFlags flags = 0);
    ~VSCDockWidget();

private:
    
};

#endif // VSCDOCKWIDGET_H
