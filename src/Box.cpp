#include "Box.hpp"

#include <cmath>

#include "Utils.hpp"
#include "Files.hpp"

Box::Box(Utils & utils_in) : Domain(utils_in),_boxrtmap("Box",utils_in._files.get_basepath()) {

  _name = "Box";
  
   _lb = new double[_spacedim];
   _le = new double[_spacedim];

  
}


Box::~Box()  {  
  
  delete [] _lb;
  delete [] _le;
  
}

void Box::init(double Lref_in)     {

   _Lref = Lref_in;
   double ILref = 1./_Lref;
   _lb[0] = _boxrtmap.get("lxb")*ILref;
   _le[0] = _boxrtmap.get("lxe")*ILref;
   _lb[1] = _boxrtmap.get("lyb")*ILref;
   _le[1] = _boxrtmap.get("lye")*ILref;
   if (_spacedim == 3) {
   _lb[2] = _boxrtmap.get("lzb")*ILref;
   _le[2] = _boxrtmap.get("lze")*ILref;
   }
 
  return;
}


// ========================================================
//this routine picks the point xyz and translates/rotates it
//to the [0,LX]x[0,LY]x[0,LZ] range
//it is regardless of "vb"
//lb and le have length 3
//the dimension of x_in is _spacedim
//the dimension of the x_out vector is _spacedim
//the lb and le are passed as NONdimensional, so we must nondimensionalize them OUTSIDE!
//always remember that passing pointers without range is not a good idea!
//if you use std::vectors you can also know the SIZE of the vector you pass

  //xpr and ypr are the rotated variables
  //then you shift them because so is the mesh file
  //so the equations of the lines for enforcing the bcs are for lines
  //that have been first rotated then shifted from the original mesh (the other one generated by libmesh)
//TODO depending on the SPECIFIC MESH you must provide a different
//transformation
// but, if the type of transformation can be expressed 
//in a GENERAL way so that ALL the REAL BOXES can be 
//transformed:
//the general way is:
//you give three rotations
//         three translations
//and you're done
//we should do in the femusconf.in a part 
//related only to the Box as <Box> </Box>
void Box::TransformPointToRef(const double* x_in,double* x_out) const {

  double thetaz = _utils._urtmap.get("thetaz");
  double transfl = _utils._urtmap.get("transflag");

    double        xpr[3]={0.,0.,0.};
    double         Dx[3]={0.,0.,0.};

  xpr[0]=x_in[0]*cos(thetaz) + x_in[1]*sin(thetaz);
  xpr[1]=x_in[1]*cos(thetaz) - x_in[0]*sin(thetaz);  

  Dx[0]=0.5*(_le[0]-_lb[0])*transfl;
  Dx[1]=0.5*(_le[1]-_lb[1])*transfl;
  x_out[0]=xpr[0] + Dx[0];
  x_out[1]=xpr[1] + Dx[1];

   if (_spacedim == 3) {
          Dx[2]=0.5*(_le[2]-_lb[2])*transfl;
         xpr[2]=x_in[2]; //no rotation wrt planes parallel to xy occurs
       x_out[2]=xpr[2] + Dx[2];
   }  
       
  return;
}

