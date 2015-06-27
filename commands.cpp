#include <iostream>
#include <iomanip>
#include <fstream>
#include <assert.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <queue>
#include <boost/regex.hpp>
#include <boost/tokenizer.hpp>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include "commands.h"
#include "Point.h"
#include "Vector.h"
#include "ProjPoint.h"
#include "TransformationWithFrames.h"
#include "TexShape.h"

using namespace std;
using namespace boost;

const double	 pi = 3.1415926535897;
const char polypathstr[] = "poly";
const char polyextstr[] = ".poly";
const char texpathstr[] = "tex";
boost::filesystem::path polypath(polypathstr);
boost::filesystem::path polyext(polyextstr);

Command::~Command() { }

token Command::tokenize_cmd(string command) {
	char_separator<char> seq(" \\");
	return token(command, seq);
}

void Command::writePolygons(polygonList polygons, string filename) {
	boost::filesystem::create_directory(polypathstr);
	ofstream outfile((string(polypathstr) + "/" + filename).c_str());
	//cout << (string(polypathstr) + "/" + filename).c_str() << " created\n";
	BOOST_FOREACH(vector<Point> polygon, polygons) {
		if (polygon.size() < 1)
			continue;
		BOOST_FOREACH(Point p, polygon) {
			outfile << p << " ";
		}
		outfile << endl;
	}
	outfile.close();
}

/**
 * store the command name and filename argument
 */
token::iterator preparse(token tok, Command &c) {
	token::iterator itr = tok.begin();
	c.cname = *itr; // command name
	assert(++itr != tok.end());

	// check for file extension
	cmatch what;
	regex exp("(.*)(\\.poly)");
	if (regex_match(((string)*itr).c_str(), what, exp)) { // file extension found
		c.filename = what[0];
	} else { // append file extension
		c.filename = ((string)*itr).append(polyextstr);
	}
	return ++itr;
}

/**
 * convert a transform code in string to enum type
 */
code_t codeStrToEnum(string code) {
	int index = -1;
	do {
		if (string(codeStr[++index]) == code)
			break;
	} while ((code_t)index != END);
	assert((code_t)index != END);
	code_t enumcode = (code_t)index;
	return enumcode;
}

MidasPolygon::MidasPolygon(token tok) {
	token::iterator itr = preparse(tok, *this);
	// insert the 2d points
	for (; itr != tok.end(); ++itr) {
		double x = atof( ((string)*itr).c_str() );
		assert(++itr != tok.end());
		double y = atof( ((string)*itr).c_str() );
		int r = 255, g = 255, b = 255;//, a = 255;
		token::iterator backtrackItr = itr;
		if (++itr != tok.end()) {
			string color = (string)*itr;
			if (color.size() == 6) {
				string red = string("0x").append(color.substr(0,2));
				string green = string("0x").append(color.substr(2,2));
				string blue = string("0x").append(color.substr(4,2));
				//string alpha = string("0x").append(color.substr(6,2));
				//cout << red << " " << green << " " << blue << endl;
				char *endptr;
				r = strtol(red.c_str(), &endptr, 16);
				g = strtol(green.c_str(), &endptr, 16);
				b = strtol(blue.c_str(), &endptr, 16);
				//a = strtol(alpha.c_str(), &endptr, 16);
			} else
				itr = backtrackItr;
		} else
			itr = backtrackItr;
		//cout << r << " " << g << " " << b << endl;
		Point p(x, y, 0.0, r, g, b);
		//cout << p << "\n";
		points.push_back(p);
		//cout << points.back() << "\n\n";
		assert(itr != tok.end());
	}
	//cout << endl;
}

void MidasPolygon::execute() {
	//cout << "Executing Polygon\n";
	boost::filesystem::create_directory(polypathstr);
	ofstream file((string(polypathstr) + "/" + this->filename).c_str());
	BOOST_FOREACH(Point p, this->points) {
		file << p << " ";
	}
	file << "\n";
	file.close();
}

Assemble::Assemble(token tok) {
	token::iterator itr = preparse(tok, *this);

	for (; itr != tok.end(); ++itr) {
		boost::filesystem::path filepath = polypath / (*itr + polyextstr);
		assert(boost::filesystem::exists(filepath));
		this->files.push_back(*itr);
	}
}

