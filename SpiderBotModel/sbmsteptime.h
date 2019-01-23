#ifndef SBMSTEPTIME_H
#define SBMSTEPTIME_H

#include <QWidget>
#include <QToolButton>
#include <QDoubleSpinBox>

class SbmStepTime : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(uint32_t timeInSteps READ getTimeInSteps WRITE setTimeInSteps)
    Q_PROPERTY(double timeInterval READ getTimeInterval WRITE setTimeInterval)

private:
    uint32_t m_timeInSteps;
    double m_timeInterval;
    QDoubleSpinBox *sbVal;
    QToolButton *bUnits;

public:
    explicit SbmStepTime(double timeInterval, uint32_t timeInSteps = 10, QWidget *parent = nullptr);
    uint32_t getTimeInSteps() const;
    double getTimeInterval() const;

signals:
    void onChangedValue(uint32_t timeInSteps, double timeInterval);

private slots:
    void on_bUnits_clicked(bool checked);
    void on_sbVal_editingFinished();
    void on_sbVal_valueChanged(double val);

public slots:
    void setTimeInSteps(uint32_t timeInSteps);
    void setTimeInterval(double timeInterval);
};

#endif // SBMSTEPTIME_H
