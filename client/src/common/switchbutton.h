#ifndef SWITCHBUTTON_H
#define SWITCHBUTTON_H

#include <QPushButton>

/**
 * This is a button which can be switched on and off.
 * It looks similar as the on/off switches of Android.
 * <p>
 * The method isChecked() return the on/off state.
 */

class SwitchButton : public QPushButton {
    Q_OBJECT

public:
    /** Constructor */
    explicit SwitchButton(QWidget* parent = 0);

    /** Set the color for on and off status. */
    void setColors(const QColor on=Qt::darkGreen, const QColor off=Qt::gray);

    /** Set the labels for on and off status. */
    void setLabels(const QString on=QString(tr("ON")), const QString off=QString(tr("OFF")));

protected:

    void paintEvent(QPaintEvent* paint);

    /** Color for on state */
    QColor onColor;

    /** Color for off state */
    QColor offColor;

    /** Label for on state */
    QString onLabel;

    /** Label for off state */
    QString offLabel;

    /** Color of the label in on state */
    QColor onLabelColor;

    /** Color of the label in off state */
    QColor offLabelColor;

};

#endif // SWITCHBUTTON_H

