void Assemble::execute() {
	//cout << "Executing Assemble\n";
	boost::filesystem::create_directory(polypathstr);
	ofstream newfile((string(polypathstr) + "/" + this->filename).c_str());
	BOOST_FOREACH(string thisfile, files) {
		ifstream curfile((polypath / (thisfile + polyextstr)).string().c_str());
		char c;
		while ((c = curfile.get()) != EOF)
			newfile << c;
		curfile.close();
	}
	newfile.close();
	//cout << (string(polypathstr) + "/" + this->filename).c_str() << " assembled\n";
}

Transform::Transform() {

}

Transform::Transform(polygonList polygons) {
	this->polygons = polygons;
}

Transform::Transform(projPolygonList projPolygons) {
	this->projPolygons = projPolygons;
}

/**
 * transform constructor with argument
 * param tok tokenized command
 */
Transform::Transform(token tok) {
	token::iterator itr = preparse(tok, *this);
	readPolygons(this->filename, &polygons, &texturedPolygons);
	readCodes(tok, itr);
}

void Transform::readCodes(token tok, token::iterator itr) {
	for(; itr != tok.end(); itr++) {
		code_t code = codeStrToEnum(*itr);
		itr++;
		assert(itr != tok.end());
		tfCmd[code] = new double(atof((*itr).c_str()));
	}
}

vector<Point> getCoords(token tok) {
	vector<Point> newPolygon;
	vector<string> coords; // put all coords string in a vector
	BOOST_FOREACH(string s, tok) {
		//cout << s << " ";
		coords.push_back(s);
		////cout << coords.back() << " ";
	}
	//cout << endl;

	assert(coords.size() % 4 == 0);
	for (unsigned int i = 0; i < coords.size(); i += 4) {
		double x = atof(coords[i].c_str());
		double y = atof(coords[i+1].c_str());
		double z = atof(coords[i+2].c_str());
		string color = coords[i+3];
		assert(color.size() == 6);
		string red = string("0x").append(color.substr(0,2));
		string green = string("0x").append(color.substr(2,2));
		string blue = string("0x").append(color.substr(4,2));
		//string alpha = string("0x").append(color.substr(6,2));
		//cout << red << " " << green << " " << blue << endl;
		char *endptr;
		int r = strtol(red.c_str(), &endptr, 16);
		int g = strtol(green.c_str(), &endptr, 16);
		int b = strtol(blue.c_str(), &endptr, 16);
		//int a = strtol(alpha.c_str(), &endptr, 16);
		newPolygon.push_back(Point(x, y, z, r, g, b));
	}
	return newPolygon;
}

void Command::readPolygons(string filename, polygonList* polygons, texturedPolygonList* texturedPolygons) {
	//cout << "Reading " << filename << ":\n";
	char c[512];
	assert(boost::filesystem::exists(polypath / filename));
	//cout << (string(polypathstr) + "/" + filename).c_str() << endl;
	ifstream inf((string(polypathstr) + "/" + filename).c_str());
	while(inf.good()) {
		inf.getline(c,512);
		if (c[0] == '#' || c[0] == '\n') // comment or blank lines
			continue;
		if (c[0] == '\0')
			break;

		string str(c);
		erase_all(str, ")"); // remove extra characters
		erase_all(str, "(");
		replace_all(str, ",", " ");
//		cout << str << endl;

		char_separator<char> sep(" "); // tokenize line
		tokenizer<char_separator<char> > tok(str, sep);

		token::iterator itr = tok.begin();
		if (*itr == string("t")) {	// if textured polygon
			TexShape newTexShape;
			newTexShape.texturefile = *(++itr);
			int count = atoi((++itr)->c_str());
			//cout << count << endl;
			for (int i = 0; i < count; i++) {
				if (c[0] == '#' || c[0] == '\n') // comment or blank lines
					continue;
				if (c[0] == '\0')
					break;
				inf.getline(c,512);
				str = string(c);
				erase_all(str, ")"); // remove extra characters
				erase_all(str, "(");
				replace_all(str, ",", " ");
				tok = token(str, sep);
				newTexShape.shape.push_back(getCoords(tok));

				inf.getline(c,512);
				str = string(c);
				erase_all(str, ")"); // remove extra characters
				erase_all(str, "(");
				replace_all(str, ",", " ");
				tok = token(str, sep);
				newTexShape.texture.push_back(getCoords(tok));
			}
			//newTexShape.print(cout);
			if (texturedPolygons) {
				texturedPolygons->push_back(newTexShape);
			}
			continue;
		}

		vector<Point> newPolygon = getCoords(tok);
		if (newPolygon.size() > 0) // make sure no empty polygon is inserted
			polygons->push_back(newPolygon);
	}
	BOOST_FOREACH(vector<Point> polygon, *polygons) {
		BOOST_FOREACH(Point p, polygon) {
			//cout << p << " ";
		}
		//cout << endl;
	}
	inf.close();
}

