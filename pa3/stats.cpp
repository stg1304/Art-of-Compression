#include "stats.h"
#include <math.h>


long stats::getSum(char channel, pair<int,int> ul, pair<int,int> lr){
  long l1,l2,l3;
  if(channel == 'r'){
    if(!Pexist(lr.first,ul.second-1)) l1 = 0;
    else l1 = sumRed[lr.first][ul.second-1];

    if(!Pexist(ul.first-1,lr.second)) l2 = 0;
    else l2 = sumRed[ul.first-1][lr.second];

    if(!Pexist(ul.first-1,ul.second-1)) l3 = 0;
    else l3 = sumRed[ul.first-1][ul.second-1];

    return sumRed[lr.first][lr.second]- l1 -l2 + l3;
      }
  if(channel == 'g'){
    if(!Pexist(lr.first,ul.second-1)) l1 = 0;
    else l1 = sumGreen[lr.first][ul.second-1];

    if(!Pexist(ul.first-1,lr.second)) l2 = 0;
    else l2 = sumGreen[ul.first-1][lr.second];

    if(!Pexist(ul.first-1,ul.second-1)) l3 = 0;
    else l3 = sumGreen[ul.first-1][ul.second-1];

    return sumGreen[lr.first][lr.second]- l1 -l2 + l3;
      }
  if(channel == 'b'){
    if(!Pexist(lr.first,ul.second-1)) l1 = 0;
    else l1 = sumBlue[lr.first][ul.second-1];

    if(!Pexist(ul.first-1,lr.second)) l2 = 0;
    else l2 = sumBlue[ul.first-1][lr.second];

    if(!Pexist(ul.first-1,ul.second-1)) l3 = 0;
    else l3 = sumBlue[ul.first-1][ul.second-1];

    return sumBlue[lr.first][lr.second]- l1 -l2 + l3;
    }

}

long stats::getSumSq(char channel, pair<int,int> ul, pair<int,int> lr){
  long l1,l2,l3;
  if(channel == 'r'){
    if(!Pexist(lr.first,ul.second-1)) l1 = 0;
    else l1 = sumsqRed[lr.first][ul.second-1];

    if(!Pexist(ul.first-1,lr.second)) l2 = 0;
    else l2 = sumsqRed[ul.first-1][lr.second];

    if(!Pexist(ul.first-1,ul.second-1)) l3 = 0;
    else l3 = sumsqRed[ul.first-1][ul.second-1];

    return sumsqRed[lr.first][lr.second]- l1 -l2 + l3;
      }
  if(channel == 'g'){
    if(!Pexist(lr.first,ul.second-1)) l1 = 0;
    else l1 = sumsqGreen[lr.first][ul.second-1];

    if(!Pexist(ul.first-1,lr.second)) l2 = 0;
    else l2 = sumsqGreen[ul.first-1][lr.second];
    if(!Pexist(ul.first-1,ul.second-1)) l3 = 0;

    else l3 = sumsqGreen[ul.first-1][ul.second-1];

    return sumsqGreen[lr.first][lr.second]- l1 -l2 + l3;
      }
  if(channel == 'b'){
    if(!Pexist(lr.first,ul.second-1)) l1 = 0;
    else l1 = sumsqBlue[lr.first][ul.second-1];
    if(!Pexist(ul.first-1,lr.second)) l2 = 0;
    else l2 = sumsqBlue[ul.first-1][lr.second];
    if(!Pexist(ul.first-1,ul.second-1)) l3 = 0;
    else l3 = sumsqBlue[ul.first-1][ul.second-1];
    return sumsqBlue[lr.first][lr.second]- l1 -l2 + l3;
    }
}

