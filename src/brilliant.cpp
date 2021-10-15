//---------------------------------------------------------------------------

#include "brilliant.h"
#include <GL/gl.h>
#define _USE_MATH_DEFINES
#include <math.h>

//---------------------------------------------------------------------------
_Bril_Model::_Bril_Model(){
        _inited = false;
}
//---------------------------------------------------------------------------
_Bril_Model::~_Bril_Model(){
        glDeleteLists (_list,1);
        _inited = false;
}
//---------------------------------------------------------------------------
void _Bril_Model::Draw_Brill(){
        if(_inited)
                glCallList(_list);
}
//---------------------------------------------------------------------------
void _Bril_Model::Build_Brill(){
  int i;
  glBegin(GL_TRIANGLES);
      for(i=0;i<triangle_count;i++){
        glTexCoord2fv(_Texel  [_Triangle[i][0]]);
        glNormal3fv  (_Normal [_Triangle[i][0]]);
        glVertex3fv  (_Vertice[_Triangle[i][0]]);

        glTexCoord2fv(_Texel  [_Triangle[i][1]]);
        glNormal3fv  (_Normal [_Triangle[i][1]]);
        glVertex3fv  (_Vertice[_Triangle[i][1]]);

        glTexCoord2fv(_Texel  [_Triangle[i][2]]);
        glNormal3fv  (_Normal [_Triangle[i][2]]);
        glVertex3fv  (_Vertice[_Triangle[i][2]]);
                }
  glEnd();
}
//---------------------------------------------------------------------------
void _Bril_Model::MadeBrillMap(){

  _Vertice = new point3 [point_num];
  _Normal = new point3 [point_num];
  _Texel = new texel2 [point_num];
  _Triangle = new triangle_index3[triangle_count];

  int i;

//нижняя точка
  _Vertice[0][0] = 0.0;
  _Vertice[0][1] = 0.0;
  _Vertice[0][2] = -Z_bottom;

//верхняя точка, нулевой пояс и верхний пояс
  double angle1,angle2;
  double r_temp,z_temp;
  double mid_angle;

  r_temp = (1.0 - Z_middle);
  z_temp = Z_middle*Z_top;
  mid_angle = M_PI/double(edge_count);

//верхняя точка
  _Vertice[point_num-1][0] = 0.0;
  _Vertice[point_num-1][1] = 0.0;
  _Vertice[point_num-1][2] = z_temp;

  for(i = 1;i<=edge_count;i++){
        angle1 = double(i-1)/double(edge_count)*M_PI*2.0;
        angle2 = angle1 + mid_angle;
//нулевой пояс
        _Vertice[i][0] = sin(angle1);
        _Vertice[i][1] = cos(angle1);
        _Vertice[i][2] = 0.0;
//верхний пояс
        _Vertice[i + edge_count][0] = sin(angle2)*r_temp;
        _Vertice[i + edge_count][1] = cos(angle2)*r_temp;
        _Vertice[i + edge_count][2] = z_temp;
  }

// СТРОИМ ТРЕУГОЛЬНИКИ

  // НИЖНИЕ
    for(i=0;i<edge_count-1;i++){
        _Triangle[i][0] = 0;
        _Triangle[i][1] = i+1;
        _Triangle[i][2] = i+2;
    }
    _Triangle[i][0] = 0;
    _Triangle[i][1] = i+1;
    _Triangle[i][2] = 1;

  // СРЕДНИЕ И ВЕРХНИЕ
    for(i=0;i<edge_count-1;i++){
        _Triangle[i+edge_count][0] = i+1;
        _Triangle[i+edge_count][2] = i+2;
        _Triangle[i+edge_count][1] = edge_count + i + 1;

        _Triangle[i+2*edge_count][0] = i+2;
        _Triangle[i+2*edge_count][1] = edge_count + i + 1;
        _Triangle[i+2*edge_count][2] = edge_count + i + 2;

        _Triangle[i+3*edge_count][0] = point_num-1;
        _Triangle[i+3*edge_count][2] = edge_count + i + 1;
        _Triangle[i+3*edge_count][1] = edge_count + i + 2;
    }

    _Triangle[i+edge_count][0] = 1;
    _Triangle[i+edge_count][1] = i+1;
    _Triangle[i+edge_count][2] = edge_count + i + 1;

    _Triangle[i+2*edge_count][0] = 1;
    _Triangle[i+2*edge_count][1] = edge_count + i + 1;
    _Triangle[i+2*edge_count][2] = edge_count + 1;

    _Triangle[i+3*edge_count][0] = point_num-1;
    _Triangle[i+3*edge_count][2] = edge_count + i + 1;
    _Triangle[i+3*edge_count][1] = edge_count + 1;
}
//---------------------------------------------------------------------------
void _Bril_Model::DeleteBrillMap(){
        if(!_inited)
                return;
        if(_Vertice!=NULL)
                delete [] _Vertice;
        if(_Triangle!=NULL)
                delete [] _Triangle;
        if(_Texel!=NULL)
                delete [] _Texel;
        if(_Normal!=NULL)
                delete [] _Normal;
}
//---------------------------------------------------------------------------
void _Bril_Model::MadeGLList(const int& count, const double& bottom, const double& top, const double& middle, const int& precision, const double& intensity){

        if(_inited)
                return;

        edge_count = count<3?3:count;
        prec = precision<1?1:precision;

        light_intensity = intensity<=0.0?1.0:intensity;

        Z_bottom = bottom<=0.0?1.0:bottom;
        Z_top = top<=0.0?1.0:top;
        Z_middle = middle<=0.0?0.5:middle;
        if(Z_middle>=1.0)
            Z_middle = 0.5;

        point_num = 1 + 2*edge_count*prec + 1;  // нижняя + 2 пояса + верхняя
        triangle_count = edge_count + 2*edge_count + edge_count;
        triangle_count *= prec;

        MadeBrillMap();
//NormCoords();
        SetTexels();
//ScaleCoords();
        SetNormals();

        _list = glGenLists(1);
        glNewList(_list, GL_COMPILE);
            Build_Brill();
        glEndList();

        DeleteBrillMap();
        _inited = true;

}
//---------------------------------------------------------------------------
void _Bril_Model::SetNormals(){
        double _rad;
        int i;
        for(i=0;i<point_num-1;i++){
                _rad = sqrt(_Vertice[i][0]*_Vertice[i][0] +
                        _Vertice[i][1]*_Vertice[i][1] +
                        _Vertice[i][2]*_Vertice[i][2]);
                _rad /= light_intensity;
                _Normal[i][0] = _Vertice[i][0]/_rad;
                _Normal[i][1] = _Vertice[i][1]/_rad;
                _Normal[i][2] = _Vertice[i][2]/_rad;
                //if(_rad > _radius)
                        //_radius = _rad;
        }
        _rad = sqrt(_Vertice[i][0]*_Vertice[i][0] +
                _Vertice[i][1]*_Vertice[i][1] +
                _Vertice[i][2]*_Vertice[i][2]);
        _rad /= light_intensity;
        _Normal[i][0] = _Vertice[i][0]/_rad;
        _Normal[i][1] = _Vertice[i][1]/_rad;
        _Normal[i][2] = _Vertice[i][2]/_rad;
}
//---------------------------------------------------------------------------
void _Bril_Model::SetTexels(){
        for(int i=0;i<point_num;i++){
                _Texel[i][1] = _Vertice[i][0];

                if(_Vertice[i][1] < 0.0f)
                        _Texel[i][0] = 0.5f-_Vertice[i][2];
                else
                        _Texel[i][0] = 0.5f+_Vertice[i][2];
        }
}
/*
//---------------------------------------------------------------------------
void _Bril_Model::NormCoords(){
        unsigned int i;
        for(i=0;i<_point_num;i++){
                _Vertice[i][0] /= _max_x;
                _Vertice[i][2] /= _max_z*2;
        }
}
//---------------------------------------------------------------------------
void _Bril_Model::ScaleCoords(){
        unsigned int i;
        for(i=0;i<_point_num;i++){
                _Vertice[i][0]  = (_Vertice[i][0]-0.5f)*_height;
                _Vertice[i][1]  *= _depth*0.5f;
                _Vertice[i][2]  *= _width;
        }

}*/
