#ifndef SETUPTABMATRIX_H
#define SETUPTABMATRIX_H
//-----------------------------------------------------------------------------
#include <QWidget>
//-----------------------------------------------------------------------------
namespace Ui {
class SetupTabMatrix;
}
//-----------------------------------------------------------------------------
class ConversionMatrix;
class MatrixPreviewModel;
class MatrixItemDelegate;
class QMenu;
//-----------------------------------------------------------------------------
class SetupTabMatrix : public QWidget
{
    Q_OBJECT

public:
    explicit SetupTabMatrix(ConversionMatrix *matrix, QWidget *parent = 0);
    ~SetupTabMatrix();
    static const QString &title();

public slots:
    void matrixChanged();

private:
    Ui::SetupTabMatrix *ui;
    ConversionMatrix *mMatrix;
    MatrixPreviewModel *mMatrixModel;
    MatrixItemDelegate *mMatrixItemDelegate;
    QMenu *mMenu;

private slots:
    void on_comboBoxConversionType_currentIndexChanged(int index);
    void on_tableViewOperations_customContextMenuRequested(const QPoint &point);
    void operationAdd();
    void operationShift();
    void operationRemove();
    void maskReset();
};
//-----------------------------------------------------------------------------
#endif // SETUPTABMATRIX_H
