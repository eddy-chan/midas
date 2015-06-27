#ifndef MIDASGL_H_
#define MIDASGL_H_

#include <QGLWidget>
#include <QTimer>
#include <vector>
#include "Point.h"
#include "Vector.h"
#include "TexShape.h"

#define PI 3.14159265

class MidasGL : public QGLWidget {
	Q_OBJECT
public:
	MidasGL(vector< vector<Point> > polygons, vector<TexShape>, QTimer *timer, QWidget *parent = 0);
	~MidasGL();

	QSize minimumSizeHint() const;
	QSize sizeHint() const;
	int frames;

public slots:
	void setWireframe(int state);
	void setSpeed(int speed);
	void setXRotation(int angle);
	void setYRotation(int angle);
	void setZRotation(int angle);
	void setXRotation(double angle);
	void setYRotation(double angle);
	void setZRotation(double angle);
	void setXTrans(int xTrans);
	void setYTrans(int yTrans);
	void setZTrans(int zTrans);
	void setEyeX(double eyeX);
	void setEyeY(double eyeY);
	void setEyeZ(double eyeZ);
	void setEyeX(int eyeX);
	void setEyeY(int eyeY);
	void setEyeZ(int eyeZ);
	void setCenterX(double centerX);
	void setCenterY(double centerY);
	void setCenterZ(double centerZ);
	void setCenterX(int centerX);
	void setCenterY(int centerY);
	void setCenterZ(int centerZ);
	void setUpX(int upX);
	void setUpY(int upY);
	void setUpZ(int upZ);
	void flight();

signals:
	void xRotationChanged(int angle);
	void yRotationChanged(int angle);
	void zRotationChanged(int angle);
	void xRotationChanged(double angle);
	void yRotationChanged(double angle);
	void zRotationChanged(double angle);
	void xEyeChanged(int eyex);
	void yEyeChanged(int eyey);
	void zEyeChanged(int eyez);
	void xCenterChanged(int centerx);
	void yCenterChanged(int centery);
	void zCenterChanged(int centerz);
	void speedChanged(int speed);

protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);

private:
	void initPos();
	void normalizeAngle(int *angle);
	void realign();
	void createPolygonLists();
	void flightView();
	void mouseView();

	GLuint polygonLists;
	GLuint texturedPolygonLists;
	GLuint* textureLists;
	vector< vector<Point> > polygons;
	vector<TexShape> texturedPolygons;
	int xRot;
	int yRot;
	int zRot;
	int yRotRoll;
	int zRotRoll;
	double xTrans;
	double yTrans;
	double zTrans;
	double eyeX;
	double eyeY;
	double eyeZ;
	double centerX;
	double centerY;
	double centerZ;
	double upX;
	double upY;
	double upZ;
//	Vector velocity;
	double speed;
	int width;
	int height;
	int elapsed;
	bool mouseMoving;
	bool flightMode;
	QTimer *timer;
	QPoint lastPos;
	QColor trolltechGreen;
	QColor trolltechPurple;
};

#endif /*MIDASGL_H_*/
