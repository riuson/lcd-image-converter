#include "widgetconvoptionsmono.h"
#include "ui_widgetconvoptionsmono.h"

#include <QButtonGroup>

#include "bytelistitemdelegate.h"
#include "convertermono.h"
//-----------------------------------------------------------------------------
WidgetConvOptionsMono::WidgetConvOptionsMono(IConverter *options, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetConvOptionsMono)
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

    this->mConv = dynamic_cast<ConverterMono*>(options);
    if (this->mConv == NULL)
    {
        this->mConv = new ConverterMono(this);
        this->mConv->loadSettings();
    }

    ConverterMono::BytesOrder orderBytes;
    ConverterMono::DataLength length;
    bool mirror;
    int level;
    ConverterMono::ConvMonoType convType;
    this->mConv->options(&orderBytes, &length, &mirror, &level, &convType);

    this->ui->radioButtonBigEndian->setChecked(orderBytes == ConverterMono::BigEndian);
    this->ui->radioButtonLittleEndian->setChecked(orderBytes == ConverterMono::LittleEndian);

    this->ui->radioButtonData8->setChecked(length == ConverterMono::Data8);
    this->ui->radioButtonData16->setChecked(length == ConverterMono::Data16);
    this->ui->radioButtonData32->setChecked(length == ConverterMono::Data32);

    this->ui->checkBoxMirrorBits->setChecked(mirror);

    this->ui->horizontalScrollBar->setValue(level);

    this->ui->comboBoxConvType->addItem("Edge", ConverterMono::Edge);
    this->ui->comboBoxConvType->addItem("Diffuse dither", ConverterMono::DiffuseDither);
    this->ui->comboBoxConvType->addItem("Ordered dither", ConverterMono::OrderedDither);
    this->ui->comboBoxConvType->addItem("Threshold dither", ConverterMono::ThresholdDither);
    this->ui->comboBoxConvType->setCurrentIndex(convType);

    this->mReady = true;
    this->updatePreview();
}
//-----------------------------------------------------------------------------
WidgetConvOptionsMono::~WidgetConvOptionsMono()
{
    delete ui;
}
//-----------------------------------------------------------------------------
void WidgetConvOptionsMono::changeEvent(QEvent *e)
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
void WidgetConvOptionsMono::updatePreview()
{
    this->ui->horizontalScrollBar->setEnabled(this->ui->comboBoxConvType->currentIndex() == 0);

    if (!this->mReady)
        return;

    this->ui->listWidget->clear();
    QStringList list;

    int bits = 8;
    ConverterMono::DataLength length = ConverterMono::Data8;

    if (this->ui->radioButtonData16->isChecked())
        bits = 16, length = ConverterMono::Data16;
    if (this->ui->radioButtonData32->isChecked())
        bits = 32, length = ConverterMono::Data32;
    this->mDelegate->setBitsCount(bits);

    bool ok;
    int a = this->ui->comboBoxConvType->itemData(this->ui->comboBoxConvType->currentIndex()).toInt(&ok);
    ConverterMono::ConvMonoType dithType = ConverterMono::Edge;
    if (ok)
        dithType = (ConverterMono::ConvMonoType)a;

    bool littleEndian = this->ui->radioButtonLittleEndian->isChecked();
    bool mirror = this->ui->checkBoxMirrorBits->isChecked();

    for (int i = 0; i < 80; i++)
    {
        int a = i % bits;
        int b = bits - a - 1;
        list.append(QString("b%1").arg(b));
        //QString str = QString("b%1").arg(b);
        //QListWidgetItem *item = new QListWidgetItem(str, this->ui->listWidget, b);
        //this->ui->listWidget->addItem(item);
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
        for (int i = 0; i < list.count(); i+= 8)
        {
            QStringList tmp = list.mid(i, 8);
            for (int j = tmp.count() - 1; j >= 0; j--)
            {
                outlist.append(tmp.at(j));
            }
        }
        list = outlist;
    }
    this->ui->listWidget->addItems(list);

    this->mConv->setOptions(littleEndian ? ConverterMono::LittleEndian : ConverterMono::BigEndian,
                               length,
                               mirror,
                               this->ui->horizontalScrollBar->value(),
                               dithType);
    this->mConv->saveSettings();

    emit this->settingsChanged();
}
//-----------------------------------------------------------------------------
