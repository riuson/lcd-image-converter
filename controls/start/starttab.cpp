/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2012 riuson
 * mailto: riuson@gmail.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/
 */

#include "starttab.h"
#include "ui_starttab.h"

#include <QFileInfo>

StartTab::StartTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartTab)
{
    ui->setupUi(this);

    this->connect(this->ui->labelRecentFiles, SIGNAL(linkActivated(QString)), SIGNAL(openRecent(QString)));
    this->connect(this->ui->buttonNewImage, SIGNAL(clicked()), SIGNAL(createNewImage()));
    this->connect(this->ui->buttonNewFont, SIGNAL(clicked()), SIGNAL(createNewFont()));

    this->mRecentFilesList = NULL;
}

StartTab::~StartTab()
{
    delete ui;
}

void StartTab::setRecentFiles(const QStringList *list)
{
    this->mRecentFilesList = list;

    if (list->count() > 0)
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
    else
        this->ui->labelRecentFiles->hide();
}

const QString StartTab::tabName() const
{
    QString result = tr("Start");
    return result;
}

void StartTab::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
    {
        ui->retranslateUi(this);
        this->setRecentFiles(this->mRecentFilesList);

        // find parent QTabWidget
        QObject *w = this;
        while (w != NULL)
        {
            QTabWidget *tab = dynamic_cast<QTabWidget *> (w);
            if (tab != NULL)
            {
                int index = tab->indexOf(this);
                tab->setTabText(index, this->tabName());
                break;
            }
            w = w->parent();
        }
        break;
    }
    default:
        break;
    }
}

