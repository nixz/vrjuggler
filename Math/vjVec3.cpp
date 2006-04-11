/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#include <vjConfig.h>
#include <Math/vjVec3.h>

void vjVec3::xformFull(const vjMatrix& _m, const vjVec3& _v)
{
    vjVec4 tempVec(_v[0], _v[1], _v[2], 1.0f);
    tempVec.xform(_m, tempVec);

    float w_coord_div = 1/tempVec.vec[3];

    vec[0] = (tempVec.vec[0]* w_coord_div);
    vec[1] = (tempVec.vec[1]* w_coord_div);
    vec[2] = (tempVec.vec[2]* w_coord_div);
}

void vjVec3::xformVec(const vjMatrix& _m, const vjVec3& _v)
{
    vjVec4 tempVec(_v[0], _v[1], _v[2], 0.0f);
    tempVec.xform(_m, tempVec);

    vec[0] = tempVec.vec[0];
    vec[1] = tempVec.vec[1];
    vec[2] = tempVec.vec[2];
}

vjVec3 operator *(float _s, const vjVec3& _v) {
    return vjVec3(_v[0]*_s, _v[1]*_s, _v[2]*_s);
}

vjVec3 operator *(const vjVec3& _v, float _s) {
    return vjVec3(_v[0]*_s, _v[1]*_s, _v[2]*_s);
}

vjVec3 operator /(const vjVec3& _v, float _s) {
    _s = 1.0f/_s;
    return vjVec3(_v[0]*_s, _v[1]*_s, _v[2]*_s);
}

//vjVec3 operator *(const vjVec3& _v, const vjMatrix&  _m) {
//    // transform as point (w=1), assuming affine transformation
//    // i.e. does not use slower dst.xformFullPt().
//    vjVec3 dst; dst.xformPt(_v, _m); return dst;
//}

ostream& operator<<(ostream& out, vjVec3& _v)
{
    for(int j=0;j<3;j++)
    {
    	out << _v.vec[j] << ", ";
    }

    return out;	
}
