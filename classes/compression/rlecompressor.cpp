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
#include "rlesequence.h"
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
        QVector<quint32> *output,
        quint32 minimumOfEquals)
{
    output->clear();

    QVector<RleSequence *> sequencesSource;

    this->collectSequences(input, &sequencesSource);

    QVector<RleSequence *> sequencesCombined;
    this->combineSequences(&sequencesSource, minimumOfEquals, &sequencesCombined);

    for (int i = 0; i < sequencesCombined.size(); i++)
    {
        RleSequence *seq = sequencesCombined.at(i);
        this->flushSequence(seq, dataSize, output);
    }
}
//-----------------------------------------------------------------------------
void RleCompressor::collectSequences(
        const QVector<quint32> *input,
        QVector<RleSequence *> *sequences)
{
    RleSequence *temp = new RleSequence();
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
            temp = new RleSequence();
        }

        temp->append(value);
        index++;
    }

    sequences->append(temp);
}
//-----------------------------------------------------------------------------
void RleCompressor::combineSequences(
        const QVector<RleSequence *> *inputSequences,
        quint32 minimumOfEquals,
        QVector<RleSequence *> *outputSequences)
{
    RleSequence *temp = new RleSequence();

    for (int i = 0; i < inputSequences->size(); i++)
    {
        RleSequence *seq = inputSequences->at(i);

        // if number of equals values >= minimal
        if (seq->size() >= minimumOfEquals)
        {
            // save previous
            outputSequences->append(temp);

            // copy and save
            temp = new RleSequence(seq);
            outputSequences->append(temp);

            // create new
            temp = new RleSequence();
        }
        else
        {
            temp->append(seq);
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
        const RleSequence *sequence,
        DataBlockSize dataSize,
        QVector<quint32> *output)
{
    if (sequence->size() > 0)
    {
        quint32 size = this->getMaxSize(dataSize);

        // if all equals
        if (sequence->allEquals())
        {
            //qDebug() << "all equals";
            //qDebug() << " count: " << queue->size();
            //qDebug() <<  " of : " << output->last();
            for (quint32 shift = 0; shift < sequence->size(); shift += size)
            {
                int partSize = qMin(size, sequence->size() - shift);
                this->flushSequencePart(sequence, shift, partSize, output);
            }
        }
        else // if all non-equals
        {
            //qDebug() << "all non-equals";
            //qDebug() << " count: " << -queue->size();
            for (quint32 shift = 0; shift < sequence->size(); shift += size)
            {
                int partSize = qMin(size, sequence->size() - shift);
                this->flushSequencePart(sequence, shift, partSize, output);
            }
        }
    }
}
//-----------------------------------------------------------------------------
void RleCompressor::flushSequencePart(
        const RleSequence *sequence,
        quint32 start,
        quint32 length,
        QVector<quint32> *output)
{
    if (sequence->size() > 1 && sequence->allEquals())
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
