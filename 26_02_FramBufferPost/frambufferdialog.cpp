#include "frambufferdialog.h"
#include "ui_frambuffersettingdialog.h"

FrambufferDialog::FrambufferDialog(QWidget *parent) :
    QDialog(parent),
    ui_(new Ui::FrambufferSettingDialog)
{
    ui_->setupUi(this);
    connect(ui_->comboBox, &QComboBox::currentIndexChanged, this, &FrambufferDialog::slotFbTypeChang);
}

void FrambufferDialog::setFbType(FrambufferType type)
{
    ui_->comboBox->setCurrentIndex(static_cast<int>(type));
}

FrambufferType FrambufferDialog::getFbType() const
{
    return fbType_;
}

FrambufferDialog::~FrambufferDialog()
{
    delete ui_;
}

void FrambufferDialog::slotFbTypeChang(int index) {
    fbType_ = static_cast<FrambufferType>(index);
}

