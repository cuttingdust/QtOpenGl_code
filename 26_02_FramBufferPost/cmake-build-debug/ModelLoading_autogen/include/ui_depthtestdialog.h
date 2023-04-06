/********************************************************************************
** Form generated from reading UI file 'depthtestdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.4.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEPTHTESTDIALOG_H
#define UI_DEPTHTESTDIALOG_H

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

class Ui_DepthTestDialog
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *comboBox;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *DepthTestDialog)
    {
        if (DepthTestDialog->objectName().isEmpty())
            DepthTestDialog->setObjectName("DepthTestDialog");
        DepthTestDialog->resize(400, 102);
        verticalLayout = new QVBoxLayout(DepthTestDialog);
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label = new QLabel(DepthTestDialog);
        label->setObjectName("label");
        label->setMaximumSize(QSize(80, 16777215));

        horizontalLayout->addWidget(label);

        comboBox = new QComboBox(DepthTestDialog);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName("comboBox");

        horizontalLayout->addWidget(comboBox);


        verticalLayout->addLayout(horizontalLayout);

        buttonBox = new QDialogButtonBox(DepthTestDialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(DepthTestDialog);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, DepthTestDialog, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, DepthTestDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(DepthTestDialog);
    } // setupUi

    void retranslateUi(QDialog *DepthTestDialog)
    {
        DepthTestDialog->setWindowTitle(QCoreApplication::translate("DepthTestDialog", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("DepthTestDialog", " \346\267\261\345\272\246\346\265\213\350\257\225", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("DepthTestDialog", "GL_LESS", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("DepthTestDialog", "GL_ALWAYS", nullptr));
        comboBox->setItemText(2, QCoreApplication::translate("DepthTestDialog", "GL_GREATER", nullptr));
        comboBox->setItemText(3, QCoreApplication::translate("DepthTestDialog", "GL_EQUAL", nullptr));

    } // retranslateUi

};

namespace Ui {
    class DepthTestDialog: public Ui_DepthTestDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEPTHTESTDIALOG_H
