// Copyright (c) 2019 The PIVX developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef RECEIVEWIDGET_H
#define RECEIVEWIDGET_H

#include "qt/transcendence/pwidget.h"
#include "addresstablemodel.h"
#include "qt/transcendence/furabstractlistitemdelegate.h"
#include "qt/transcendence/addressfilterproxymodel.h"

#include <QSpacerItem>
#include <QWidget>
#include "qt/materialui/qtmaterialraisedbutton.h"
#include <QPixmap>

class TELOSGUI;
class SendCoinsRecipient;

namespace Ui {
class ReceiveWidget;
}

QT_BEGIN_NAMESPACE
class QModelIndex;
QT_END_NAMESPACE

class ReceiveWidget : public PWidget
{
    Q_OBJECT

public:
    explicit ReceiveWidget(TELOSGUI* parent);
    ~ReceiveWidget();

    void loadWalletModel() override;

public slots:
    void onRequestClicked();
    void onMyAddressesClicked();
    void onNewAddressClicked();

private slots:
    void changeTheme(bool isLightTheme, QString &theme) override ;
    void onLabelClicked();
    void onCopyClicked();
    void refreshView(QString refreshAddress = QString());
    void handleAddressClicked(const QModelIndex &index);
private:
    Ui::ReceiveWidget *ui;

    FurAbstractListItemDelegate *delegate;
    AddressTableModel* addressTableModel = nullptr;
    AddressFilterProxyModel *filter = nullptr;

    QSpacerItem *spacer = nullptr;

    // Cached last address
    SendCoinsRecipient *info = nullptr;
    // Cached qr
    QPixmap *qrImage = nullptr;

    void updateQr(QString address);
    void updateLabel();

    bool isShowingDialog = false;

};

#endif // RECEIVEWIDGET_H
