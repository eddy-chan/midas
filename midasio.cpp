//============================================================================
// Name        : midasio.cpp
// Author      : Eddy Chan
// Version     :
// Copyright   : blah blah blah no copying without permission
// Description : Hello World in C, Ansi-style
//============================================================================

#include <iostream>
#include <cstdlib>
#include <string>
#include "commands.h"
using namespace std;
using namespace boost;

void parse() {
	char ccommand[256];
	string strcommand;
	Command *c = NULL;
	do {
		cin.getline(ccommand, 256);
		strcommand = string(ccommand);

		token tok = Command::tokenize_cmd(strcommand);
		string cname = *(tok.begin());
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
		}
		if (c) {
			c->execute();
			delete c;
			c = NULL;
		}
	} while (strcommand != string("quit"));
}

int main(void) {
	parse();
	return 0;
}
