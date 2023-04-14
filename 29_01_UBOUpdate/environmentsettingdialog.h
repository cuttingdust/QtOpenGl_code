#ifndef __ENVIRONMENTSETTINGDIALOG_H__
#define __ENVIRONMENTSETTINGDIALOG_H__

#include <QDialog>

namespace Ui {
class EnvironmentSettingDialog;
}
enum EnvironmentType{
    ET_DESERT,
    ET_FACTTORY,
    ET_HORROR,
    ET_BIOCHEMICALLAB
};

class EnvironmentSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EnvironmentSettingDialog(QWidget *parent = 0);
    ~EnvironmentSettingDialog();
public:
    void setEnvType(EnvironmentType type);
    EnvironmentType getEnvType() const;
private slots:
    void slotEnvSettingChang(int index);

private:
    Ui::EnvironmentSettingDialog *ui_;
    EnvironmentType envType_;
};

#endif // __ENVIRONMENTSETTINGDIALOG_H__
