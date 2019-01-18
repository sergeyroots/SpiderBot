#include "cmdpositionitem.h"
#include <QHBoxLayout>
#include <QToolButton>
#include <QLabel>
#include <QStyle>

CmdPositionItem::CmdPositionItem(sbmSpiderBotSettings_t *settings, QWidget *parent) :
        QWidget(parent),
        m_readOnly(false),
        m_removable(true) {
    uint32_t footCount = settings->footCount;
    angles.footCount = footCount;
    angles.segmentsCount = static_cast<uint32_t*>(malloc(footCount * sizeof(uint32_t)));
    angles.angles = static_cast<float**>(malloc(footCount * sizeof(float*)));
    uint32_t segmentCount;
    for (uint32_t i=0; i<footCount; ++i) {
        segmentCount = settings->foots[i].segmentCount;
        angles.segmentsCount[i] = segmentCount;
        angles.angles[i] = static_cast<float*>(malloc(segmentCount * sizeof(float)));
        for(uint32_t j=0; j<segmentCount; ++j) {
            angles.angles[i][j] = settings->foots[i].segments[j].angleDefault;
        }
    }

    QHBoxLayout *l = new QHBoxLayout();
    l->setMargin(0);
    l->setSpacing(3);
    QLabel *lName = new QLabel();
    lName->setText("default position");
    lName->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    bRemove = new QToolButton();
    bRemove->setAutoRaise(true);
    bRemove->setToolTip("Remove");
    bRemove->setIcon(style()->standardIcon(QStyle::SP_TitleBarCloseButton));
    bRemove->setCursor(Qt::PointingHandCursor);
    bRemove->setVisible(m_removable);

    l->addWidget(lName);
    l->addWidget(bRemove);

    setLayout(l);
}

void CmdPositionItem::setReadOnly(bool readOnly) {
    m_readOnly = readOnly;
}

void CmdPositionItem::setRemovable(bool removable) {
    m_removable = removable;
    bRemove->setVisible(removable);
}

sbmFootAngles_t *CmdPositionItem::getAngles() {
    return &angles;
}
