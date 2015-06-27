#ifndef MIDASVIEW_H
#define MIDASVIEW_H

#include <QWidget>
#include <QPushButton>
#include <QSpinBox>
#include <QCheckBox>
#include <vector>
#include <string>
#include "midasgl.h"
#include "Point.h"
#include "commands.h"

class QSlider;

class MidasW : public QWidget {
	Q_OBJECT
public:
	MidasW(polygonList polygons, texturedPolygonList texturedPolygons);

public slots:
	void checkFPS();

private:
	QSlider *createSlider();
	QSpinBox *createSpinBox(int, bool);
	QTimer *timer;
	void createInterface();
	polygonList polygons;

	MidasGL *glWidget;
};

#endif // MIDASVIEW_H
