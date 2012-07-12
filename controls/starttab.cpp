#include "starttab.h"
#include "ui_starttab.h"
//-----------------------------------------------------------------------------
StartTab::StartTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::starttab)
{
    ui->setupUi(this);
}
//-----------------------------------------------------------------------------
StartTab::~StartTab()
{
    delete ui;
}
//-----------------------------------------------------------------------------
