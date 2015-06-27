#include <QtGui>
#include <QApplication>
#include <iostream>
#include <cstdlib>
#include <string>
#include <boost/foreach.hpp>
#include "commands.h"
#include "midasview.h"

using namespace std;
using namespace boost;

void parse(polygonList *polygons, texturedPolygonList *texturedPolygons) {
	char ccommand[512];
	string strcommand;
	Command *c = NULL;
	do {
		strcommand.clear();
		do { // loop and get the whole command
			cin.getline(ccommand, 512);
			if (string(ccommand).find_first_of("#") != string::npos)
				continue;
			strcommand.append(ccommand);
		} while (string(ccommand).find_last_of("\\") != string::npos);
		token tok = Command::tokenize_cmd(strcommand);
		BOOST_FOREACH(string s, tok) {
			//cout << s << " ";
		}
		cout << endl;
		string cname = *(tok.begin()); // get first token, should be command name
		if (cname == string("polygon")) {
			c = new MidasPolygon(tok);
		} else if( cname == string("assemble")) {
			c = new Assemble(tok);
		} else if( cname == string("transform")) {
			c = new Transform(tok);
		} else if( cname == string("create")) {
			c = new Create(tok);
		} else if( cname == string("view")) {
			c = new View(tok);
			strcommand = string("quit");
		} else if(cname == string("extrude")) {
			c = new Extrude(tok);
		} else if(cname == string("clip")) {
			c = new Clip(tok);
		} else if(cname == string("volume")) {
			c = new Volume(tok);
		} else if(cname == string("surface")) {
			c = new Surface(tok);
		}
		if (c) {
			c->execute();
			if (cname == string("view")) {
				*polygons = ((View*)c)->polygons;
				*texturedPolygons = ((View*)c)->texturedPolygons;
			}
			delete c;
			c = NULL;
		}
	} while (strcommand != string("quit"));
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWheelScrollLines(1);

    polygonList polygons;
    texturedPolygonList texturedPolygons;
    parse(&polygons, &texturedPolygons);
    cout << "Prepared" << endl;

//    BOOST_FOREACH(TexShape ts, texturedPolygons) {
//    	ts.print(cout);
//    }

    MidasW w(polygons, texturedPolygons);
    w.show();
    return a.exec();
}
