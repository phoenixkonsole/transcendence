// Copyright (c) 2019 The PIVX developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef EMAILINPUTDIALOG_H
#define EMAILINPUTDIALOG_H

#include <QDialog>

namespace Ui {
class EmailInputDialog;
}

class EmailInputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EmailInputDialog(QWidget *parent = nullptr);
    ~EmailInputDialog();

    void setText(QString title = "", QString message = "", QString okBtnText = "", QString cancelBtnText = "");
    const std::string getEmail() const;
    const std::string getUrl() const;
    const std::string getUsername() const;
    const std::string getPassword() const;

    bool isOk = false;
private:
    Ui::EmailInputDialog *ui;
};

#endif // EMAILINPUTDIALOG_H
