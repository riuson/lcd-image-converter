#include "widgetconvoptionscolor.h"
#include "ui_widgetconvoptionscolor.h"
//-----------------------------------------------------------------------------
#include <QButtonGroup>

#include "bytelistitemdelegate.h"
//-----------------------------------------------------------------------------
WidgetConvOptionsColor::WidgetConvOptionsColor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetConvOptionsColor)
{
    ui->setupUi(this);
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

    this->ui->radioButtonBigEndian->setChecked(true);
    this->ui->radioButtonLittleEndian->setChecked(false);

    this->ui->radioButtonData8->setChecked(true);

    QStringList ordersList;
    ordersList << "RGB";
    ordersList << "RBG";
    ordersList << "GRB";
    ordersList << "GBR";
    ordersList << "BRG";
    ordersList << "BGR";
    this->ui->comboBoxColorsOrder->addItems(ordersList);
    this->ui->comboBoxColorsOrder->setCurrentIndex(0);
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
    if (this->ui->comboBoxColorsOrder->currentIndex() < 0)
        return;
    this->ui->listWidget->clear();
    QStringList list;

    int bits = 8;
    if (this->ui->radioButtonData16->isChecked())
        bits = 16;
    if (this->ui->radioButtonData32->isChecked())
        bits = 32;
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
}
//-----------------------------------------------------------------------------
