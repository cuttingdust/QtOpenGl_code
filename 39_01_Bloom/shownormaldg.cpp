#include "shownormaldg.h"
#include "ui_shownormaldg.h"


ShowNormalDg::ShowNormalDg(QWidget *parent) :
    QDialog(parent),
    ui_(new Ui::ShowNormalDialog)
{
    ui_->setupUi(this);
    connect(ui_->horizontalSlider, &QSlider::valueChanged, this, &ShowNormalDg::signalShowNormalLength);
}


ShowNormalDg::~ShowNormalDg()
{
    delete ui_;
}

auto ShowNormalDg::slotShowNormal(int vau) -> void {
    normalLength_ = vau;
}

auto ShowNormalDg::getNormalLength() const -> int {
    return normalLength_;
}
