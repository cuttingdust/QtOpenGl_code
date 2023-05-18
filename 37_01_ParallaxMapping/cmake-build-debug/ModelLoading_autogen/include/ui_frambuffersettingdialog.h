/********************************************************************************
** Form generated from reading UI file 'frambuffersettingdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.4.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRAMBUFFERSETTINGDIALOG_H
#define UI_FRAMBUFFERSETTINGDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_FrambufferSettingDialog
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *comboBox;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *FrambufferSettingDialog)
    {
        if (FrambufferSettingDialog->objectName().isEmpty())
            FrambufferSettingDialog->setObjectName("FrambufferSettingDialog");
        FrambufferSettingDialog->resize(400, 102);
        verticalLayout = new QVBoxLayout(FrambufferSettingDialog);
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label = new QLabel(FrambufferSettingDialog);
        label->setObjectName("label");
        label->setMaximumSize(QSize(80, 16777215));

        horizontalLayout->addWidget(label);

        comboBox = new QComboBox(FrambufferSettingDialog);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName("comboBox");

        horizontalLayout->addWidget(comboBox);


        verticalLayout->addLayout(horizontalLayout);

        buttonBox = new QDialogButtonBox(FrambufferSettingDialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(FrambufferSettingDialog);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, FrambufferSettingDialog, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, FrambufferSettingDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(FrambufferSettingDialog);
    } // setupUi

    void retranslateUi(QDialog *FrambufferSettingDialog)
    {
        FrambufferSettingDialog->setWindowTitle(QCoreApplication::translate("FrambufferSettingDialog", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("FrambufferSettingDialog", "Frambuffer", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("FrambufferSettingDialog", "\351\273\230\350\256\244", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("FrambufferSettingDialog", "\347\233\270\345\217\215", nullptr));
        comboBox->setItemText(2, QCoreApplication::translate("FrambufferSettingDialog", "\347\201\260\345\272\246", nullptr));
        comboBox->setItemText(3, QCoreApplication::translate("FrambufferSettingDialog", "\346\235\203\351\207\215\347\201\260\345\272\246", nullptr));
        comboBox->setItemText(4, QCoreApplication::translate("FrambufferSettingDialog", "\351\224\220\345\214\226", nullptr));
        comboBox->setItemText(5, QCoreApplication::translate("FrambufferSettingDialog", "\346\250\241\347\263\212", nullptr));

    } // retranslateUi

};

namespace Ui {
    class FrambufferSettingDialog: public Ui_FrambufferSettingDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRAMBUFFERSETTINGDIALOG_H
