#include "gui.h"


ClasterWindow::ClasterWindow(QWidget* parent):QWidget(parent) {

	QGridLayout* gl = new QGridLayout(this);

	originImg = new QLabel("Original Image",this);
	originImg->resize(700, 700);
	originImg->setFrameShape(QFrame::Panel);
	originImg->setLineWidth(3);

	clastImg = new QLabel("ClastImage",this);
	clastImg->resize(700, 700);
	clastImg->setFrameShape(QFrame::Panel);
	clastImg->setLineWidth(3);

	kMeanBtn = new QPushButton("KMean",this);
	DbBtn = new QPushButton("DBSCAN", this);
	loadImg = new QPushButton("Load Image", this);
	saveImg = new QPushButton("Save Image", this);

	connect(loadImg, SIGNAL(clicked()), this, SLOT(openImage()));
	connect(DbBtn, SIGNAL(clicked()), this, SLOT(setDbParam()));
	connect(kMeanBtn, SIGNAL(clicked()), this, SLOT(setKmeansParam()));
	connect(saveImg, SIGNAL(clicked()), this, SLOT(saveImage()));

	gl->addWidget(originImg, 0, 0, 2, 2);
	gl->addWidget(clastImg, 0, 2, 2, 2); 
	gl->addWidget(kMeanBtn, 2, 0, 1, 1);
	gl->addWidget(DbBtn, 2, 1, 1, 1);
	gl->addWidget(loadImg, 2, 2, 1, 1);
	gl->addWidget(saveImg, 2, 3, 1, 1);
	this->setFixedSize(1400, 800);

};


void ClasterWindow::openImage() {
	QString str = "JPEG (*.jpg *.jpeg)";
	QString fileName = QFileDialog::getOpenFileName(
		this, "OpenFile", "C://Urers//",
		tr("All files (*.*);;JPEG (*.jpg *.jpeg);;TIFF (*.tif)"),
		&str
	);
	std::wstring wstr = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(fileName.toStdString());
	std::filesystem::path p1(wstr);
	img = Image(p1.string());

	if (!img.getIm().empty()) {
		cv::cvtColor(img.getIm(), img.getIm(), cv::COLOR_BGR2RGB);
		QImage img1((uchar*)img.getIm().data, img.getIm().cols,
			img.getIm().rows, img.getIm().step, QImage::Format_RGB888);

		img1 = img1.scaled(originImg->width() - originImg->lineWidth() * 2,
			originImg->height() - originImg->lineWidth() * 2);

		QPixmap pixmap = QPixmap::fromImage(img1);
		originImg->setPixmap(pixmap);
	}

}

void ClasterWindow::setDbParam() {
	DBSCANSetting setWindow(this);
	setWindow.show();
}


void ClasterWindow::DBSCAN(const DoubleStrings& S) {

	if (!img.getIm().empty()) {
		cv::Mat dbscanIm = img.DBSCAN(S.rad.toInt(), S.minQuan.toInt());
		if (!dbscanIm.empty()) {
			QImage clasterIm((uchar*)dbscanIm.data, dbscanIm.cols,
				dbscanIm.rows, dbscanIm.step, QImage::Format_RGB888);
			clasterIm = clasterIm.scaled(clastImg->width() - clastImg->lineWidth() * 2,
				clastImg->height() - clastImg->lineWidth() * 2);
			QPixmap pixmap = QPixmap::fromImage(clasterIm);
			clastImg->setPixmap(pixmap);
		}
	}

}


void ClasterWindow::setKmeansParam() {
	KMeansSetting setWindow(this);
	setWindow.show();
}


void ClasterWindow::KMeans(const QString& param) {

	if (param != '0' && !img.getIm().empty()) {
		cv::Mat kMeansIm = img.kMeans(param.toInt());
		if (!kMeansIm.empty()) {
			QImage clasterIm((uchar*)kMeansIm.data, kMeansIm.cols,
				kMeansIm.rows, kMeansIm.step, QImage::Format_RGB888);
			clasterIm = clasterIm.scaled(clastImg->width() - clastImg->lineWidth() * 2,
				clastImg->height() - clastImg->lineWidth() * 2);
			QPixmap pixmap = QPixmap::fromImage(clasterIm);
			clastImg->setPixmap(pixmap);
		}
	}
}


void ClasterWindow::saveImage() {

	clastImg->pixmap().save(QFileDialog::getSaveFileName(0, tr("Save image"),
		QDir::currentPath(), "*.jpg"));

}




DBSCANSetting::DBSCANSetting(QWidget* parent) :QWidget(parent) {
	QGridLayout* gl = new QGridLayout(this);

	radius = new QLineEdit(this);
	radius->setValidator(new QRegularExpressionValidator(
		QRegularExpression("[0-9]*"), radius));
	radius->setPlaceholderText("input radius");
	minQuantity = new QLineEdit(this);
	minQuantity->setValidator(new QRegularExpressionValidator(
		QRegularExpression("[0-9]*"), minQuantity));
	minQuantity->setPlaceholderText("input min quantuty");

	apply = new QPushButton("apply", this);
	cancel = new QPushButton("cancel", this);

	gl->addWidget(radius, 0, 0);
	gl->addWidget(minQuantity, 0, 1);
	gl->addWidget(apply, 1, 0);
	gl->addWidget(cancel, 1, 1);

	dial=new QDialog(this, Qt::Window);
	dial->setSizeGripEnabled(true);
	dial->setLayout(gl);
	connect(cancel, SIGNAL(clicked()), this, SLOT(exitSlot()));
	connect(apply, SIGNAL(clicked()), this, SLOT(buttonClicked()));
	connect(this, SIGNAL(sendMessage(DoubleStrings)),
			parent, SLOT(DBSCAN(DoubleStrings)));
	dial->exec();

}

KMeansSetting::KMeansSetting(QWidget* parent) :QWidget(parent) {
	QGridLayout* gl = new QGridLayout(this);
	numOfClasters = new QLineEdit(this);
	numOfClasters->setValidator(new QRegularExpressionValidator(
		QRegularExpression("[0-9]*"), numOfClasters));
	numOfClasters->setPlaceholderText("input number of clasters");

	apply = new QPushButton("apply", this);
	cancel = new QPushButton("cancel", this);

	gl->addWidget(numOfClasters, 0, 0);
	gl->addWidget(apply, 0, 3);
	gl->addWidget(cancel, 0, 4);

	dial = new QDialog(this, Qt::Window);
	dial->setSizeGripEnabled(true);
	dial->setLayout(gl);
	connect(cancel, SIGNAL(clicked()), this, SLOT(exitSlot()));
	connect(apply, SIGNAL(clicked()), this, SLOT(buttonClicked()));
	connect(this, SIGNAL(sendMessage(QString)),
		parent, SLOT(KMeans(QString)));

	dial->exec();
}
