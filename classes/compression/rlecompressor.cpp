/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2012 riuson
 * mailto: riuson@gmail.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/
 */

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

    QQueue<Sequence *> sequencesSource;

    this->collectSequences(input, &sequencesSource);

    QQueue<Sequence *> sequencesCombined;
    this->combineSequences(&sequencesSource, 2, &sequencesCombined);

    for (int i = 0; i < sequencesCombined.size(); i++)
    {
        Sequence *seq = sequencesCombined.at(i);
        this->flushSequence(seq, dataSize, output);
    }
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
bool RleCompressor::allEquals(const QQueue<quint32> *queue)
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
void RleCompressor::collectSequences(
        const QVector<quint32> *input,
        QQueue<Sequence *> *sequences)
{
    Sequence *temp = new Sequence();
    temp->append(input->at(0));
    quint32 index = 1;

    while (index < (quint32)input->size())
    {
        quint32 value = input->at(index);

        // if new value not equals to last sequence
        if (temp->last() != value)
        {
            // save last sequence
            sequences->append(temp);
            // and create new
            temp = new Sequence();
        }

        temp->append(value);
        index++;
    }

    sequences->append(temp);
}
//-----------------------------------------------------------------------------
void RleCompressor::combineSequences(
        const QQueue<Sequence *> *inputSequences,
        quint32 minimalEqualsLength,
        QQueue<Sequence *> *outputSequences)
{
    Sequence *temp = new Sequence();

    for (int i = 0; i < inputSequences->size(); i++)
    {
        Sequence *seq = inputSequences->at(i);

        // if number of equals values >= minimal
        if ((quint32)seq->size() >= minimalEqualsLength)
        {
            // save previous
            outputSequences->append(temp);

            // copy
            temp = new Sequence();
            for (int j = 0; j < seq->size(); j++)
            {
                temp->append(seq->at(j));
            }
            // save
            outputSequences->append(temp);

            // create new
            temp = new Sequence();
        }
        else
        {
            for (int j = 0; j < seq->size(); j++)
            {
                temp->append(seq->at(j));
            }
        }
    }

    outputSequences->append(temp);
}
//-----------------------------------------------------------------------------
quint32 RleCompressor::getMaxSize(DataBlockSize dataSize)
{
    quint32 result;

    switch (dataSize)
    {
    case Data8:
        result = 0x7f;
        break;
    case Data16:
        result = 0x7fff;
        break;
    case Data24:
        result = 0x7fffff;
        break;
    case Data32:
        result = 0x7fffffff;
    default:
        result = 127;
        break;
    }

    return result;
}
//-----------------------------------------------------------------------------
void RleCompressor::flushSequence(
        const Sequence *sequence,
        DataBlockSize dataSize,
        QVector<quint32> *output)
{
    if (!sequence->isEmpty())
    {
        quint32 size = this->getMaxSize(dataSize);

        // if all equals
        if (this->allEquals(sequence))
        {
            //qDebug() << "all equals";
            //qDebug() << " count: " << queue->size();
            //qDebug() <<  " of : " << output->last();
            for (quint32 shift = 0; shift < (quint32)sequence->length(); shift += size)
            {
                int partSize = qMin(size, (quint32)(sequence->length() - shift));
                this->flushSequencePart(sequence, shift, partSize, true, output);
            }
        }
        else // if all non-equals
        {
            //qDebug() << "all non-equals";
            //qDebug() << " count: " << -queue->size();
            for (quint32 shift = 0; shift < (quint32)sequence->length(); shift += size)
            {
                int partSize = qMin(size, (quint32)(sequence->length() - shift));
                this->flushSequencePart(sequence, shift, partSize, false, output);
            }
        }
    }
}
//-----------------------------------------------------------------------------
void RleCompressor::flushSequencePart(
        const Sequence *sequence,
        quint32 start,
        quint32 length,
        bool allEquals,
        QVector<quint32> *output)
{
    if (allEquals)
    {
        output->append(length);
        output->append(sequence->at(start));
    }
    else
    {
        output->append(-length);

        for (quint32 i = 0; i < length; i++)
        {
            output->append(sequence->at(start + i));
        }
    }
}
//-----------------------------------------------------------------------------
