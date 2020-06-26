// Copyright (c) 2019 The PIVX developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DEFAULTINPUTDIALOG_H
#define DEFAULTINPUTDIALOG_H

#include <QDialog>

namespace Ui {
class DefaultInputDialog;
}

class DefaultInputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DefaultInputDialog(QWidget *parent = nullptr);
    ~DefaultInputDialog();

    void setText(QString title = "", QString message = "", QString prompt = "", QString okBtnText = "", QString cancelBtnText = "");
    const std::string getInput() const;

    bool isOk = false;
private:
    Ui::DefaultInputDialog *ui;
};

#endif // DEFAULTINPUTDIALOG_H
