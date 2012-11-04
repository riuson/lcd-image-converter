/*
 * LCD Image Converter. Converts images and fonts for embedded applciations.
 * Copyright (C) 2010 riuson
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

#include "dialogabout.h"
#include "ui_dialogabout.h"

#include <QTextStream>
#include <QFile>
#include <QXmlQuery>
#include <QBuffer>
#include "revisioninfo.h"
//-----------------------------------------------------------------------------
DialogAbout::DialogAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAbout)
{
    ui->setupUi(this);

    // hide ? button from title
    this->setWindowFlags(this->windowFlags() & (~Qt::WindowContextHelpButtonHint));

    // update icon
    QIcon icon;
    icon.addFile(":/images/icon64", QSize(64, 64));
    this->setWindowIcon(icon);

    this->showLicense();

    // show revision info
    {
        QString about = this->ui->labelInfo->text();
        QString formattedAbout = QString(about).arg(RevisionInfo::hash(), RevisionInfo::date());
        this->ui->labelInfo->setText(formattedAbout);
    }

    // show Qt version
    {
        QString about = this->ui->labelInfo->text();
        QString formattedAbout = QString(about).arg(qVersion());
        this->ui->labelInfo->setText(formattedAbout);
    }

    this->connect(this->ui->labelLinks, SIGNAL(linkActivated(QString)), SLOT(linkActivated(QString)));

    // focus on Close button
    this->ui->buttonBox->setFocus();
}
//-----------------------------------------------------------------------------
DialogAbout::~DialogAbout()
{
    delete ui;
}
//-----------------------------------------------------------------------------
void DialogAbout::showLicense()
{
    QFile file_license(":/text/gpl3");
    if (file_license.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file_license);
        QString license = stream.readAll();
        file_license.close();

        this->ui->textBrowser->setText(license);
    }
}
//-----------------------------------------------------------------------------
void DialogAbout::showHistory()
{
    // XML file
    QString xml;
    {
        QFile file(":/history/changes");
        if (file.open(QIODevice::ReadOnly))
        {
            QTextStream stream(&file);
            xml = stream.readAll();
            file.close();
        }
    }

    // XSL file
    QString xsl;
    {
        QFile file(":/history/xsl");
        if (file.open(QIODevice::ReadOnly))
        {
            QTextStream stream(&file);
            xsl = stream.readAll();
            file.close();
        }
    }

    // CSS file
    QString style;
    {
        QFile file(":/history/style");
        if (file.open(QIODevice::ReadOnly))
        {
            QTextStream stream(&file);
            style = stream.readAll();
            file.close();
        }
    }

    // transform
    QString html;
    if (this->transformHistory(xml, xsl, &html))
    {
        this->ui->textBrowser->document()->setDefaultStyleSheet(style);
        this->ui->textBrowser->setHtml(html);
    }
}
//-----------------------------------------------------------------------------
bool DialogAbout::transformHistory(const QString &xml, const QString &xsl, QString *html)
{
    bool isSuccessfully = false;

    // XML file
    QBuffer history;
    {
        QString value = xml;

        value.replace("<sha1>current</sha1>", QString("<sha1>%1</sha1>").arg(RevisionInfo::hash()));
        value.replace("<date>current</date>", QString("<date>%1</date>").arg(RevisionInfo::date()));

        QByteArray array = value.toUtf8();

        history.setData(array);
    }

    // XSL file
    QBuffer transform;
    {
        QString value = xsl;

        value.replace("$current_date", QString("%1").arg(RevisionInfo::date()));

        QByteArray array = value.toUtf8();

        transform.setData(array);
    }

    // transform
    if (history.open(QIODevice::ReadOnly))
    {
        if (transform.open(QIODevice::ReadOnly))
        {
            QXmlQuery query(QXmlQuery::XSLT20);

            query.setFocus(&history);
            query.setQuery(&transform);

            QString resultHtml = "";
            if (query.evaluateTo(&resultHtml))
            {
                *html = resultHtml;
                isSuccessfully = true;
            }

            transform.close();
        }
        history.close();
    }

    return isSuccessfully;
}
//-----------------------------------------------------------------------------
void DialogAbout::linkActivated(const QString &link)
{
    if (link == "license")
    {
        this->showLicense();
    }
    if (link == "history")
    {
        this->showHistory();
    }
}
//-----------------------------------------------------------------------------
