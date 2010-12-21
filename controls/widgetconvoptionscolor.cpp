#include "widgetconvoptionscolor.h"
#include "ui_widgetconvoptionscolor.h"
//-----------------------------------------------------------------------------
#include <QButtonGroup>

#include "bytelistitemdelegate.h"
#include "convertercolor.h"
//-----------------------------------------------------------------------------
WidgetConvOptionsColor::WidgetConvOptionsColor(IConverter *options, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetConvOptionsColor)
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
    this->mDelegate->enableHighlightColors(true);
    this->ui->listWidget->setItemDelegate(this->mDelegate);

    this->mConv = new ConverterColor(this);

    this->mConv->loadSettings();
    ConverterColor::BytesOrder orderBytes;
    ConverterColor::DataLength length;
    bool mirror, pack;
    int bitsPerPointRed, bitsPerPointGreen, bitsPerPointBlue;
    ConverterColor::ColorsOrder orderColors;
    this->mConv->options(&orderBytes,
                            &length,
                            &mirror,
                            &pack,
                            &bitsPerPointRed,
                            &bitsPerPointGreen,
                            &bitsPerPointBlue,
                            &orderColors);

    this->ui->radioButtonBigEndian->setChecked(orderBytes == ConverterColor::BigEndian);
    this->ui->radioButtonLittleEndian->setChecked(orderBytes == ConverterColor::LittleEndian);

    this->ui->radioButtonData8->setChecked(length == ConverterColor::Data8);
    this->ui->radioButtonData16->setChecked(length == ConverterColor::Data16);
    this->ui->radioButtonData32->setChecked(length == ConverterColor::Data32);

    this->ui->checkBoxMirrorBits->setChecked(mirror);
    this->ui->checkBoxPack->setChecked(pack);

    this->ui->spinBoxRedbits->setValue(bitsPerPointRed);
    this->ui->spinBoxGreenBits->setValue(bitsPerPointGreen);
    this->ui->spinBoxBlueBits->setValue(bitsPerPointBlue);

    QStringList ordersList;
    ordersList << "RGB";
    ordersList << "RBG";
    ordersList << "GRB";
    ordersList << "GBR";
    ordersList << "BRG";
    ordersList << "BGR";
    this->ui->comboBoxColorsOrder->addItems(ordersList);
    this->ui->comboBoxColorsOrder->setCurrentIndex((int)orderColors);

    this->mReady = true;
    this->updatePreview();
}
//-----------------------------------------------------------------------------
WidgetConvOptionsColor::~WidgetConvOptionsColor()
{
    delete ui;
}
//-----------------------------------------------------------------------------
void WidgetConvOptionsColor::changeEvent(QEvent *e)
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
void WidgetConvOptionsColor::updatePreview()
{
    if (!this->mReady)
        return;

    this->ui->listWidget->clear();
    QStringList list;

    int bits = 8;
    ConverterColor::DataLength length = ConverterColor::Data8;
    if (this->ui->radioButtonData16->isChecked())
        bits = 16, length = ConverterColor::Data16;
    if (this->ui->radioButtonData32->isChecked())
        bits = 32, length = ConverterColor::Data32;
    this->mDelegate->setBitsCount(bits);

    int bitsPerPointR = this->ui->spinBoxRedbits->value();
    int bitsPerPointG = this->ui->spinBoxGreenBits->value();
    int bitsPerPointB = this->ui->spinBoxBlueBits->value();

    QStringList colorsRed, colorsGreen, colorsBlue;
    for (int i = bitsPerPointR - 1; i >= 0; i--)
        colorsRed << QString("R.%1").arg(i);
    for (int i = bitsPerPointG - 1; i >= 0; i--)
        colorsGreen << QString("G.%1").arg(i);
    for (int i = bitsPerPointB - 1; i >= 0; i--)
        colorsBlue << QString("B.%1").arg(i);

    QString colorsOrder = this->ui->comboBoxColorsOrder->currentText();
    ConverterColor::ColorsOrder orderColors = (ConverterColor::ColorsOrder)this->ui->comboBoxColorsOrder->currentIndex();
    QStringList colors;
    for (int i = 0; i < colorsOrder.length(); i++)
    {
        if (colorsOrder.at(i) == QChar('R'))
            colors << colorsRed;
        if (colorsOrder.at(i) == QChar('G'))
            colors << colorsGreen;
        if (colorsOrder.at(i) == QChar('B'))
            colors << colorsBlue;
    }

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

    this->mConv->setOptions(littleEndian ? ConverterColor::LittleEndian : ConverterColor::BigEndian,
                               length,
                               mirror,
                               pack,
                               bitsPerPointR,
                               bitsPerPointG,
                               bitsPerPointB,
                               orderColors);
    this->mConv->saveSettings();

    emit this->settingsChanged();
}
//-----------------------------------------------------------------------------
