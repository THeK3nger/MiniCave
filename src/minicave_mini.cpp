// ~*~ More info at https://goo.gl/s3W2MG
//------------------------------------------------
#include <iostream>// Info: https://goo.gl/s3W2MG
using i=int;template<i W,i H>struct minicave{bool 
map[H*W];minicave(){for(i r=0;r<H;r++){for(i c=0;c
<W;c++){k(c,r)=c==0||r==0||(c==W-1)||(r==H-1)||r!=
H/2&&rand()%10<4;}}}i e(bool C){for(i r=0;r<=H-1;r
++){for(i c=0;c<=W-1;c++){i w=nw(c,r,1);k(c,r)=k(c
,r)?w>=3:C?w>=5:w>=5||nw(c,r,2)<=2;}}}i nw(i x,i y
,i r){i w=0;for(i Y=y-r;Y<=y+r;Y++){for(i X=x-r;X
<=x+r;X++){if((X!=x||Y!=y)&&(X<0||Y<0||X>W-1||Y>H-
1||k(X,Y)))w+=1;}}return w;}bool&k(i x,i y){return
map[x+y*W];}auto str(){std::string s;for(i r=0;r<H
;r++){for(i c=0;c<W;c++){s+=k(c,r)?"#":".";}s+=
"\n";}return s;}};i main(){srand(getpid());
minicave<80,80>m;for(i j=0;j<4;++j){m.e(false);}m.
e(true);std::cout<<m.str();}