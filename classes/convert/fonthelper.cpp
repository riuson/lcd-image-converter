#include "fonthelper.h"
//-----------------------------------------------------------------------------
#include <QPixmap>
#include <QPainter>
#include "idatacontainer.h"
//-----------------------------------------------------------------------------
FontHelper::FontHelper(QObject *parent) :
    QObject(parent)
{
}
//-----------------------------------------------------------------------------
QImage FontHelper::drawString(const IDataContainer *data, const QString &value)
{
    int width = 0, height = 0;
    QImage::Format format = QImage::Format_ARGB32;
    // find summary preview image size
    for (int i = 0; i < value.length(); i++)
    {
        QChar ch = value.at(i);
        if (data->keys().contains(ch))
        {
            QImage *image = data->image(ch);

            format = image->format();
            width += image->width();
            height = qMax(height, image->height());
        }
    }

    // create preview image
    QImage preview = QImage(width, height, format);
    QPixmap previewPixmap = QPixmap::fromImage(preview);
    QPainter painter(&previewPixmap);

    // place characters on image
    int x = 0;
    for (int i = 0; i < value.length(); i++)
    {
        QChar ch = value.at(i);
        if (data->keys().contains(ch))
        {
            QImage *image = data->image(ch);
            painter.drawImage(x, 0, *image);

            x += image->width();
        }
    }

    return previewPixmap.toImage();
}
//-----------------------------------------------------------------------------
