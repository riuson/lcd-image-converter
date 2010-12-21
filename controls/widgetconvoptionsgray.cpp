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

    this->mGroupByteOrder = new QButtonGroup(this);
    this->mGroupDataSize = new QButtonGroup(this);

    this->mGroupByteOrder->addButton(this->ui->radioButtonBigEndian);
    this->mGroupByteOrder->addButton(this->ui->radioButtonLittleEndian);

    this->mGroupDataSize->addButton(this->ui->radioButtonData8);
    this->mGroupDataSize->addButton(this->ui->radioButtonData16);
    this->mGroupDataSize->addButton(this->ui->radioButtonData32);

    this->mDelegate = new ByteListItemDelegate(this);
    this->ui->listWidget->setItemDelegate(this->mDelegate);

    this->mConv = dynamic_cast<ConverterGrayscale*>(options);
    if (this->mConv == NULL)
    {
        this->mConv = new ConverterGrayscale(this);
        this->mConv->loadSettings();
    }

    this->mConv->loadSettings();
    ConverterGrayscale::BytesOrder orderBytes;
    ConverterGrayscale::DataLength length;
    bool mirror, pack;
    int bitsPerPoint;
    this->mConv->options(&orderBytes, &length, &mirror, &pack, &bitsPerPoint);

    this->ui->radioButtonBigEndian->setChecked(orderBytes == ConverterGrayscale::BigEndian);
    this->ui->radioButtonLittleEndian->setChecked(orderBytes == ConverterGrayscale::LittleEndian);

    this->ui->radioButtonData8->setChecked(length == ConverterGrayscale::Data8);
    this->ui->radioButtonData16->setChecked(length == ConverterGrayscale::Data16);
    this->ui->radioButtonData32->setChecked(length == ConverterGrayscale::Data32);

    this->ui->checkBoxMirrorBits->setChecked(mirror);
    this->ui->checkBoxPack->setChecked(pack);

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

    int bits = 8;
    ConverterGrayscale::DataLength length = ConverterGrayscale::Data8;
    if (this->ui->radioButtonData16->isChecked())
        bits = 16, length = ConverterGrayscale::Data16;
    if (this->ui->radioButtonData32->isChecked())
        bits = 32, length = ConverterGrayscale::Data32;
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

    bool littleEndian = this->ui->radioButtonLittleEndian->isChecked();
    bool mirror = this->ui->checkBoxMirrorBits->isChecked();
    bool pack = this->ui->checkBoxPack->isChecked();

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

    if (littleEndian)
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

    this->mConv->setOptions(littleEndian ? ConverterGrayscale::LittleEndian : ConverterGrayscale::BigEndian,
                               length,
                               mirror,
                               pack,
                               bitsPerPoint);
    this->mConv->saveSettings();
}
//-----------------------------------------------------------------------------
