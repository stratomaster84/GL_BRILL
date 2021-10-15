//---------------------------------------------------------------------------

#ifndef BrilliantH
#define BrilliantH

#define BACK_PIC "background.tga"
#define EDGE_PIC "edge.tga"

typedef float point3[3];
typedef unsigned int triangle_index3[3];
typedef float texel2[2];

class _Bril_Model{
public:
        int _list;

                // начало переменных, которые можно запросить
        bool _inited;

        int edge_count;
        int prec;

        double Z_bottom;
        double Z_top;
        double Z_middle;

        double light_intensity;

        int point_num;
        int triangle_count;
                // конец переменных, которые можно запросить

        point3                  *_Vertice;
        point3                  *_Normal;
        texel2                  *_Texel;
        triangle_index3         *_Triangle;

        void MadeBrillMap();
        void DeleteBrillMap();

        void SetNormals();
        void SetTexels();
//void NormCoords();
//void ScaleCoords();

        void Build_Brill();

        _Bril_Model();
        ~_Bril_Model();

        void MadeGLList(const int& count, const double& bottom, const double& top, const double& middle, const int& precision, const double& intensity);

        void Draw_Brill();
};
//---------------------------------------------------------------------------
#endif
 
