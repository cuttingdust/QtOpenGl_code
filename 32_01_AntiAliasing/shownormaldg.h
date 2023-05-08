#ifndef SHOW_NORMAL_DG_H
#define SHOW_NORMAL_DG_H

#include <QDialog>
#include <QOpenGLFunctions_4_1_Core>

namespace Ui {
class ShowNormalDialog;
}

class ShowNormalDg : public QDialog
{
    Q_OBJECT
public:
    explicit ShowNormalDg(QWidget *parent = 0);
    ~ShowNormalDg();

signals:
    auto signalShowNormalLength(int vau) -> void;

public:
   auto getNormalLength () const -> int;


protected slots:
    auto slotShowNormal(int vau) ->void;

private:
    Ui::ShowNormalDialog *ui_;

    int normalLength_ = 0;
};

#endif // SHOW_NORMAL_DG_H
