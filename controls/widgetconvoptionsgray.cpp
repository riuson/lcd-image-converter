#include "widgetconvoptionsgray.h"
#include "ui_widgetconvoptionsgray.h"
//-----------------------------------------------------------------------------
#include <QButtonGroup>

#include "bytelistitemdelegate.h"
#include "convertergrayscale.h"
//-----------------------------------------------------------------------------
WidgetConvOptionsGray::WidgetConvOptionsGray(IConverter *options, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetConvOptionsGray)
{
    ui->setupUi(this);

    this->mReady = false;

    this->mDelegate = new ByteListItemDelegate(this);
    this->ui->listWidget->setItemDelegate(this->mDelegate);

    this->mConv = dynamic_cast<ConverterGrayscale*>(options);
    if (this->mConv == NULL)
    {
        this->mConv = new ConverterGrayscale(this);
        this->mConv->loadSettings();
    }

    int bitsPerPoint = this->mConv->depth();

    this->ui->spinBoxBitsPerPoint->setValue(bitsPerPoint);

    this->mReady = true;
    this->updatePreview();
}
//-----------------------------------------------------------------------------
WidgetConvOptionsGray::~WidgetConvOptionsGray()
{
    delete ui;
}
//-----------------------------------------------------------------------------
void WidgetConvOptionsGray::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
//-----------------------------------------------------------------------------
void WidgetConvOptionsGray::updatePreview()
{
    if (!this->mReady)
        return;

    this->ui->listWidget->clear();
    QStringList list;

    ConverterGrayscale::DataLength length = this->mConv->length();
    int bits = (int)length;
    this->mDelegate->setBitsCount(bits);

    // 3 - 2 bits
    // 7 - 3 bits
    // 15 - 4 bits
    // 31 - 5 bits
    // 63 - 6 bits
    // 127 - 7 bits
    // 255 - 8 bits

    int bitsPerPoint = this->ui->spinBoxBitsPerPoint->value();

    QStringList colors;
    for (int i = bitsPerPoint - 1; i >= 0; i--)
        colors << QString("b.%1").arg(i);

    bool swapBytes= this->mConv->swapBytes();
    bool mirror = this->mConv->mirror();
    bool pack = this->mConv->pack();

    for (int i = 0, j = 0, k = 0; i < 80; i++)
    {
        QString a = colors.at(k++);
        list.append(a.replace(".", QString("%1.").arg(j)));
        if (k == colors.length())// end of point (color bits)
        {
            j++;
            k = 0;
            if (!pack)
            {
                if ((i % bits) + colors.length() > bits - 1)
                {
                    while (i % bits != bits - 1)
                    {
                        list.append("0");
                        i++;
                    }
                }
            }
        }
    }

    if (swapBytes)
    {
        QStringList outlist;
        for (int i = 0; i < list.count(); i+= bits)
        {
            for (int j = (bits / 8); j > 0; j--)//1, 2 or 4
            {
                int start = i + (j - 1) * 8;
                if (start >= 0 && (start + 8 - 1) <= list.count() )
                outlist.append(list.mid(start, 8));
            }
        }
        list = outlist;
    }

    if (mirror)
    {
        QStringList outlist;
        for (int i = 0; i < list.count(); i+= bits)
        {
            QStringList tmp = list.mid(i, bits);
            for (int j = tmp.count() - 1; j >= 0; j--)
            {
                outlist.append(tmp.at(j));
            }
        }
        list = outlist;
    }
    this->ui->listWidget->addItems(list);

    this->mConv->setDepth(bitsPerPoint);

    this->mConv->saveSettings();

    emit this->settingsChanged();
}
//-----------------------------------------------------------------------------
void WidgetConvOptionsGray::dataLengthChanged(int value)
{
    switch (value)
    {
    case 8:
    case 16:
    case 32:
        this->mConv->setLength((IConverter::DataLength)value);
        break;
    default:
        this->mConv->setLength(IConverter::Data8);
        break;
    }
}
//-----------------------------------------------------------------------------
void WidgetConvOptionsGray::dataPackChanged(bool value)
{
    this->mConv->setPack(value);
}
//-----------------------------------------------------------------------------
void WidgetConvOptionsGray::swapBytesChanged(bool value)
{
    this->mConv->setSwapBytes(value);
}
//-----------------------------------------------------------------------------
void WidgetConvOptionsGray::mirrorBytesChanged(bool value)
{
    this->mConv->setMirror(value);
}
//-----------------------------------------------------------------------------
