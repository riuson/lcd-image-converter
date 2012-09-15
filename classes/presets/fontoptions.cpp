#include "fontoptions.h"
//-----------------------------------------------------------------------------
#include <QStringList>
//-----------------------------------------------------------------------------
FontOptions::FontOptions(QObject *parent) :
    QObject(parent)
{
    this->mBom = false;
    this->mEncoding = FontOptions::encodings().at(0);
}
//-----------------------------------------------------------------------------
bool FontOptions::bom() const
{
    return this->mBom;
}
//-----------------------------------------------------------------------------
const QString &FontOptions::encoding() const
{
    return this->mEncoding;
}
//-----------------------------------------------------------------------------
void FontOptions::setBom(bool value)
{
    this->mBom = value;

    emit this->changed();
}
//-----------------------------------------------------------------------------
void FontOptions::setEncoding(const QString &value)
{
    if (FontOptions::encodings().contains(value))
    {
        this->mEncoding = value;

        emit this->changed();
    }
}
//-----------------------------------------------------------------------------
const QStringList &FontOptions::encodings()
{
    static const QStringList result =
            QStringList() << "UTF-8"
                          << "UTF-16"
                          << "UTF-16BE"
                          << "UTF-16LE"
                          << "UTF-32"
                          << "UTF-32BE"
                          << "UTF-32LE"
                          << "Windows-1250"
                          << "Windows-1251"
                          << "Windows-1252"
                          << "Windows-1253"
                          << "Windows-1254"
                          << "Windows-1255"
                          << "Windows-1256"
                          << "Windows-1257"
                          << "Windows-1258";

    return result;
}
//-----------------------------------------------------------------------------