void Transform::execute() {
	//cout << "Executing Transform\n";
	transform(translate() * scale() * rotate(), false);
}

polygonList Transform::transform(Transformation t, bool project) {
	//cout << "Transformation matrix:\n";
	//cout << t << endl;
	if (!project) { // transforming only
		vector< vector<Point> >::iterator polygonItr = polygons.begin();
		for(; polygonItr != polygons.end(); polygonItr++) {
			vector<Point>::iterator pointItr = (*polygonItr).begin();
			for(; pointItr != (*polygonItr).end(); pointItr++) // apply matrix to each points
				(*polygonItr).insert((*polygonItr).erase(pointItr),  t.applied_to(*pointItr));
		}
		for (int i = 0; i < texturedPolygons.size(); i++) {
			TexShape *ts = &texturedPolygons[i];
			vector< vector<Point> >::iterator polygonItr = ts->shape.begin();
			for(; polygonItr != ts->shape.end(); polygonItr++) {
				vector<Point>::iterator pointItr = (*polygonItr).begin();
				for(; pointItr != (*polygonItr).end(); pointItr++) // apply matrix to each points
					(*polygonItr).insert((*polygonItr).erase(pointItr),  t.applied_to(*pointItr));
			}
		}
		//cout << "polygons: \n";
//		BOOST_FOREACH(vector<Point> polygon, polygons) {
//			BOOST_FOREACH(Point p, polygon) {
//				cout << p << " ";
//			}
//			cout << endl;
//		}
		//cout << "textured polygons:\n";
//		BOOST_FOREACH(TexShape ts, texturedPolygons) {
//			BOOST_FOREACH(vector<Point> shape, ts.shape) {
//				BOOST_FOREACH(Point p, shape) {
//					cout << p << " ";
//				}
//				cout << endl;
//			}
//		}
	} else { // camera / viewing transform
		vector< vector<ProjPoint> >::iterator polygonItr = projPolygons.begin();
		for(; polygonItr != projPolygons.end(); polygonItr++) {
			vector<ProjPoint>::iterator pointItr = (*polygonItr).begin();
			for(; pointItr != (*polygonItr).end(); pointItr++)
				(*polygonItr).insert((*polygonItr).erase(pointItr),  t.applied_to(*pointItr));
		}

//		BOOST_FOREACH(vector<ProjPoint> polygon, projPolygons) {
//			BOOST_FOREACH(ProjPoint p, polygon) {
//				//cout << p << " ";
//			}
//			//cout << endl;
//		}

		polygons.clear();
		BOOST_FOREACH(vector<ProjPoint> polygon, projPolygons) {
			vector<Point> newpolygon;
			BOOST_FOREACH(ProjPoint p, polygon) {
//				if (p.w() < 0)
//					p.setW(-1.0 * p.w());
				//cout << p << " ";
				Point pp = p.project();
				newpolygon.push_back(pp);
			}
			polygons.push_back(newpolygon);
			//cout << endl;
		}
	}
	return this->polygons;
}

Transformation Transform::translate() {
	double x = (tfCmd[XT])?*tfCmd[XT]:0.000;
	double y = (tfCmd[YT])?*tfCmd[YT]:0.000;
	double z = (tfCmd[ZT])?*tfCmd[ZT]:0.000;
	//cout << codeStr[XT] << ": " << x << endl;
	//cout << codeStr[YT] << ": " << y << endl;
	//cout << codeStr[ZT] << ": " << z << endl;
	return Transformation::translate_by(x, y, z);
}

