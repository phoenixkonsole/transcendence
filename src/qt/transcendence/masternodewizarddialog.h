// Copyright (c) 2019 The PIVX developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef MASTERNODEWIZARDDIALOG_H
#define MASTERNODEWIZARDDIALOG_H

#include <QDialog>
#include "walletmodel.h"
#include "qt/transcendence/snackbar.h"
#include "masternodeconfig.h"
#include "qt/materialui/qtmaterialraisedbutton.h"

class WalletModel;

namespace Ui {
class MasterNodeWizardDialog;
class QtMaterialRaisedButton;
}

class MasterNodeWizardDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MasterNodeWizardDialog(WalletModel *walletMode, QWidget *parent = nullptr);
    ~MasterNodeWizardDialog();

    QString returnStr = "";
    bool isOk = false;
    CMasternodeConfig::CMasternodeEntry* mnEntry = nullptr;

private slots:
    void onNextClicked();
    void onBackClicked();
private:
    Ui::MasterNodeWizardDialog *ui;
    QtMaterialRaisedButton* icConfirm1;
    QtMaterialRaisedButton* icConfirm3;
    QtMaterialRaisedButton* icConfirm4;
    SnackBar *snackBar = nullptr;
    int pos = 0;

    WalletModel *walletModel = nullptr;
    bool createMN();
    // Process WalletModel::SendCoinsReturn and generate a pair consisting
    // of a message and message flags for use in emit message().
    // Additional parameter msgArg can be used via .arg(msgArg).
    void processSendCoinsReturn(const WalletModel::SendCoinsReturn& sendCoinsReturn, const QString& msgArg = QString(), bool fPrepare = false);
    void inform(QString text);
    void initBtn(std::initializer_list<QtMaterialRaisedButton*> args);
};

#endif // MASTERNODEWIZARDDIALOG_H
