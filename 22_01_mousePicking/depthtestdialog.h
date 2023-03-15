#ifndef DEPTHTEST_TEST_DIALOG_H
#define DEPTHTEST_TEST_DIALOG_H

#include <QDialog>
#include <QOpenGLFunctions_4_1_Core>

namespace Ui {
class DepthTestDialog;
}
enum DepthFuncType{
    DFT_LESS,
    DFT_ALWAYS,
    DFT_GREATER,
    DFT_EQUAL
};

class DepthTestDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DepthTestDialog(QWidget *parent = 0);
    ~DepthTestDialog();
public:
    void setDepthTestFunc(const DepthFuncType depthFuncType);
    DepthFuncType getDepthTestFunc() const;

private slots:
    void slotDepthFuncChang(int index);

private:
    Ui::DepthTestDialog *ui_;
    DepthFuncType depthFuncType_;
    GLenum testFunc_;
public:
    GLenum getTestFunc() const;
    void setTestFunc(GLenum testFunc);
};

#endif
