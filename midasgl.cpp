#include <QtGui>
#include <QtOpenGL>
#include <GL/glu.h>
#include <iostream>
#include <boost/foreach.hpp>
#include <cmath>
#include "TexShape.h"

#include "midasgl.h"

MidasGL :: MidasGL(vector< vector<Point> > p, vector<TexShape> tp, QTimer *timer, QWidget *parent)
	: QGLWidget(parent), polygons(p), texturedPolygons(tp) {
	trolltechGreen = QColor::fromCmykF(0.40,0.0,1.0,0.0);
	trolltechPurple = QColor::fromCmykF(0.39,0.39,0.0,0.0);
	mouseMoving = false;
	flightMode = true;
	this->timer = timer;
	frames = 0;
	initPos();
//	for (int i = 0; i < 10; i++) {
//
//	}
}

MidasGL::~MidasGL() {
	makeCurrent();
	glDeleteLists(polygonLists, polygons.size());
}

void MidasGL::initPos() {
	setXRotation(0);
	setYRotation(0);
	setZRotation(0);
	setEyeX(0);
	setEyeY(0);
	setEyeZ(350);
	setCenterX(0);
	setCenterY(0);
	setCenterZ(-100);
	yRotRoll = 0;
	zRotRoll = 0;
	xTrans = 0;
	yTrans = 0;
	zTrans = 0;
	upX = 0;
	upY = 1;
	upZ = 0;
	speed = 0.1;

}

QSize MidasGL::minimumSizeHint() const {
	return QSize(400,400);
}

QSize MidasGL::sizeHint() const {
	return QSize(400,400);
}

void MidasGL::setWireframe(int state) {
	switch (state) {
	case Qt::Unchecked:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case Qt::Checked:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	}
	updateGL();
}

void MidasGL::setSpeed(int speed) {
	this->speed = (double)speed / 10.0;
	emit speedChanged((int)(speed * 10));
}

void MidasGL::setXRotation(double angle) {
//	normalizeAngle(&angle);
	if (angle != xRot) {
		xRot = angle;
		emit xRotationChanged(angle);
//		setCenterY(eyeY + -200 * sin(xRot * (PI / 180.0)));
		updateGL();
	}
}
void MidasGL::setYRotation(double angle) {
//	normalizeAngle(&angle);
	if (angle != yRot) {
		yRot = angle;
		emit yRotationChanged(angle);
		setCenterX(eyeX + 200 * sin(yRot * (PI / 180.0)));
		setCenterZ(eyeZ + -200 * cos(yRot * (PI / 180.0)));
		//updateGL();
	}
}

void MidasGL::setZRotation(double angle) {
//	normalizeAngle(&angle);
	if (angle != zRot) {
		zRot = angle;
		emit zRotationChanged(angle);
		//updateGL();
	}
}
void MidasGL::setXRotation(int angle) {
	setXRotation((double)angle);
	emit xRotationChanged(angle);
}
void MidasGL::setYRotation(int angle) {
	setYRotation((double)angle);
	emit yRotationChanged(angle);
}
void MidasGL::setZRotation(int angle) {
	setZRotation((double)angle);
	emit yRotationChanged(angle);
}

void MidasGL::setXTrans(int xTrans) {
	this->xTrans = xTrans;
	//updateGL();
}
void MidasGL::setYTrans(int yTrans) {
	this->yTrans = yTrans;
	//updateGL();
}
void MidasGL::setZTrans(int zTrans) {
	this->zTrans = zTrans;
	if (!flightMode) updateGL();
}
void MidasGL::setEyeX(double eyeX) {
	this->eyeX = eyeX;
	emit xEyeChanged((int)eyeX);
	//updateGL();
}
void MidasGL::setEyeY(double eyeY) {
	this->eyeY = eyeY;
	emit yEyeChanged((int)eyeY);
	//updateGL();
}
void MidasGL::setEyeZ(double eyeZ) {
	this->eyeZ = eyeZ;
	emit zEyeChanged((int)eyeZ);
	if (!flightMode) updateGL();
}
void MidasGL::setEyeX(int eyeX) {
	setEyeX((double)eyeX);
}
void MidasGL::setEyeY(int eyeY) {
	setEyeY((double)eyeY);
}
void MidasGL::setEyeZ(int eyeZ) {
	setEyeZ((double)eyeZ);
}
void MidasGL::setCenterX(double centerX) {
	this->centerX = centerX;
	emit xCenterChanged((int)centerX);
	//updateGL();
}
void MidasGL::setCenterY(double centerY) {
	this->centerY = centerY;
	emit yCenterChanged((int)centerY);
	//updateGL();
}
void MidasGL::setCenterZ(double centerZ) {
	this->centerZ = centerZ;
	emit zCenterChanged((int)centerZ);
	//updateGL();
}
void MidasGL::setCenterX(int centerX) {
	setCenterX((double)centerX);
}
void MidasGL::setCenterY(int centerY) {
	setCenterY((double)centerY);
}
void MidasGL::setCenterZ(int centerZ) {
	setCenterZ((double)centerZ);
}
void MidasGL::setUpX(int upX) {
	this->upX = upX;
	//updateGL();
}
void MidasGL::setUpY(int upY) {
	this->upY = upY;
	//updateGL();
}
void MidasGL::setUpZ(int upZ) {
	this->upZ = upZ;
	//updateGL();
}

