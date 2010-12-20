#include "widgetconvoptionsmono.h"
#include "ui_widgetconvoptionsmono.h"

#include "bytelistitemdelegate.h"
//-----------------------------------------------------------------------------
WidgetConvOptionsMono::WidgetConvOptionsMono(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetConvOptionsMono)
{
    ui->setupUi(this);
    this->mDelegate = new ByteListItemDelegate(this);
    this->ui->listWidget->setItemDelegate(this->mDelegate);
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
    this->ui->listWidget->clear();
    QStringList list;

    int bits = 8;
    if (this->ui->radioButtonData16->isChecked())
        bits = 16;
    if (this->ui->radioButtonData32->isChecked())
        bits = 32;
    this->mDelegate->setBitsCount(bits);

    bool littleEndian = this->ui->radioButtonLittleEndian->isChecked();

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
    this->ui->listWidget->addItems(list);
}
//-----------------------------------------------------------------------------