Transformation Transform::scale() {
	double x = (tfCmd[XS])?*tfCmd[XS]:1.000;
	double y = (tfCmd[YS])?*tfCmd[YS]:1.000;
	double z = (tfCmd[ZS])?*tfCmd[ZS]:1.000;
	double u = (tfCmd[US])?*tfCmd[US]:1.000;
	//cout << codeStr[XS] << ": " << x << endl;
	//cout << codeStr[YS] << ": " << y << endl;
	//cout << codeStr[ZS] << ": " << z << endl;
	//cout << codeStr[US] << ": " << u << endl;
	return Transformation::scale_by(x, y, z) * Transformation::scale_by(u, u, u);
}

Transformation Transform::rotate() {
	double xd = (tfCmd[XD])?*tfCmd[XD]:0.000;
	double yd = (tfCmd[YD])?*tfCmd[YD]:0.000;
	double zd = (tfCmd[ZD])?*tfCmd[ZD]:0.000;
	double xr = (tfCmd[XR])?*tfCmd[XR]:0.000;
	double yr = (tfCmd[YR])?*tfCmd[YR]:0.000;
	double zr = (tfCmd[ZR])?*tfCmd[ZR]:0.000;
	//cout << codeStr[XD] << ": " << xd << endl;
	//cout << codeStr[YD] << ": " << yd << endl;
	//cout << codeStr[ZD] << ": " << zd << endl;
	//cout << codeStr[XR] << ": " << xr << endl;
	//cout << codeStr[YR] << ": " << yr << endl;
	//cout << codeStr[ZR] << ": " << zr << endl;
	return Transformation::x_rotate_by(xd) * Transformation::y_rotate_by(yd) *
		Transformation::z_rotate_by(zd) * Transformation::x_rotate_by(xr * 180.0 / pi) *
		Transformation::y_rotate_by(yr * 180.0 / pi) * Transformation::z_rotate_by(zr * 180.0 / pi);
}


/**
 * create newfile oldfile transforms
 */
Create::Create(token tok) {
	token::iterator itr = preparse(tok, *this);
	outputfile = filename;

	assert(itr != tok.end());
	// check for file extension
	cmatch what;
	regex exp("(.*)(\\.poly)");
	if (regex_match(((string)*itr).c_str(), what, exp)) { // file extension found
		filename = what[0];
	} else { // append file extension
		filename = ((string)*itr).append(polyextstr);
	}
	readPolygons(filename, &polygons, &texturedPolygons);
	readCodes(tok, ++itr);
}

void Create::execute() {
	//cout << "Executing Create\n";
	Transform::execute();
	writePolygons(polygons, this->outputfile);
	BOOST_FOREACH(TexShape ts, texturedPolygons) {
		Surface::writeSurface(ts, outputfile);
	}
}

View::View() {

}

View::View(token tok) {
	token::iterator itr = preparse(tok, *this);
	eyeX = atof((*itr).c_str());
	assert(++itr != tok.end());
	eyeY = atof((*itr).c_str());
	assert(++itr != tok.end());
	eyeZ = atof((*itr).c_str());
	assert(++itr != tok.end());
	upX = atof((*itr).c_str());
	assert(++itr != tok.end());
	upY = atof((*itr).c_str());
	assert(++itr != tok.end());
	upZ = atof((*itr).c_str());
	assert(++itr != tok.end());
	centerX = atof((*itr).c_str());
	assert(++itr != tok.end());
	centerY = atof((*itr).c_str());
	assert(++itr != tok.end());
	centerZ = atof((*itr).c_str());
	assert(++itr != tok.end());
	angle = atof((*itr).c_str());
	assert(++itr != tok.end());
	near = atof((*itr).c_str());
	assert(++itr != tok.end());
	far = atof((*itr).c_str());
	readPolygons(filename, &polygons, &texturedPolygons);
}

void View::convert() {
	//cout << "Converting points to projective space\n";
	BOOST_FOREACH(vector<Point> polygon, polygons) {
		vector<ProjPoint> newpolygon;
		BOOST_FOREACH(Point p, polygon) {
			ProjPoint pp(p);
			newpolygon.push_back(pp);
			//cout << pp << " ";
		}
		projPolygons.push_back(newpolygon);
		//cout << endl;
	}
}

/**
 * view file eyex eyey eyez roll upy pitch centerx centery centerz angle near far
 */