void MidasGL::flight() {
	this->timer->stop();
	if (flightMode) {
		elapsed = (elapsed + qobject_cast<QTimer*>(sender())->interval());
	//	cout << "elapsed: " << elapsed << endl;
	//	cout << "speed: " << speed << endl;
		realign();
		if (abs(zRot/16) > 1) { // rotate left/right a bit when rolling
			if (zRot > 1)
				setYRotation(yRot + log((double)zRot/32));
			else
				setYRotation(yRot - log((double)abs(zRot)/32));
		}
		if (abs(xRot/16) > 1) { // move the center a bit when mouse cause x rotation
			setCenterY(eyeY - xRot/2);
		}
		Vector velocity = Point(centerX, centerY, centerZ) - Point(eyeX, eyeY, eyeZ);
		velocity.normalize();
	//	cout << "direction: " << velocity << endl;
	//	cout << "velocity: " << velocity*speed << endl;
		double eyeXMove = eyeX + velocity.x()*speed;
		double eyeYMove = eyeY + velocity.y()*speed;
		double eyeZMove = eyeZ + velocity.z()*speed;
		setEyeX(eyeXMove);
		setEyeY(eyeYMove);
		setEyeZ(eyeZMove);
		setCenterX(centerX + velocity.x()*speed);
		setCenterY(centerY + velocity.y()*speed);
		setCenterZ(centerZ + velocity.z()*speed);
	}
	updateGL();
	frames++;
	this->timer->start();
}

void MidasGL::realign() {
	if (!mouseMoving && flightMode) {
		if (zRot != 0) {
			cout << "realigning zRot...\n";
			int zRotBack = (int)(.8 * (double)zRot);
			setZRotation(zRotBack);
			cout << "zRot: " << zRot << endl;
		}
		if (xRot != 0) {
//			cout << "realigning xRot...\n";
//			int xRotBack = (int)(.8 * (double)xRot);
//			setXRotation(xRotBack);
//			cout << "xRot: " << xRot << endl;
			setXRotation(0);
		}
		if (centerY != eyeY) {
			// if view is not leveled
			cout << "realigning centerY...\n";
			cout << "centerY: " << centerY << " eyeY: " << eyeY << endl;
			double yLevelBack = .2 * (centerY - eyeY);
			setCenterY(centerY - yLevelBack);
			if (abs(centerY - eyeY) <= 1)
				setCenterY(eyeY);
		}
	}
}

void MidasGL::initializeGL() {

	qglClearColor(trolltechPurple.dark());
	createPolygonLists();

//	const GLfloat red[4] = {0.6f, 0.f, 0.f, 1.f};
//	const GLfloat blue[4] = {0.f, 0.f, 0.4f, 1.f };
//	const GLfloat green[4] = {0.f, 0.5f, 0.f, 1.f };
	const GLfloat white[4] = {0.9f, 0.9f, 0.9f, 0.9f};
//	const GLfloat LightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	const GLfloat position[4] = { 0.f, 5.f, 20.f, 1.f };

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	glLightfv( GL_LIGHT1, GL_AMBIENT, white);
	glLightfv( GL_LIGHT1, GL_DIFFUSE, white);
	glLightfv( GL_LIGHT1, GL_SPECULAR, white);
	glLightfv( GL_LIGHT1, GL_POSITION, position);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 2.f);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.f);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHT1);
 	glEnable(GL_TEXTURE_2D);
}

void MidasGL::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	if (flightMode) {
		flightView();
	} else {
		mouseView();
	}

//	const GLfloat position[4] = { eyeX, eyeY, eyeZ, 0.f };
//	glLightfv( GL_LIGHT1, GL_POSITION, position);

	glListBase(polygonLists);
	GLuint lists[polygons.size()];
	for (unsigned int i = 0; i < polygons.size(); i++) {
		lists[i] = i;
	}
	glCallLists((GLsizei)polygons.size(), GL_UNSIGNED_INT, lists);

