#include "environmentsettingdialog.h"
#include "ui_environmentsettingdialog.h"

EnvironmentSettingDialog::EnvironmentSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui_(new Ui::EnvironmentSettingDialog)
{
    ui_->setupUi(this);
    connect(ui_->comboBox, &QComboBox::currentIndexChanged, this, &EnvironmentSettingDialog::slotEnvSettingChang);
}

void EnvironmentSettingDialog::setEnvType(EnvironmentType type)
{
    ui_->comboBox->setCurrentIndex(static_cast<int>(type));
}

EnvironmentType EnvironmentSettingDialog::getEnvType() const
{
    return envType_;
}

EnvironmentSettingDialog::~EnvironmentSettingDialog()
{
    delete ui_;
}

void EnvironmentSettingDialog::slotEnvSettingChang(int index)
{
    envType_ = static_cast<EnvironmentType>(index);
}
