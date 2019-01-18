#ifndef FOOTITEM_H
#define FOOTITEM_H

#include <QWidget>
#include <stdint.h>

class FootItem : public QWidget
{
    Q_OBJECT
public:
    explicit FootItem(uint32_t segmentCount, float *angles, QWidget *parent = nullptr);

signals:

public slots:
};

#endif // FOOTITEM_H
