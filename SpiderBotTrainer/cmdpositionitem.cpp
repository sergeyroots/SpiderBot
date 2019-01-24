#include "cmdpositionitem.h"
#include <QHBoxLayout>
#include <QToolButton>
#include <QStyle>

CmdPositionItem::CmdPositionItem(sbmSpiderBotSettings_t *settings, QWidget *parent) :
        QWidget(parent),
        m_readOnly(false),
        m_removable(true),
        m_stepCount(10),
        lName(new QLabel) {
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
    createUI();
}

CmdPositionItem::CmdPositionItem(sbmFootAngles_t *anglesSrc, QWidget *parent) :
        QWidget(parent),
        m_readOnly(false),
        m_removable(true),
        m_stepCount(10),
        lName(new QLabel) {
    uint32_t footCount = anglesSrc->footCount;
    angles.footCount = footCount;
    angles.segmentsCount = static_cast<uint32_t*>(malloc(footCount * sizeof(uint32_t)));
    angles.angles = static_cast<float**>(malloc(footCount * sizeof(float*)));
    uint32_t segmentCount;
    for (uint32_t i=0; i<footCount; ++i) {
        segmentCount = anglesSrc->segmentsCount[i];
        angles.segmentsCount[i] = segmentCount;
        angles.angles[i] = static_cast<float*>(malloc(segmentCount * sizeof(float)));
        for(uint32_t j=0; j<segmentCount; ++j) {
            angles.angles[i][j] = anglesSrc->angles[i][j];
        }
    }
    createUI();
}

bool CmdPositionItem::isReadOnly() const {
    return m_readOnly;
}

bool CmdPositionItem::isRemovable() const {
    return m_removable;
}

void CmdPositionItem::setName(QString name) {
    lName->setText(name);
}

CmdPositionItem *CmdPositionItem::clone() {
    CmdPositionItem *newItem = new CmdPositionItem(&angles);
    newItem->setReadOnly(m_readOnly);
    newItem->setRemovable(m_removable);
    newItem->setName(lName->text());
    newItem->setStepCount(m_stepCount);
    return newItem;
}

uint32_t CmdPositionItem::getStepCount() const {
    return m_stepCount;
}

void CmdPositionItem::createUI() {
    QHBoxLayout *l = new QHBoxLayout();
    l->setMargin(0);
    l->setSpacing(3);
    lName->setText("default position");
    lName->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    bRemove = new QToolButton();
    bRemove->setAutoRaise(true);
    bRemove->setToolTip("Remove");
    bRemove->setIcon(style()->standardIcon(QStyle::SP_TitleBarCloseButton));
    bRemove->setCursor(Qt::PointingHandCursor);
    bRemove->setVisible(m_removable);

    lName->setMinimumHeight(bRemove->sizeHint().height());

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

void CmdPositionItem::setStepCount(uint32_t stepCount) {
    m_stepCount = stepCount;
}
