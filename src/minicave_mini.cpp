#include <iostream>
using i=int;template<i W,i H>struct minicave{bool map[H*W];minicave(){for(i
r=0;r<H;r++){for(i c=0;c<W;c++) {k(c,r)=c==0||r==0||(c==W-1)||(r==H-1)||r!=
H/2&&rand()%10<4;}}}i evolve(bool C){for(i r=0;r<=H-1;r++){for(i c=0;c<=W-1;
c++){i w1=walls(c,r,1);k(c,r)=k(c,r)?w1>=3:C?w1>=5:w1>=5||walls(c,r,2)<=2;}}
}i walls(i x,i y,i r){i w=0;for(i iY=y-r;iY<=y+r;iY++){for(i iX=x-r;iX<=x+r;
iX++){if((iX!=x||iY!=y)&&(iX<0||iY<0||iX>W-1||iY>H-1||k(iX,iY)))w+=1;}}
return w;}bool&k(i x,i y){return map[x+y*W];}auto str(){std::string s;for(i
r=0;r<H;r++){for(i c=0;c<W;c++){s+=k(c,r)?"#":".";}s+="\n";}return s;}};i
main(){srand(getpid());minicave<80,80>m;for(i j=0;j<4;++j){m.evolve(false);}
m.evolve(true);std::cout<<m.str();}