/********************************************************************************
** Form generated from reading UI file 'shownormaldg.ui'
**
** Created by: Qt User Interface Compiler version 6.4.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHOWNORMALDG_H
#define UI_SHOWNORMALDG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ShowNormalDialog
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSlider *horizontalSlider;

    void setupUi(QDialog *ShowNormalDialog)
    {
        if (ShowNormalDialog->objectName().isEmpty())
            ShowNormalDialog->setObjectName("ShowNormalDialog");
        ShowNormalDialog->resize(400, 102);
        verticalLayout = new QVBoxLayout(ShowNormalDialog);
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label = new QLabel(ShowNormalDialog);
        label->setObjectName("label");
        label->setMaximumSize(QSize(80, 16777215));

        horizontalLayout->addWidget(label);

        horizontalSlider = new QSlider(ShowNormalDialog);
        horizontalSlider->setObjectName("horizontalSlider");
        horizontalSlider->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(horizontalSlider);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(ShowNormalDialog);

        QMetaObject::connectSlotsByName(ShowNormalDialog);
    } // setupUi

    void retranslateUi(QDialog *ShowNormalDialog)
    {
        ShowNormalDialog->setWindowTitle(QCoreApplication::translate("ShowNormalDialog", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("ShowNormalDialog", " \346\267\261\345\272\246\346\265\213\350\257\225", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ShowNormalDialog: public Ui_ShowNormalDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHOWNORMALDG_H