void View::execute() {
//	convert();
	//cout << "Executing View\n";
//	Frame f1(Vector(1, 0, 0), Vector(0, 1, 0), Vector(0, 0, 1), Point(centerX, centerY, centerZ));
//	Vector w = Point(eyeX, eyeY, eyeZ) - Point(centerX, centerY, centerZ);
//	w.normalize();
//	Frame f2(w, Vector(upX,1,0), Point(eyeX, eyeY, eyeZ));
//	Transform tf(projPolygons);
//	Transformation camera_t = Transformation::frame_to_frame(f1, f2);
//	Transformation viewing_t = Transformation::view(near, far, angle);
//	//cout << "Camera:\n" << camera_t << endl;
//	//cout << "Viewing:\n" << viewing_t << endl;
//	polygons = tf.transform(camera_t * viewing_t, true);
	//cout << "Final Points:\n";
//	BOOST_FOREACH(vector<Point> polygon, polygons) {
//		BOOST_FOREACH(Point p, polygon) {
//			//cout << p << " ";
//		}
//		//cout << endl;
//	}
}

Extrude::Extrude(token tok) {
	token::iterator itr = preparse(tok, *this);
	assert(itr != tok.end());

	cmatch what;
	regex exp("(.*)(\\.poly)");
	if (regex_match(((string)*itr).c_str(), what, exp)) { // file extension found
		newfile = what[0];
	} else { // append file extension
		newfile = ((string)*itr).append(polyextstr);
	}
	assert(++itr != tok.end());
	units = atof((*itr).c_str());
	readPolygons(filename, &polygons);
}

/**
 * extrude oldfile newfile units
 */

void Extrude::execute() {
	//cout << "Executing Extrude\n";
	vector<vector<Point> > newPolygons;

	BOOST_FOREACH(vector<Point> polygon, polygons) {
		newPolygons.push_back(polygon); // add the original base
		vector<Point> topFace;
		Vector normal;
		for (unsigned int i = 0; i < polygon.size(); i++) {
			if (i == 0) { // get the normal vector from first 3 points
				Vector v1;
				Vector v2;
				v1 = polygon.back() - polygon[0];
				//cout << "v1: " << v1 << endl;
				v2 = polygon[i+1] - polygon[i];
				v1.normalize();
				v2.normalize();
				//cout << "v2: " << v2 << endl;
				normal = cross(v1, v2);
				normal.normalize();
				//cout << "normal: " << normal << endl;
				normal *= units;
			}
			topFace.push_back(polygon[i] + normal); // add the extruded points
		}
//		BOOST_FOREACH(Point p, topFace) {
//			//cout << p << " ";
//		}
		//cout << endl;
		newPolygons.push_back(topFace); // add extruded face

		// add the extruded sides
		for (unsigned int i = 0; i < polygon.size(); i++) {
			vector<Point> newSideFace;
			newSideFace.push_back(polygon[i]); // push 2 base points
			if (i+1 < polygon.size())
				newSideFace.push_back(polygon[i+1]);
			else
				newSideFace.push_back(polygon[0]);
			if (i+1 < topFace.size()) // push 2 top face points
				newSideFace.push_back(topFace[i+1]);
			else
				newSideFace.push_back(topFace[0]);
			newSideFace.push_back(topFace[i]);

			newPolygons.push_back(newSideFace); // add extruded sides
		}

//		BOOST_FOREACH(vector<Point> polygon, newPolygons) {
//			BOOST_FOREACH(Point p, polygon) {
//				//cout << p << " ";
//			}
//			//cout << endl;
//		}

		writePolygons(newPolygons, this->newfile);

	}
}

Clip::Clip(token tok) {
	token::iterator itr = preparse(tok, *this);
	assert(itr != tok.end());

	cmatch what;
	regex exp("(.*)(\\.poly)");
	if (regex_match(((string)*itr).c_str(), what, exp)) { // file extension found
		newfile = what[0];
	} else { // append file extension
		newfile = ((string)*itr).append(polyextstr);
	}
	double px, py, pz, nx, ny, nz;
	assert(++itr != tok.end());
	px = atof((*itr).c_str());
	assert(++itr != tok.end());
	py = atof((*itr).c_str());
	assert(++itr != tok.end());
	pz = atof((*itr).c_str());
	assert(++itr != tok.end());
	nx = atof((*itr).c_str());
	assert(++itr != tok.end());
	ny = atof((*itr).c_str());
	assert(++itr != tok.end());
	nz = atof((*itr).c_str());
	p = Point(px, py, pz);
	vnorm = Vector(nx, ny, nz);
	vnorm.normalize();

	readPolygons(filename, &polygons);
}

