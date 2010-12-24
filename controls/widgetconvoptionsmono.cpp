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

    this->mDelegate = new ByteListItemDelegate(this);
    this->ui->listWidget->setItemDelegate(this->mDelegate);

    this->mConv = dynamic_cast<ConverterMono*>(options);
    if (this->mConv == NULL)
    {
        this->mConv = new ConverterMono(this);
        this->mConv->loadSettings();
    }

    int level = this->mConv->level();
    ConverterMono::ConvMonoType convType = this->mConv->dithType();

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

    ConverterMono::DataLength length = this->mConv->length();
    int bits = (int)length;

    this->mDelegate->setBitsCount(bits);

    bool ok;
    int a = this->ui->comboBoxConvType->itemData(this->ui->comboBoxConvType->currentIndex()).toInt(&ok);
    ConverterMono::ConvMonoType dithType = ConverterMono::Edge;
    if (ok)
        dithType = (ConverterMono::ConvMonoType)a;

    bool littleEndian = this->mConv->order() == IConverter::LittleEndian;
    bool mirror = this->mConv->mirror();

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

    this->mConv->setDithType(dithType);
    this->mConv->setLevel(this->ui->horizontalScrollBar->value());

    this->mConv->saveSettings();

    emit this->settingsChanged();
}
//-----------------------------------------------------------------------------
void WidgetConvOptionsMono::dataLengthChanged(int value)
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
    this->updatePreview();
}
//-----------------------------------------------------------------------------
void WidgetConvOptionsMono::dataPackChanged(bool value)
{
    Q_UNUSED(value);
}
//-----------------------------------------------------------------------------
void WidgetConvOptionsMono::swapBytesChanged(bool value)
{
    this->mConv->setOrder(value ? IConverter::LittleEndian : IConverter::BigEndian);
    this->updatePreview();
}
//-----------------------------------------------------------------------------
void WidgetConvOptionsMono::mirrorBytesChanged(bool value)
{
    this->mConv->setMirror(value);
    this->updatePreview();
}
//-----------------------------------------------------------------------------
