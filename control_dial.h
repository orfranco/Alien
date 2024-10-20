#ifndef CONTROL_DIAL_H
#define CONTROL_DIAL_H

#include <QWidget>
#include <QDial>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>

class ControlDial : public QWidget
{
    Q_OBJECT

public:
    explicit ControlDial(const QString& label, QWidget* parent = nullptr);

    void setDialValue(int value);
    bool isSendEnabled() const;
    void toggleSendStateWithParam(bool isOn);
    QPushButton* getSendOnlyButton() const { return m_sendOnlyButton; }


signals:
    void sendMidiMessage(int value, int ccChannel);

private slots:
    void toggleSendState();

private:
    QDial* m_dial;
    QLabel* m_label;
    QCheckBox* m_sendToggle;
    QPushButton* m_sendOnlyButton;

    static int nextControlChangeNumber;

    int m_midiCC;
    bool m_sendEnabled;

    void setToggleStyle();
};

#endif // CONTROL_DIAL_H
