#include <QtGui>
#ifdef WIN32
	#include <QHBoxLayout>
	#include <QDial>
	#include <QLabel>
	#include <QLCDNumber>
	#include <QSlider>
#endif
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "Point.h"

#include "midasgl.h"
#include "midasview.h"
#include "commands.h"

using namespace std;

MidasW::MidasW(polygonList polygons, texturedPolygonList texturedPolygons) {
	this->polygons = polygons;
	timer = new QTimer(this);
	glWidget = new MidasGL(polygons, texturedPolygons, timer, this);
	createInterface();
	connect(timer, SIGNAL(timeout()), glWidget, SLOT(flight()));
	timer->start(25);
}

void MidasW::createInterface() {

	QHBoxLayout *mainLayout = new QHBoxLayout;
	QGridLayout *controlLayout = new QGridLayout;
	QVBoxLayout *controlPanel = new QVBoxLayout;


	QSpinBox *eyeX = createSpinBox(0, true);
	QSpinBox *eyeY = createSpinBox(0, true);
	QSpinBox *eyeZ = createSpinBox(0, false);
//	QSpinBox *upX = createSpinBox(0);
//	QSpinBox *upY = createSpinBox(1);
//	QSpinBox *upZ = createSpinBox(0);
	QSpinBox *centerX = createSpinBox(0, true);
	QSpinBox *centerY = createSpinBox(0, true);
	QSpinBox *centerZ = createSpinBox(-200, true);
	QSpinBox *transX = createSpinBox(0, false);
	QSpinBox *transY = createSpinBox(0, false);
	QSpinBox *transZ = createSpinBox(0, false);

	QDial *speedDial = new QDial();
	speedDial->setMaximum(50);
	speedDial->setMinimum(0);
	speedDial->setWrapping(false);
	speedDial->setNotchesVisible(true);
	speedDial->setSingleStep(1);
	speedDial->setValue(1);
	speedDial->setDisabled(true);


	QCheckBox *wireframe = new QCheckBox("Enable Wireframe");
	wireframe->setCheckState(Qt::Unchecked);

	QLabel *xLabel = new QLabel("x");
	xLabel->setMaximumSize(QSize(10,10));
	QLabel *yLabel = new QLabel("y");
	yLabel->setMaximumSize(QSize(10,10));
	QLabel *zLabel = new QLabel("z");
	zLabel->setMaximumSize(QSize(10,10));
	QLabel *eyeLabel = new QLabel("eye");
	QLabel *centerLabel = new QLabel("center");
//	QLabel *upLabel = new QLabel("up");
	QLabel *transLabel = new QLabel("trans");
	QLabel *speedLabel = new QLabel("speed");
	speedLabel->setAlignment(Qt::AlignHCenter);

	controlLayout->addWidget(xLabel, 0, 1);
	controlLayout->addWidget(yLabel, 0, 2);
	controlLayout->addWidget(zLabel, 0, 3);

	controlLayout->addWidget(eyeLabel, 1, 0);
	controlLayout->addWidget(eyeX, 1, 1);
	controlLayout->addWidget(eyeY, 1, 2);
	controlLayout->addWidget(eyeZ, 1, 3);

	controlLayout->addWidget(centerLabel, 2, 0);
	controlLayout->addWidget(centerX, 2, 1);
	controlLayout->addWidget(centerY, 2, 2);
	controlLayout->addWidget(centerZ, 2, 3);

//	controlLayout->addWidget(upLabel, 3, 0);
//	controlLayout->addWidget(upX, 3, 1);
//	controlLayout->addWidget(upY, 3, 2);
//	controlLayout->addWidget(upZ, 3, 3);

	controlLayout->addWidget(transLabel, 4, 0);
	controlLayout->addWidget(transX, 4, 1);
	controlLayout->addWidget(transY, 4, 2);
	controlLayout->addWidget(transZ, 4, 3);

	QHBoxLayout *speedView = new QHBoxLayout;
	QLCDNumber *speedDisplay = new QLCDNumber(2);
	speedDisplay->display(1);
	speedDisplay->setMaximumSize(QSize(50,100));
	speedDisplay->setSegmentStyle(QLCDNumber::Flat);
	speedView->addWidget(speedDial);
	speedView->addWidget(speedDisplay);

	controlPanel->addLayout(controlLayout);
	controlPanel->addLayout(speedView);
	controlPanel->addWidget(speedLabel);
	controlPanel->addWidget(wireframe);

//	controlLayout->addWidget(xSlider,6,1,1,4);
//	controlLayout->addWidget(ySlider,7,1,1,4);
//	controlLayout->addWidget(zSlider,8,1,1,4);


	mainLayout->addWidget(glWidget);
	mainLayout->addLayout(controlPanel);
	setLayout(mainLayout);

	connect(wireframe, SIGNAL(stateChanged(int)), glWidget, SLOT(setWireframe(int)));

//	connect(eyeX, SIGNAL(valueChanged(int)), glWidget, SLOT(setEyeX(int)));
//	connect(eyeY, SIGNAL(valueChanged(int)), glWidget, SLOT(setEyeY(int)));
//	connect(eyeZ, SIGNAL(valueChanged(int)), glWidget, SLOT(setEyeZ(int)));
	connect(glWidget, SIGNAL(xEyeChanged(int)), eyeX, SLOT(setValue(int)));
	connect(glWidget, SIGNAL(yEyeChanged(int)), eyeY, SLOT(setValue(int)));
	connect(glWidget, SIGNAL(zEyeChanged(int)), eyeZ, SLOT(setValue(int)));

//	connect(centerX, SIGNAL(valueChanged(int)), glWidget, SLOT(setCenterX(int)));
//	connect(centerY, SIGNAL(valueChanged(int)), glWidget, SLOT(setCenterY(int)));
//	connect(centerZ, SIGNAL(valueChanged(int)), glWidget, SLOT(setCenterZ(int)));
	connect(glWidget, SIGNAL(xCenterChanged(int)), centerX, SLOT(setValue(int)));
	connect(glWidget, SIGNAL(yCenterChanged(int)), centerY, SLOT(setValue(int)));
	connect(glWidget, SIGNAL(zCenterChanged(int)), centerZ, SLOT(setValue(int)));

//	connect(upX, SIGNAL(valueChanged(int)), glWidget, SLOT(setUpX(int)));
//	connect(upY, SIGNAL(valueChanged(int)), glWidget, SLOT(setUpY(int)));
//	connect(upZ, SIGNAL(valueChanged(int)), glWidget, SLOT(setUpZ(int)));
	connect(transX, SIGNAL(valueChanged(int)), glWidget, SLOT(setXTrans(int)));
	connect(transY, SIGNAL(valueChanged(int)), glWidget, SLOT(setYTrans(int)));
	connect(transZ, SIGNAL(valueChanged(int)), glWidget, SLOT(setZTrans(int)));

	connect(glWidget, SIGNAL(speedChanged(int)), speedDial, SLOT(setValue(int)));
	//connect(speedDial, SIGNAL(valueChanged(int)), glWidget, SLOT(setSpeed(int)));

	connect(glWidget, SIGNAL(speedChanged(int)), speedDisplay, SLOT(display(int)));
//	connect(xSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setXRotation(int)));
//	connect(ySlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setYRotation(int)));
//	connect(zSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setZRotation(int)));

	QTimer *fpsTimer = new QTimer(this);
	connect(fpsTimer, SIGNAL(timeout()), this, SLOT(checkFPS()));
	fpsTimer->start(1000);

	setWindowTitle(tr("MidasView"));

}

void MidasW::checkFPS() {
//	cout << glWidget->frames << " frames\n";
	glWidget->frames = 0;
}

QSpinBox* MidasW::createSpinBox(int defaultVal, bool isDisabled) {
	QSpinBox *newSB = new QSpinBox();
	newSB->setValue(defaultVal);
	newSB->setRange(-200,200);
	newSB->setAccelerated(true);
	newSB->setDisabled(isDisabled);
	return newSB;
}

QSlider *MidasW::createSlider() {
	QSlider *slider = new QSlider(Qt::Horizontal);
	slider->setRange(0, 360*16);
	slider->setSingleStep(16);
	slider->setPageStep(15*16);
	slider->setTickInterval(15*16);
	slider->setTickPosition(QSlider::TicksRight);
	slider->setMinimumWidth(100);
	return slider;
}


