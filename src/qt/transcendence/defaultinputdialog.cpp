// Copyright (c) 2019 The PIVX developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "qt/transcendence/defaultinputdialog.h"
#include "qt/transcendence/forms/ui_defaultinputdialog.h"
#include "guiutil.h"
#include "qt/transcendence/qtutils.h"

DefaultInputDialog::DefaultInputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DefaultInputDialog)
{
    ui->setupUi(this);

    // Stylesheet
    this->setStyleSheet(parent ? parent->styleSheet() : GUIUtil::loadStyleSheet());

    // Container
    ui->frame->setProperty("cssClass", "container-dialog");

    // Text
    ui->labelTitle->setText("Dialog Title");
    ui->labelTitle->setProperty("cssClass", "text-title-dialog");

    ui->labelMessage->setText("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.");
    ui->labelMessage->setProperty("cssClass", "text-main-grey");

    ui->labelInput->setText("Input:");
    ui->labelInput->setProperty("cssClass", "text-main-grey");
    
    initCssEditLine(ui->lineEdit, true);

    // Buttons
    ui->btnEsc->setText("");
    ui->btnEsc->setProperty("cssClass", "ic-close");

    ui->btnCancel->setProperty("cssClass", "btn-dialog-cancel");
    ui->btnSave->setText("OK");
    ui->btnSave->setProperty("cssClass", "btn-primary");

    connect(ui->btnEsc, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->btnSave, &QPushButton::clicked, [this](){this->isOk = true; accept();});
}

void DefaultInputDialog::setText(QString title, QString message, QString prompt, QString okBtnText, QString cancelBtnText){
    if(!okBtnText.isNull()) ui->btnSave->setText(okBtnText);
    if(!cancelBtnText.isNull()){
        ui->btnCancel->setVisible(true);
        ui->btnCancel->setText(cancelBtnText);
    }else{
        ui->btnCancel->setVisible(false);
    }
    if(!message.isNull()) ui->labelMessage->setText(message);
    if(!title.isNull()) ui->labelTitle->setText(title);
    if(!prompt.isNull()) ui->labelInput->setText(prompt);
}

const std::string DefaultInputDialog::getInput() const {
    QString str = ui->lineEdit->text();
    return str.toStdString();
}

DefaultInputDialog::~DefaultInputDialog()
{
    delete ui;
}
