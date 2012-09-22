#include "rlecompressor.h"
//-----------------------------------------------------------------------------
#include <QVector>
#include <QQueue>
//#include <QDebug>
//-----------------------------------------------------------------------------
RleCompressor::RleCompressor(QObject *parent) :
    QObject(parent)
{
}
//-----------------------------------------------------------------------------
void RleCompressor::compress(
        QVector<quint32> *input,
        DataBlockSize dataSize,
        QVector<quint32> *output)
{
    output->clear();

    quint32 size;
    switch (dataSize)
    {
    case Data8:
        size = 0x7f;
        break;
    case Data16:
        size = 0x7fff;
        break;
    case Data24:
        size = 0x7fffff;
        break;
    case Data32:
        size = 0x7fffffff;
    default:
        size = 127;
        break;
    }

    quint32 i = 1;
    QQueue<quint32> queue;
    queue.append(input->at(0));

    while (i < (quint32)input->size())
    {
        quint32 value = input->at(i);

        if (!queue.isEmpty())
        {
            //qDebug() << "new value: " << value;

            if (queue.size() > 1)
            {
                // if new value not equals to previous
                if (this->allEquals(&queue) && queue.last() != value)
                {
                    //qDebug() << "new value not equals to previous";
                    output->append(queue.size());
                    //qDebug() << " count: " << queue.size();
                    *output << queue.dequeue();
                    //qDebug() <<  " of : " << output->last();
                    queue.clear();
                }
                else
                {
                    // if new value equals to last in non-equals queue
                    if (!this->allEquals(&queue) && queue.last() == value)
                    {
                        //qDebug() << "new value equals to last in non-equals queue";
                        queue.takeLast();
                        output->append(-queue.size()); // minus
                        //qDebug() << " count: " << -queue.size();
                        while (!queue.isEmpty())
                        {
                            *output << queue.dequeue();
                            //qDebug() <<  " of : " << output->last();
                        }
                        queue << value;
                    }
                }
            }
        }

        if ((quint32)queue.size() >= size)
            this->flush(output, &queue);

        queue << value;
        //qDebug() << "queue new value: " << value;

        i++;
    }

    this->flush(output, &queue);
}
//-----------------------------------------------------------------------------
void RleCompressor::flush(QVector<quint32> *output, QQueue<quint32> *queue)
{
    if (!queue->isEmpty())
    {
        // if all equals
        if (this->allEquals(queue))
        {
            //qDebug() << "all equals";
            output->append(queue->size());
            //qDebug() << " count: " << queue->size();
            *output << queue->dequeue();
            //qDebug() <<  " of : " << output->last();
            queue->clear();
        }
        else
        {
            // if all non-equals
            if (!this->allEquals(queue))
            {
                //qDebug() << "all non-equals";
                output->append(-queue->size()); // minus
                //qDebug() << " count: " << -queue->size();
                while (!queue->isEmpty())
                {
                    *output << queue->dequeue();
                    //qDebug() <<  " of : " << output->last();
                }
            }
        }
    }
}
//-----------------------------------------------------------------------------
bool RleCompressor::allEquals(QQueue<quint32> *queue)
{
    bool result = true;
    quint32 first = queue->at(0);
    for (int i = 1; i < queue->size(); i++)
    {
        if (queue->at(i) != first)
            result = false;
    }
    return result;
}
//-----------------------------------------------------------------------------