//	glListBase(texturedPolygonLists);
//	unsigned int tsize = 0;
//	BOOST_FOREACH(TexShape ts, texturedPolygons) {
//		tsize += ts.shape.size();
//	}
//	GLuint tlists[tsize];
//	for (unsigned int i = 0; i < tsize; i++) {
//		tlists[i] = i;
//	}
//	glCallLists((GLsizei)tsize, GL_UNSIGNED_INT, tlists);

	int base = texturedPolygonLists;
	for (int i = 0; i < texturedPolygons.size(); i++) {
		glBindTexture(GL_TEXTURE_2D, textureLists[i]);
		glListBase(base);
		GLuint lists[texturedPolygons[i].shape.size()];
		for (unsigned int j = 0; j < texturedPolygons[i].shape.size(); j++)
			lists[j] = j;
		glCallLists((GLsizei)texturedPolygons[i].shape.size(), GL_UNSIGNED_INT, lists);
		base += texturedPolygons[i].shape.size();
	}

}

void MidasGL::mouseView() {
	glRotated(xRot/16.0, 1.0,0.0,0.0); // for mouse view
	glRotated(yRot/16.0, 0.0,1.0,0.0); //
	glRotated(zRot/16.0,0.0,0.0,1.0);  //
}

void MidasGL::flightView() {
	glTranslated(xTrans, yTrans, zTrans);
//	glRotated(xRot/16, 1.0,0.0,0.0); // for mouse view
//	glRotated(yRot, 0.0,1.0,0.0); //
	glRotated(zRot/16,0.0,0.0,1.0);  //
	gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);

}

void MidasGL::resizeGL(int width, int height) {
	this->width = width;
	this->height = height;
	int side = qMin(width, height);
	glViewport((width-side)/2, (height-side)/2,side,side);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (!flightMode) {
		glOrtho(-100.0, +100.0, -100.0, +100.0, -250.0, +250.0);
	} else {
		gluPerspective(45, width/height, 1, 550);
	}
	glMatrixMode(GL_MODELVIEW);
}

