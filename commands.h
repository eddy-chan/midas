#ifndef COMMANDS_H_
#define COMMANDS_H_

#ifdef WIN32
// undefine previous definition of near/far if on Windows
	#undef near
	#undef far
#endif

#include <string>
#include <vector>
#include <map>
#include <boost/regex.hpp>
#include <boost/tokenizer.hpp>
#include "Point.h"
#include "ProjPoint.h"
#include "Vector.h"
#include "TransformationWithFrames.h"
#include "midastype.h"
#include "BezierPatch.h"
#include "TexShape.h"
using namespace boost;

typedef tokenizer<char_separator<char> > token;
typedef vector<vector<Point> > polygonList;
typedef vector<vector<ProjPoint> > projPolygonList;
typedef vector<TexShape> texturedPolygonList;


struct codeCmp {
	bool operator()(const code_t c1, const code_t c2) const {
		return c1 < c2;
	}
};

typedef map<const code_t, double*, codeCmp> tfCodeValMap;

class Command {
public:
	Command() {}
	virtual ~Command();
	string cname;
	string filename;
	virtual void execute() = 0;
	static token tokenize_cmd(string command);
	void readPolygons(string filename, polygonList*, texturedPolygonList* = NULL);
	void writePolygons(polygonList polygons, string filename);
};

/**
 * polygon command
 * polygon <polygon_name> [2d points]+
 */
class MidasPolygon : public Command {
public:
	vector<Point> points;

	MidasPolygon(token tok);
	void execute();
};

/**
 * assemble command
 * assemble <object_name> [existing_object]+
 */
class Assemble : public Command {
public:
	vector<string> files;

	Assemble(token tok);
	void execute();
};

/**
 * transform command
 * transform <polygon_name> [<transform_code> <value>]+
 */
class Transform : public Command {
public:
	tfCodeValMap tfCmd;
	polygonList polygons;
	projPolygonList projPolygons;
	texturedPolygonList texturedPolygons;

	Transform();
	Transform(polygonList polygons);
	Transform(projPolygonList projPolygons);
	Transform(token tok);
	void execute();
	polygonList transform(Transformation t, bool project);
	void readCodes(token tok, token::iterator itr);
	Transformation translate();
	Transformation scale();
	Transformation rotate();
};

/**
 * create command
 * create <new_polygon_name> <existing_polygon_name> [<transform_code> <value>]+
 */
class Create : public Transform {
public:
	Transform tfPolygon;
	string outputfile;

	Create(token tok);
	void execute();
};

/**
 * View command
 * view <polygon_name> <eyex eyey eyez centerx centery centerz angle near far>
 */
class View : public Command {
public:
	float eyeX;
	float eyeY;
	float eyeZ;
	float upX;
	float upY;
	float upZ;
	float centerX;
	float centerY;
	float centerZ;
	float angle;
	float near;
	float far;
	polygonList polygons;
	projPolygonList projPolygons;
	texturedPolygonList texturedPolygons;

	View();
	View(token tok);
	void convert(); // convert points to proj points
	void execute();
};

/**
 * Extrude command
 * extrude <existing_polygon_name> <new_polygon_name> <unit>
 */
class Extrude : public Command {
public:
	string newfile;
	double units;
	polygonList polygons;

	Extrude(token tok);
	void execute();
};

/**
 * Clip command
 *  clip objects using a plane specified by a point and normal vector
 *  clip <existing_object> <new_object> <px py pz nx ny nz>
 */
class Clip : public Command {
public:
	string newfile;
	Point p;
	Vector vnorm;
	polygonList polygons;

	Clip(token tok);
	void execute();
};

/**
 * Volume command
 * create a volume of revolution with an existing object around an axis defined by
 * a point and normal vector
 *  volume <existing_object> <new_object> <px py pz vx vy vz alpha resolution>
 */
class Volume : public Command {
public:
	string newfile;
	Point referencePoint; // point of revolution
	Vector axis; // axis of revolution
	double alpha; // angle of revolution
	double resolution; // steps for generating new polygons
	polygonList polygons;

	Volume(token tok);
	void execute();
};

/**
 * Surface command
 * create a textured surface using Bezier patch, subdivided by a refinement level
 * surface <surface_name> <texture_file> <level> <16-3d controls points>
 */
class Surface : public Command {
public:
	string texturefile;
	BezierPatch *surface;
	TexShape polygons;

	Surface(token tok);
	void execute();
	static void writeSurface(TexShape polygons, string filename, ios_base::openmode mode = ios_base::app);
};

#endif /*COMMANDS_H_*/
