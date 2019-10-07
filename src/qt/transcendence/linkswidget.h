// Copyright (c) 2019 The PIVX developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef LINKSWIDGET_H
#define LINKSWIDGET_H

#include <QDialog>

namespace Ui {
class LinksWidget;
}

class LinksWidget : public QDialog
{
    Q_OBJECT

public:
    explicit LinksWidget(QWidget *parent = nullptr);
    ~LinksWidget();

    void showEvent(QShowEvent *event) override;

public slots:
   void windowResizeEvent(QResizeEvent* event);
   void setSection(int num);
private slots:
    void onFaq1Clicked();
    void onFaq2Clicked();
    void onFaq3Clicked();
    void onFaq4Clicked();
    void onFaq5Clicked();
    void onFaq6Clicked();
    void onFaq7Clicked();
    void onFaq8Clicked();
    void onFaq9Clicked();
    void onFaq10Clicked();
private:
    Ui::LinksWidget *ui;
    int pos = 0;

    std::vector<QPushButton*> getButtons();
};

#endif // LINKSWIDGET_H
