#ifndef ACTIONFILEHANDLERS_H
#define ACTIONFILEHANDLERS_H
//-----------------------------------------------------------------------------
#include <QObject>

#include "actionhandlersbase.h"
//-----------------------------------------------------------------------------
class IMainWindow;
//-----------------------------------------------------------------------------
class ActionFileHandlers : public ActionHandlersBase
{
    Q_OBJECT
public:
    explicit ActionFileHandlers(QObject *parent = 0);
    
signals:
    void rememberFilename(const QString &filename);
    void closeRequest(QWidget *tab);
    void tabChanged(QWidget *w, const QString &documentName, const QString &filename);
    void tabCreated(QWidget *w, const QString &documentName, const QString &filename);

public slots:
    void newImage_triggered();
    void newFont_triggered();
    void open_triggered();
    void rename_triggered();
    void save_triggered();
    void saveAs_triggered();
    void close_triggered();
    void convert_triggered();
    void openFile(const QString &filename);
private slots:
    void documentChanged(bool changed, const QString &documentName, const QString &filename);
};
//-----------------------------------------------------------------------------
#endif // ACTIONFILEHANDLERS_H