void Clip::execute() {
	//cout << "Executing Clip" << endl;
	// get the clipping point
	//cout << "p: " << p << endl << "vnorm: " << vnorm << endl;
	polygonList clippedPolygons;
	BOOST_FOREACH(vector<Point> polygon, polygons) {
		vector<Point> clippedPolygon;
		Point lastPoint = polygon.back();
		BOOST_FOREACH(Point q, polygon) {
			float d1 = dot(vnorm, (q-p));
			float d2 = dot(vnorm, (lastPoint - p));
			//cout << "d1: " << d1 << " d2: " << d2 << endl;
			if (d1 >= 0.0 && d2 >= 0.0) {
				clippedPolygon.push_back(q);
				//cout << q << " is IN" << endl;
			}
			else if (d1 >= 0.0 && d2 < 0.0 || d1 < 0.0 && d2 >= 0.0) { // lastpoint on outside
				float t = d1 / (d1 - d2);
				Point I = q + t*(lastPoint - q);
				Point *outPoint; // determine which one is out to set color
				if (d1 >= 0.0) { // current is IN, last is OUT
					//cout << q << " is IN\n";
					outPoint = &lastPoint;
				} else { // current is OUT, last is IN
					//cout << q << " is OUT\n";
					outPoint = &q;
				}
				I.setA(outPoint->a()); // set color of intersection point
				I.setG(outPoint->g()); //  to the color of point on
				I.setR(outPoint->r()); //  outside
				I.setB(outPoint->b());
				//cout << I << " is an intersection\n";
				clippedPolygon.push_back(I); // add intersection point to polygon
				if (d1 >= 0.0) // add current point to polygon if IN
					clippedPolygon.push_back(q);
			} else if (d1 == 0.0) { // point is on the plane
				clippedPolygon.push_back(q);
				//cout << q << " is on the plane\n";
			} else {
				//cout << q << " is OUT\n";
			}
			lastPoint = q;
		}
		clippedPolygons.push_back(clippedPolygon);
	}

	//cout << "clipped polygons:\n";
//	BOOST_FOREACH(vector<Point> clippedPolygon, clippedPolygons) {
//		BOOST_FOREACH(Point cp, clippedPolygon) {
//			//cout << cp << " ";
//		}
//		//cout << endl;
//	}

	writePolygons(clippedPolygons, this->newfile);
}

Volume::Volume(token tok) {
	token::iterator itr = preparse(tok, *this);
	assert(itr != tok.end());

	cmatch what;
	regex exp("(.*)(\\.poly)");
	if (regex_match(((string)*itr).c_str(), what, exp)) { // file extension found
		newfile = what[0];
	} else { // append file extension
		newfile = ((string)*itr).append(polyextstr);
	}
	assert(++itr != tok.end());
	double px = atof((*itr).c_str());
	assert(++itr != tok.end());
	double py = atof((*itr).c_str());
	assert(++itr != tok.end());
	double pz = atof((*itr).c_str());
	assert(++itr != tok.end());
	double vx = atof((*itr).c_str());
	assert(++itr != tok.end());
	double vy = atof((*itr).c_str());
	assert(++itr != tok.end());
	double vz = atof((*itr).c_str());
	assert(++itr != tok.end());
	alpha = atof((*itr).c_str());
	assert(++itr != tok.end());
	resolution = atof((*itr).c_str());

	referencePoint = Point(px, py, pz);
	axis = Vector(vx, vy, vz);

	readPolygons(filename, &polygons);
}

