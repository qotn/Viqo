#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QJsonObject>
#include <QStandardPaths>
#include <QFile>

#include "ui_mainwindow.h"

class MainWindow;

class Settings : public QObject
{
  Q_OBJECT
public:
  explicit Settings(MainWindow* mwin, Ui::MainWindow* ui, QObject* parent = 0);

  void saveStatus();
  void loadStatus();
  void oldLoad();

  void saveSettings();
  void loadSettings();

  // main window
  bool getIs184() const;

  // account
  QString getUserMail() const;
  void setUserMail(QString value);

  QString getUserPass() const;
  void setUserPass(QString value);

  int getLoginWay() const;
  void setLoginWay(int value);

  QString getUserSession() const;
  void setUserSession(QString value);

  QString getCookieFile() const;
  void setCookieFile(QString value);

  // comment
  bool getOwnerComment() const;
  void setOwnerComment(bool value);

  bool getDispNG() const;
  void setDispNG(bool value);

signals:

public slots:

private slots:

private:
  int loginWay;
  QString userSession;
  QString cookieFile;

  QString userMail;
  QString userPass;

  bool ownerComment;
  bool dispNG;

  MainWindow* mwin;
  Ui::MainWindow *ui;
};

#endif // SETTINGS_H