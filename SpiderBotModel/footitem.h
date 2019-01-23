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
    Q_PROPERTY(bool readOnly READ isReadOnly WRITE setReadOnly)

private:
    uint32_t segmentCount;
    float *angles;
    bool m_editVisible;
    bool m_readOnly;
    QHBoxLayout *mainLayout;
    QLabel *lName;
    QLabel *lValues;
    QHBoxLayout *layoutEditor;
    void updateLabelValues(void);
    QLineEdit *createLineEditor(float *val);

public:
    explicit FootItem(uint32_t segmentCount, float *angles, QWidget *parent = nullptr);
    QSize sizeHint() const;
    void setFootName(QString footName);
    void setFootIndex(int footIndex);
    void setEditVisible(bool visible);
    bool isEditVisible() const;    
    bool isReadOnly() const;

protected:
    void mouseDoubleClickEvent(QMouseEvent *);
    void keyPressEvent(QKeyEvent *event);

signals:
    void onFootAngleChanged(uint32_t segmentCount, float *angles);

public slots:
    void setReadOnly(bool readOnly);
};

#endif // FOOTITEM_H
