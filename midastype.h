/*
 * midastype.h
 *
 *  Created on: Nov 3, 2008
 *      Author: eddyc
 */

#ifndef MIDASTYPE_H_
#define MIDASTYPE_H_

/* define transform codes */
#define TFCODE \
	X(XT, "XT")\
	X(YT, "YT")\
	X(ZT, "ZT")\
	X(XS, "XS")\
	X(YS, "YS")\
	X(ZS, "ZS")\
	X(US, "US")\
	X(XD, "XD")\
	X(YD, "YD")\
	X(ZD, "ZD")\
	X(XR, "XR")\
	X(YR, "YR")\
	X(ZR, "ZR")\
	X(END, "END")

#define X(a, b) a,
typedef enum {
	TFCODE
} code_t;
#undef X

#define X(a, b) b,
static const char* codeStr[] = {
	TFCODE
};
#undef X
/* end define transform codes */

/* define command names */
#define CMDNAME \
	X(polygon, "polygon")\
	X(assemble, "assemble")\
	X(transform, "transform")\
	X(create, "create")\
	X(extrude, "extrude")

#define X(a, b) a,
typedef enum {
	CMDNAME
} cmdname_t;
#undef X

#define X(a, b) b,
static const char* cmdStr[] = {
	CMDNAME
};
#undef X
/* end define command names */

#endif /* MIDASTYPE_H_ */
