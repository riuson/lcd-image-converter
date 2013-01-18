#include "statusmanager.h"

#include <QStatusBar>
#include "revisionlabel.h"
//-----------------------------------------------------------------------------
StatusManager::StatusManager(QStatusBar *statusBar, QObject *parent) :
    QObject(parent)
{
    this->mBar = statusBar;

    this->addItem(StatusData::MouseCoordinates);
    this->addItem(StatusData::ImageSize);
    this->addItem(StatusData::Scale);
    this->addItem(StatusData::ImageIndex);

    this->mBar->addWidget(new RevisionLabel(this->mBar));
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
            this->updateItem(key, statuses);
        }
    }

    // hide
    QList<StatusData::StatusType> existingKeys = this->mList.keys();
    for (int i = 0; i < existingKeys.length(); i++)
    {
        StatusData::StatusType existingKey = existingKeys.at(i);
        if (!keys.contains(existingKey))
        {
            this->hideItem(existingKey);
        }
    }
}
//-----------------------------------------------------------------------------
void StatusManager::hideAll()
{
    QList<StatusData::StatusType> keys = this->mList.keys();
    for (int i = 0; i < keys.length(); i++)
    {
        StatusData::StatusType key = keys.at(i);
        this->hideItem(key);
    }
}
//-----------------------------------------------------------------------------
void StatusManager::updateItem(
        StatusData::StatusType key,
        const StatusData *statuses)
{
    QLabel *label = this->mList.value(key);
    label->setVisible(true);

    switch (key)
    {
    case StatusData::ImageIndex:
    {
        int current = statuses->data(key).toInt();
        int total = statuses->data(StatusData::ImagesCount).toInt();
        QString message = tr("Char: %1/%2").arg(current + 1).arg(total);
        label->setText(message);
        break;
    }
    case StatusData::MouseCoordinates:
    {
        QPoint point = statuses->data(key).toPoint();
        QString message = tr("Cursor: %1,%2").arg(point.x()).arg(point.y());
        label->setText(message);
        break;
    }
    case StatusData::Scale:
    {
        int scale = statuses->data(key).toInt();
        QString message = tr("Scale: %1x").arg(scale);
        label->setText(message);
        break;
    }
    case StatusData::ImageSize:
    {
        QSize imageSize = statuses->data(key).toSize();
        QString message = tr("Size: %1x%2").arg(imageSize.width()).arg(imageSize.height());
        label->setText(message);
        break;
    }
    default:
    {
        label->setText(statuses->data(key).toString());
        break;
    }
    }
}
//-----------------------------------------------------------------------------
void StatusManager::addItem(StatusData::StatusType key)
{
    QLabel *label = new QLabel(this->mBar);
    this->mList.insert(key, label);
    this->mBar->addPermanentWidget(label);
    label->setVisible(false);
    label->setMinimumWidth(50);
}
//-----------------------------------------------------------------------------
void StatusManager::hideItem(StatusData::StatusType key)
{
    if (this->mList.contains(key))
    {
        QLabel *label = this->mList.value(key);
        label->setVisible(false);
    }
}
//-----------------------------------------------------------------------------
