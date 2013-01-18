#include "statusmanager.h"

#include <QStatusBar>
#include "revisionlabel.h"
//-----------------------------------------------------------------------------
StatusManager::StatusManager(QStatusBar *statusBar, QObject *parent) :
    QObject(parent)
{
    this->mBar = statusBar;

    this->mBar->addPermanentWidget(new RevisionLabel(this->mBar));
}
//-----------------------------------------------------------------------------
StatusManager::~StatusManager()
{
}
//-----------------------------------------------------------------------------
void StatusManager::updateData(const StatusData *statuses)
{
    // update
    QList<StatusData::StatusType> keys = statuses->keys();
    for (int i = 0; i < keys.length(); i++)
    {
        StatusData::StatusType key = keys.at(i);

        // update existing
        if (this->mList.contains(key))
        {
            QLabel *label = this->mList.value(key);

            this->updateItem(key, label, statuses);
        }
        else
        {
            QLabel *label = new QLabel(this->mBar);
            this->mBar->addWidget(label);
            this->mList.insert(key, label);

            this->updateItem(key, label, statuses);
        }
    }

    // remove
    QList<StatusData::StatusType> existingKeys = this->mList.keys();
    for (int i = existingKeys.length() - 1; i >= 0; i--)
    {
        StatusData::StatusType existingKey = existingKeys.at(i);
        if (!keys.contains(existingKey))
        {
            QLabel *label = this->mList.value(existingKey);

            this->mList.remove(existingKey);

            this->mBar->removeWidget(label);
            delete label;
        }
    }
}
//-----------------------------------------------------------------------------
void StatusManager::updateItem(
        StatusData::StatusType type,
        QLabel *label,
        const StatusData *statuses)
{
    switch (type)
    {
    case StatusData::ImageIndex:
    {
        QList<QVariant> list = statuses->data(type).toList();
        int current = list.at(0).toInt();
        int total = list.at(1).toInt();
        QString message = tr("Image: %1/%2").arg(current + 1).arg(total);
        label->setText(message);
        break;
    }
    case StatusData::MouseCoordinates:
    {
        QPoint point = statuses->data(type).toPoint();
        QString message = tr("x: %1, y: %2").arg(point.x()).arg(point.y());
        label->setText(message);
        break;
    }
    case StatusData::Scale:
    {
        int scale = statuses->data(type).toInt();
        QString message = tr("%1x").arg(scale);
        label->setText(message);
        break;
    }
    default:
    {
        label->setText(statuses->data(type).toString());
        break;
    }
    }
}
//-----------------------------------------------------------------------------
