#ifndef FRAMBUFFER_DIALOG_H
#define FRAMBUFFER_DIALOG_H

#include <QDialog>

namespace Ui {
    class FrambufferSettingDialog;
}
enum FrambufferType{
    FT_DEFAULT,
    FT_OPPSITE,
    FT_GRAYSCALE,
    FT_GRAYSCALEWEIGHT,
    FT_SHARPENING,
    FT_DIM
};

class FrambufferDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FrambufferDialog(QWidget *parent = 0);
    ~FrambufferDialog();
public:
    void setFbType(FrambufferType type);
    FrambufferType getFbType() const;
private slots:
    void slotFbTypeChang(int index);

private:
    Ui::FrambufferSettingDialog *ui_;
    FrambufferType fbType_;
};

#endif // FRAMBUFFER_DIALOG_H
