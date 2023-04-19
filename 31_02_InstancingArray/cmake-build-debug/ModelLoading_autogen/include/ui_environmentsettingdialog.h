/********************************************************************************
** Form generated from reading UI file 'environmentsettingdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.4.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ENVIRONMENTSETTINGDIALOG_H
#define UI_ENVIRONMENTSETTINGDIALOG_H

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

class Ui_EnvironmentSettingDialog
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *comboBox;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *EnvironmentSettingDialog)
    {
        if (EnvironmentSettingDialog->objectName().isEmpty())
            EnvironmentSettingDialog->setObjectName("EnvironmentSettingDialog");
        EnvironmentSettingDialog->resize(400, 102);
        verticalLayout = new QVBoxLayout(EnvironmentSettingDialog);
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label = new QLabel(EnvironmentSettingDialog);
        label->setObjectName("label");
        label->setMaximumSize(QSize(80, 16777215));

        horizontalLayout->addWidget(label);

        comboBox = new QComboBox(EnvironmentSettingDialog);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName("comboBox");

        horizontalLayout->addWidget(comboBox);


        verticalLayout->addLayout(horizontalLayout);

        buttonBox = new QDialogButtonBox(EnvironmentSettingDialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(EnvironmentSettingDialog);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, EnvironmentSettingDialog, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, EnvironmentSettingDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(EnvironmentSettingDialog);
    } // setupUi

    void retranslateUi(QDialog *EnvironmentSettingDialog)
    {
        EnvironmentSettingDialog->setWindowTitle(QCoreApplication::translate("EnvironmentSettingDialog", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("EnvironmentSettingDialog", " \351\200\211\346\213\251\347\216\257\345\242\203", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("EnvironmentSettingDialog", "\346\262\231\346\274\240", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("EnvironmentSettingDialog", "\345\267\245\345\216\202", nullptr));
        comboBox->setItemText(2, QCoreApplication::translate("EnvironmentSettingDialog", "\346\201\220\346\200\226", nullptr));
        comboBox->setItemText(3, QCoreApplication::translate("EnvironmentSettingDialog", "\347\224\237\345\214\226\345\256\236\351\252\214\345\256\244", nullptr));

    } // retranslateUi

};

namespace Ui {
    class EnvironmentSettingDialog: public Ui_EnvironmentSettingDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ENVIRONMENTSETTINGDIALOG_H