void Volume::execute() {
	//cout << "Executing Volume\n";

	polygonList newPolygons;

	//cout << "\tTranslating to origin...\n";
	Point p1 = -referencePoint;
	Transformation translateTransform = Transformation::translate_by(p1.x(), p1.y(), p1.z());
	Transform* transformCmd = new Transform(polygons);
	polygons = transformCmd->transform(translateTransform, false);

	Transformation stepRotate = Transformation::rotate_by(resolution, axis);
	//cout << "\t" << stepRotate << endl;

	polygonList lastPolygons = polygons;
	double curRotated = 0.0;

	for (; curRotated <= alpha; curRotated += resolution) {
		//cout << "\tRotated by " << curRotated << " degrees\n";
		polygons = transformCmd->transform(stepRotate, false);
		for(unsigned int i = 0; i < polygons.size(); i++) {
			vector<Point> polygon = polygons[i];
			vector<Point> lastpolygon = lastPolygons[i];
			newPolygons.push_back(lastpolygon);
			if (curRotated + resolution >= alpha)
				newPolygons.push_back(polygon);

			for(unsigned int j = 0; j < polygon.size(); j++) {
				vector<Point> newSideFace;
				newSideFace.push_back(polygon[j]); // push 2 base points
				if (j+1 < polygon.size())
					newSideFace.push_back(polygon[j+1]);
				else
					newSideFace.push_back(polygon[0]);
				if (j+1 < lastpolygon.size()) // push 2 top face points
					newSideFace.push_back(lastpolygon[j+1]);
				else
					newSideFace.push_back(lastpolygon[0]);
				newSideFace.push_back(lastpolygon[j]);

				newPolygons.push_back(newSideFace); // add surfaces
			}

		}
		lastPolygons = polygons;
	}
	if (curRotated < alpha) {
		// generate one more volume to close surface
	}

	//cout << "\tTranslating back to original position...\n";
	Point p0 = referencePoint;
	translateTransform = Transformation::translate_by(p0.x(), p0.y(), p0.z());
	if (transformCmd) delete transformCmd;
	transformCmd = new Transform(newPolygons);
	newPolygons = transformCmd->transform(translateTransform, false);

	writePolygons(newPolygons, newfile);
}

Surface::Surface(token tok) {
	token::iterator itr = preparse(tok, *this);
	assert(itr != tok.end());
	texturefile = *itr;
	itr++;
	int level = atoi((*itr).c_str());
	assert(level >= 0);

	matrix<Point> patch(4,4);

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			assert(++itr != tok.end());
			int x = atoi((*itr).c_str());
			assert(++itr != tok.end());
			int y = atoi((*itr).c_str());
			assert(++itr != tok.end());
			int z = atoi((*itr).c_str());
			patch(i,j) = Point(x,y,z);
		}
	}
	surface = new BezierPatch(patch);
	surface->refine(level);
}

void Surface::execute() {
	cout << "Executing Surface..." << endl;
	matrix<Point> patch = surface->flatten();
	for (unsigned int i = 0; i < patch.RowNo()-1; i++) {
		for (unsigned int j = 0; j < patch.ColNo()-1; j++) {
			vector<Point> polygon;
			vector<Point> texCoords;
			polygon.push_back(Point(patch(i,j))); // top left
			texCoords.push_back( Point(
					(double)j/((double)patch.ColNo()-1.0),
					1.0-(double)i/((double)patch.RowNo()-1.0) ) );
			polygon.push_back(Point(patch(i,j+1))); // top right
			texCoords.push_back( Point(
					(double)(j+1)/((double)patch.ColNo()-1.0),
					1.0-(double)i/((double)patch.RowNo()-1.0) ) );
			polygon.push_back(Point(patch(i+1,j+1))); // bottom right
			texCoords.push_back( Point(
					(double)(j+1)/((double)patch.ColNo()-1.0),
					1.0-(double)(i+1)/((double)patch.RowNo()-1.0) ) );
			polygon.push_back(Point(patch(i+1,j))); // bottom left
			texCoords.push_back( Point(
					(double)j/((double)patch.ColNo()-1.0),
					1.0-(double)(i+1)/((double)patch.RowNo()-1.0) ) );
			polygons.shape.push_back(polygon);
			polygons.texture.push_back(texCoords);
			polygons.texturefile = texturefile;
		}
	}
	writeSurface(polygons, filename, ios_base::trunc);
}

void Surface::writeSurface(TexShape polygons, string filename, ios_base::openmode mode) {
	boost::filesystem::create_directory(polypathstr);
	ofstream outfile((string(polypathstr) + "/" + filename).c_str(), mode);
	polygons.print(outfile);
	outfile.close();
	cout << (string(polypathstr) + "/" + filename).c_str() << " created\n";
}