stats::stats(PNG & im){
  unsigned int width = im.width();
  unsigned int height = im.height();

  for(unsigned int x = 0; x<width; x++){
    vector < long > rowR;
    vector < long > rowG;
    vector < long > rowB;
    vector < long > rowsqR;
    vector < long > rowsqG;
    vector < long > rowsqB;
    for(unsigned int y = 0; y<height; y++){
      RGBAPixel* pixel = im.getPixel(x,y);
      if(y==0 && x==0) {
       rowR.push_back(pixel->r);
       rowG.push_back(pixel->g);
       rowB.push_back(pixel->b);
       rowsqR.push_back(pow(pixel->r,2));
       rowsqG.push_back(pow(pixel->g,2));
       rowsqB.push_back(pow(pixel->b,2));
      }
     else if(x==0) {
       rowR.push_back(pixel->r + rowR[y-1]);
       rowG.push_back(pixel->g + rowG[y-1]);
       rowB.push_back(pixel->b + rowB[y-1]);
       rowsqR.push_back(pow(pixel->r,2) + rowsqR[y-1]);
       rowsqG.push_back(pow(pixel->g,2) + rowsqG[y-1]);
       rowsqB.push_back(pow(pixel->b,2) + rowsqB[y-1]);
     }
     else if(y==0) {
       rowR.push_back(pixel->r + sumRed[x-1][0]);
       rowG.push_back(pixel->g + sumGreen[x-1][0]);
       rowB.push_back(pixel->b + sumBlue[x-1][0]);
       rowsqR.push_back(pow(pixel->r,2) + sumsqRed[x-1][0]);
       rowsqG.push_back(pow(pixel->g,2) + sumsqGreen[x-1][0]);
       rowsqB.push_back(pow(pixel->b,2) + sumsqBlue[x-1][0]);
     }
     else {
       rowR.push_back(pixel->r + sumRed[x-1][y] + rowR[y-1] - sumRed[x-1][y-1]);
       rowG.push_back(pixel->g + sumGreen[x-1][y] + rowG[y-1] - sumGreen[x-1][y-1]);
       rowB.push_back(pixel->b + sumBlue[x-1][y] + rowB[y-1] - sumBlue[x-1][y-1]);
       rowsqR.push_back(pow(pixel->r,2) + sumsqRed[x-1][y] + rowsqR[y-1] - sumsqRed[x-1][y-1]);
       rowsqG.push_back(pow(pixel->g,2) + sumsqGreen[x-1][y] + rowsqG[y-1] - sumsqGreen[x-1][y-1]);
       rowsqB.push_back(pow(pixel->b,2) + sumsqBlue[x-1][y] + rowsqB[y-1] - sumsqBlue[x-1][y-1]);
     }
   }
   sumRed.push_back(rowR);
   sumGreen.push_back(rowG);
   sumBlue.push_back(rowB);
   sumsqRed.push_back(rowsqR);
   sumsqGreen.push_back(rowsqG);
   sumsqBlue.push_back(rowsqB);

      }
}

long stats::getScore(pair<int,int> ul, pair<int,int> lr){
  long r = getSumSq('r',ul,lr) - getSum('r',ul,lr)*getSum('r',ul,lr)/rectArea(ul,lr);
  long g = getSumSq('g',ul,lr) - getSum('g',ul,lr)*getSum('g',ul,lr)/rectArea(ul,lr);
  long b = getSumSq('b',ul,lr) - getSum('b',ul,lr)*getSum('b',ul,lr)/rectArea(ul,lr);
  return r+g+b;
}

RGBAPixel stats::getAvg(pair<int,int> ul, pair<int,int> lr){
  long r = getSum('r',ul,lr)/rectArea(ul,lr);
  long g = getSum('g',ul,lr)/rectArea(ul,lr);
  long b = getSum('b',ul,lr)/rectArea(ul,lr);
  RGBAPixel a = RGBAPixel(r,g,b);
  return a;
}

long stats::rectArea(pair<int,int> ul, pair<int,int> lr){
    return (lr.first - ul.first + 1) * (lr.second - ul.second + 1);
}

bool stats::Pexist(int a, int b){
    if(a<0||b<0) return false;
    return true;
}
