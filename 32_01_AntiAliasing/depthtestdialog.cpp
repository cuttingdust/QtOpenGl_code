#include "depthtestdialog.h"
#include "ui_depthtestdialog.h"


DepthTestDialog::DepthTestDialog(QWidget *parent) :
    QDialog(parent),
    ui_(new Ui::DepthTestDialog)
{
    ui_->setupUi(this);
    connect(ui_->comboBox, &QComboBox::currentIndexChanged, this, &DepthTestDialog::slotDepthFuncChang);
}

void DepthTestDialog::setDepthTestFunc(const DepthFuncType depthFuncType) {
    ui_->comboBox->setCurrentIndex(static_cast<int>(depthFuncType));
}

DepthFuncType DepthTestDialog::getDepthTestFunc() const {
    return depthFuncType_;
}

DepthTestDialog::~DepthTestDialog()
{
    delete ui_;
}

void DepthTestDialog::slotDepthFuncChang(int index) {
    depthFuncType_ = static_cast<DepthFuncType>(index);

    switch (depthFuncType_) {
        case DepthFuncType::DFT_LESS:
            setTestFunc( GL_LESS);
            break;
        case DepthFuncType::DFT_ALWAYS:
            setTestFunc(GL_ALWAYS);
            break;
        case DepthFuncType::DFT_GREATER:
            setTestFunc(GL_GREATER);
            break;
        case DepthFuncType::DFT_EQUAL:
            setTestFunc(GL_EQUAL);
            break;
        default:
            break;
    }
}

GLenum DepthTestDialog::getTestFunc() const {
    return testFunc_;
}

void DepthTestDialog::setTestFunc(GLenum testFunc) {
    testFunc_ = testFunc;
}
