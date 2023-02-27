#pragma once
#include <QtWidgets>
#include "image.h"
#include <filesystem>
#include <codecvt>
#include <locale>


struct DoubleStrings {

	DoubleStrings(QString rad, QString minQuan) : rad(rad), minQuan(minQuan) {};
	QString rad;
	QString minQuan;

};

class ClasterWindow:public QWidget {
	Q_OBJECT

public:
	ClasterWindow(QWidget* parent=0);

public slots:
	void openImage();
	void setDbParam();
	void DBSCAN(const DoubleStrings& S);
	void setKmeansParam();
	void KMeans(const QString& param);
	void saveImage();
private:
	QLabel* originImg;
	QLabel* clastImg;
	QPushButton* kMeanBtn;
	QPushButton* DbBtn;
	QPushButton* loadImg;
	QPushButton* saveImg;
	Image img;
};



class DBSCANSetting : public QWidget {
	Q_OBJECT

public:
	DBSCANSetting(QWidget* parent = 0);

public slots:
	void exitSlot()
	{
		dial->close();
	}

	void buttonClicked() {
		emit sendMessage(DoubleStrings(radius->text(),minQuantity->text()));
		exitSlot();
	}

signals:
	void sendMessage(const DoubleStrings& msg);


private:
	QLineEdit* radius;
	QLineEdit* minQuantity;
	QPushButton* apply;
	QPushButton* cancel;
	QDialog* dial;

};


class KMeansSetting : public QWidget {
	Q_OBJECT
public:
	KMeansSetting(QWidget* parent = 0);


public slots:
	void exitSlot()
	{
		dial->close();
	}

	void buttonClicked() {
		emit sendMessage(numOfClasters->text());
		exitSlot();
	}

signals:
	void sendMessage(const QString& kmean);


private:
	QPushButton* apply;
	QPushButton* cancel;
	QLineEdit* numOfClasters;
	QDialog* dial;

};
