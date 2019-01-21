#ifndef FOOTITEM_H
#define FOOTITEM_H

#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>

#include <stdint.h>

class FootItem : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool editVisible READ isEditVisible WRITE setEditVisible)
private:
    uint32_t segmentCount;
    float *angles;
    QHBoxLayout *mainLayout;
    QLabel *lName;
    QLabel *lValues;
    QHBoxLayout *layoutEditor;
    void updateLabelValues(void);
    QLineEdit *createLineEditor(float *val);

    bool m_editVisible;

public:
    explicit FootItem(uint32_t segmentCount, float *angles, QWidget *parent = nullptr);
    void setFootName(QString footName);
    void setFootIndex(int footIndex);
    void setEditVisible(bool visible);
    QSize sizeHint() const;
    bool isEditVisible() const;

protected:
    void mouseDoubleClickEvent(QMouseEvent *);

signals:
    void onFootAngleChanged(uint32_t segmentCount, float *angles);

public slots:
};

#endif // FOOTITEM_H
