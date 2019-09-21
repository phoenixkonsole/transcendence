// Copyright (c) 2019 The PIVX developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef WELCOMECONTENTWIDGET_H
#define WELCOMECONTENTWIDGET_H

#include <QWidget>
#include "qt/materialui/qtmaterialraisedbutton.h"
#include <QDialog>
#include "qt/materialui/qtmaterialraisedbutton.h"

class OptionsModel;

namespace Ui {
class WelcomeContentWidget;
}

class WelcomeContentWidget : public QDialog
{
    Q_OBJECT

public:
    explicit WelcomeContentWidget(QWidget *parent = nullptr);
    ~WelcomeContentWidget();
    int pos = 0;
    bool isOk = false;

    void setModel(OptionsModel *model);
    void checkLanguage();

signals:
    void onLanguageSelected();

public slots:
    void onNextClicked();
    void onBackClicked();
    void onSkipClicked();

private:
    Ui::WelcomeContentWidget *ui;
    QtMaterialRaisedButton *icConfirm1;
    QtMaterialRaisedButton *icConfirm2;
    QtMaterialRaisedButton *icConfirm3;
    QtMaterialRaisedButton *icConfirm4;
    QtMaterialRaisedButton *backButton;
    QtMaterialRaisedButton *nextButton;

    OptionsModel *model;

    void initLanguages();
};

#endif // WELCOMECONTENTWIDGET_H