void MidasGL::mousePressEvent(QMouseEvent *event) {
	mouseMoving = true;
	lastPos = event->pos();
#if QT_VERSION < 0x040600
	if (event->buttons() & Qt::MiddleButton) {
#else
	if (event->buttons() & Qt::MidButton) {
#endif
		flightMode ^= true;
		cout << "Flight mode: " << flightMode << endl;
		if (flightMode) {
			initPos();
		}
		resizeGL(this->width, this->height);
		updateGL();
	}
}

void MidasGL::mouseReleaseEvent(QMouseEvent *event) {
	event->isAccepted();
	mouseMoving = false;
}

void MidasGL::wheelEvent(QWheelEvent *event) {
	int numDegrees = event->delta() / 8;
	int numSteps = numDegrees/15;
	double speedDelta = (double)numSteps / 10.0;
	speed += speedDelta;
	if (speed < 0)
		speed = 0;
	else if (speed > 5)
		speed = 5;
	emit speedChanged((int)(speed*10));
}

void MidasGL::mouseMoveEvent(QMouseEvent *event) {
	mouseMoving = true;
	int dx = event->x() - lastPos.x();
	int dy = event->y() - lastPos.y();

	if (event->buttons() & Qt::LeftButton) {
		if (flightMode) {
			double xRotation = xRot + dy;
			if (xRotation < -1440) xRotation = -1440;
			if (xRotation > 1440) xRotation = 1440;
			setXRotation(xRotation);
			//setYRotation(yRot + dx);

	//		int zRotate = zRot + 2 * dx;
	//		if (zRotate > 720) zRotate = 720;
	//		if (zRotate < -720) zRotate = -720;
	//		setZRotation(zRotate);
	//		zRotRoll += 2 * dx;
		} else {
			setXRotation(xRot + 8 * dy);
			setYRotation(yRot + 8 * dx);
		}
	}
	if (event->buttons() & Qt::RightButton) {
		if (flightMode) {
	//		int yRotate = yRot + dx;
	//		if (yRotate > 45) yRotate = 45;
	//		if (yRotate < -45) yRotate = -45;
	//		setYRotation(yRotate);
	//		if (yRot + dx <= 45 && yRot + dx >= -45)
	//			yRotRoll += dx;
			int zRotate = zRot + 2 * dx;
			if (zRotate > 720) zRotate = 720;
			if (zRotate < -720) zRotate = -720;
			setZRotation(zRotate);
			zRotRoll += 2 * dx;
		} else {
			setXRotation(xRot + 8 * dy);
			setZRotation(zRot + 8 * dx);
		}
	}
	cout << "xRot: " << xRot << endl;
	cout << "yRot: " << yRot << endl;
	cout << "zRot: " << zRot << endl;

	lastPos = event->pos();
}

void MidasGL::createPolygonLists() {
	polygonLists = glGenLists(polygons.size());
	for (unsigned int i = 0; i < polygons.size(); i++) {
		glNewList(polygonLists+i, GL_COMPILE);
		glBegin(GL_POLYGON);
		vector<Point> polygon = polygons[i];
		Vector vnorm;
		if (polygon.size() > 2) {
			Vector v1 = polygon.back() - polygon[0];
			Vector v2 = polygon[1] - polygon[0];
			vnorm = cross(v1, v2);
//			Vector vorigin = Point(0,0,0) - polygon[0];
//			if (dot(vnorm, vorigin) >= 0.0)
//				vnorm = -1*vnorm;
			vnorm.normalize();
			glNormal3f(vnorm.x(), vnorm.y(), vnorm.z());
		}
		BOOST_FOREACH(Point p, polygon) {
			GLubyte r = p.r(), g = p.g(), b = p.b(), a = p.a();
			glColor4ub(r,g,b,a);
			//glColor4f(p.r(), p.g(), p.b(), p.a());
			glVertex3f(p.x(), p.y(), p.z());
		}
		glEnd();
		glFlush();
		glEndList();
	}

	// Create texture handles.
	textureLists = new GLuint[texturedPolygons.size()];

	for (unsigned int i = 0; i < texturedPolygons.size(); i++) {
		// Load the image using Qt (can be done countless other ways too).
		QImage image(string("textures/").append(texturedPolygons[i].texturefile).c_str());
		QImage ogl = QGLWidget::convertToGLFormat(image);
		//cout << texturedPolygons[i].texturefile << endl;
		//cout << ogl.width() << " " << ogl.height() << endl;

		glGenTextures(1, &textureLists[i]);
		// Bind the texture.
		glBindTexture(GL_TEXTURE_2D, textureLists[i]);

		// Setup the texture parameters (important!).
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		// Upload the texture to OpenGL.
		glTexImage2D(GL_TEXTURE_2D,
					 0,
					 GL_RGBA,
					 ogl.width(),
					 ogl.height(),
					 0,
					 GL_RGBA,
					 GL_UNSIGNED_BYTE,
					 ogl.bits());

		// Unbind the texture.
		glBindTexture(GL_TEXTURE_2D, 0);
	}
//	for (int a = 0; a < texturedPolygons.size(); a++) {
//		cout << textureLists[a] << endl;
//	}
//	exit(1);

	int tsize = 0;
	BOOST_FOREACH(TexShape ts, texturedPolygons) {
		tsize += ts.shape.size();
	}
	texturedPolygonLists = glGenLists(tsize);
	int listIndex = 0;
	for (unsigned int j = 0; j < texturedPolygons.size(); j++) {
		TexShape ts = texturedPolygons[j];
		GLint name;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &name);
		//cout << "current texture: " << name << endl;
		for(unsigned int i = 0; i < ts.shape.size(); i++) {
			glNewList(texturedPolygonLists+listIndex, GL_COMPILE);
			glBegin(GL_QUADS);

			vector<Point> polygon = ts.shape[i];
			vector<Point> texture = ts.texture[i];

			Vector vnorm;
			if (polygon.size() > 2) {
				Vector v1 = polygon.back() - polygon[0];
				Vector v2 = polygon[1] - polygon[0];
				vnorm = cross(v1, v2);
				vnorm.normalize();
				glNormal3f(vnorm.x(), vnorm.y(), vnorm.z());
			}
			for (unsigned int n = 0; n < polygon.size(); n++) {
				Point p = polygon[n];
				Point t = texture[n];
				GLubyte r = p.r(), g = p.g(), b = p.b(), a = p.a();
				GLfloat u = t.x(), v = t.y();
				glColor4ub(r,g,b,a);
				glTexCoord2f(u, v);
				glVertex3f(p.x(), p.y(), p.z());
			}
			glEnd();
			glFlush();
			glEndList();
			listIndex++;
		}
	}
	//cout << "tsize: " << tsize << endl;
	//cout << "listIndex: " << listIndex << endl;

}

void MidasGL::normalizeAngle(int *angle)
{
	while (*angle < 0)
		*angle += 360*16;
	while (*angle > 360*16)
		*angle -= 360*16;
}
