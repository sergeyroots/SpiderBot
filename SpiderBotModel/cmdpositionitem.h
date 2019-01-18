#ifndef CMDPOSITIONITEM_H
#define CMDPOSITIONITEM_H

#include <QWidget>
#include <QToolButton>

#include "sbmcommandgenerator.h"
#include "sbmsettings.h"

class CmdPositionItem : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(bool readOnly READ isReadOnly WRITE setReadOnly)
    Q_PROPERTY(bool removable READ isRemovable WRITE setRemovable)
public:
    explicit CmdPositionItem(sbmSpiderBotSettings_t *settings, QWidget *parent = nullptr);

    bool isReadOnly() const {
        return m_readOnly;
    }

    bool isRemovable() const {
        return m_removable;
    }

private:
    bool m_readOnly;
    bool m_removable;
    QToolButton *bRemove;
    sbmFootAngles_t angles;

signals:

public slots:
    void setReadOnly(bool readOnly);
    void setRemovable(bool removable);
    sbmFootAngles_t *getAngles(void);
};

#endif // CMDPOSITIONITEM_H
