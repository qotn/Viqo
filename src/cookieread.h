#ifndef COOKIEREAD_H
#define COOKIEREAD_H

#include <QObject>
#include <QtSql/QtSql>
#include <QMessageBox>

class MainWindow;

class CookieRead : public QObject
{
	Q_OBJECT
public:
	explicit CookieRead(MainWindow* mwin);
	~CookieRead();
    QString getUserSession(QString cookie_name);
	void close();

private:
	QSqlDatabase db;
	MainWindow* mwin;

signals:

public slots:

};

#endif // COOKIEREAD_H
