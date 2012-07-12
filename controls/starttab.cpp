#include "starttab.h"
#include "ui_starttab.h"

#include <QFileInfo>
//-----------------------------------------------------------------------------
StartTab::StartTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::starttab)
{
    ui->setupUi(this);

    this->connect(this->ui->labelRecentFiles, SIGNAL(linkActivated(QString)), SIGNAL(openRecent(QString)));
    this->connect(this->ui->buttonNewImage, SIGNAL(clicked()), SIGNAL(createNewImage()));
    this->connect(this->ui->buttonNewFont, SIGNAL(clicked()), SIGNAL(createNewFont()));
}
//-----------------------------------------------------------------------------
StartTab::~StartTab()
{
    delete ui;
}
//-----------------------------------------------------------------------------
void StartTab::setRecentFiles(const QStringList *list)
{
    QString listTemplate = this->ui->labelRecentFiles->text();
    QString listItems;
    for (int i = 0; i < list->count(); i++)
    {
        QString filename = list->at(i);
        QString strippedName = QFileInfo(filename).fileName();
        QString text = QString("<li><a href=\"%1\">%2</a></li>").arg(filename).arg(strippedName);
        listItems.append(text);
    }
    listTemplate.replace("#list#", listItems);
    this->ui->labelRecentFiles->setText(listTemplate);
}
//-----------------------------------------------------------------------------
