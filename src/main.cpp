#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <cstring>
#include <climits>
#include <limits.h>
#include <string>
//#include "opencv/cv.h"
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
bool checkEdge(vector<map<int,int> > &vectMapTab, int i, int j, bool visited[]);
int fetchEdge(vector<map<int,int> > &vectMapTab, int i, int j);
void updateEdge(vector<map<int,int> > &vectMapTab, int i, int j,int path_flow,int check);
void fillImage(Mat &image,vector<vector<int> > &matrix);


void updateEdge(vector<map<int,int> > &vectMapTab, int i, int j,int path_flow,int check){
    map<int,int> :: iterator nested;
    for (nested = vectMapTab.at(i).begin(); nested != vectMapTab.at(i).end(); ++nested) {
        if(nested->first == j){
            if(check==1) {
                nested->second = (nested->second) - path_flow;
            }
            else if(check==0) {
                nested->second = (nested->second) + path_flow;
            }
        }
    }
}

int fetchEdge(vector<map<int,int> > &vectMapTab, int i, int j){
    map<int,int> :: iterator nested;
    for (nested = vectMapTab.at(i).begin(); nested != vectMapTab.at(i).end(); ++nested) {
        if(nested->first == j){
            return nested->second;
        }
    }

}

bool checkEdge(vector<map<int,int> > &vectMapTab, int i, int j, bool visited[]){
    bool edge = false;
    map<int,int> :: iterator nested;
    for (nested = vectMapTab.at(i).begin(); nested != vectMapTab.at(i).end(); ++nested) {

        if (nested->first == j && nested->second!=0) {
            edge = true;
            return true;
        }
    }
    return false;
}


int bfs(vector<map<int,int> > &rGraph, int s, int t, vector<int> &parent)
{
    bool visited[rGraph.size()];
    memset(visited, 0, sizeof(visited));


    queue <int> q;
    q.push(s);

    visited[s] = true;
    parent[s] = 0;

    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        map<int,int>::iterator v;
        for (v=rGraph.at(u).begin(); v!=rGraph.at(u).end(); v++)
        {
            if(u==t){return true;}

            if (visited[v->first]==false && checkEdge(rGraph,u,v->first,visited))
            {
                q.push(v->first);
                parent[v->first] = u;
                visited[v->first] = true;
            }
        }
    }

    return (visited[t] == true);
}

void bfsReachable(vector<map<int,int> > &rGraph, int s, bool *visited){
    visited[s] = true;

    queue <int> q;
    q.push(s);

    while(!q.empty()){
        int u =q.front();
        q.pop();

        //cout<<"In BFS"<<endl;
        map<int,int>::iterator v;
        for (v=rGraph.at(u).begin(); v!=rGraph.at(u).end(); v++)
        {
            //if(u==t){return true;}

            if (visited[v->first]==false && checkEdge(rGraph,u,v->first,visited))
            {
                q.push(v->first);
                //parent[v->first] = u;
                visited[v->first] = true;

            }
        }

    }
}

void coloring(Mat &img, bool *visited,vector<map<int,int> > &vectMapTab){
    for(int i=0;i<vectMapTab.size()-2;i++){
        Vec3b pixel;
        pixel[0] = 0;
        pixel[1] = 0;
        pixel[2] = 0;
        if(visited[i]==true){
            pixel[2] = 255;
            img.at<Vec3b>(i/img.cols,i%img.cols) = pixel;
        }
        else{
            pixel[0] = 255;
            img.at<Vec3b>(i/img.cols,i%img.cols) = pixel;
        }
    }
}

void minCut(Mat &img, vector<map<int,int> > &vectMapTab, int s, int t, string resultName)
{
    int u, v;

    vector<map<int,int> > rGraph;

    rGraph = vectMapTab;
    vector<int> parent(rGraph.size());  // This array is filled by BFS and to store path

    while (bfs(rGraph, s, t, parent))
    {
        int path_flow = INT_MAX/2;

        for (v=t; v!=s; v=parent[v])
        {
            u = parent[v];
            path_flow = min(path_flow, fetchEdge(rGraph,u,v));
        }

        for (v=t; v != s; v=parent[v])
        {
            u = parent[v];
            updateEdge(rGraph,u,v,path_flow,1); //-= path_flow;
            updateEdge(rGraph,v,u,path_flow,0); //+= path_flow;
        }
    }

    bool *visited= new bool[vectMapTab.size()];
    memset(visited, false, vectMapTab.size());
    bfsReachable(rGraph, s, visited);


    /*for (int i = 0; i < vectMapTab.size(); i++) {
        map<int,int> :: iterator it;
        for(it = vectMapTab.at(i).begin();it!=vectMapTab.at(i).end();it++){
            if (visited[i] && !visited[it->first] && checkEdge(vectMapTab,i,it->first,visited))
                cout << i << " - " << it->first << endl;
        }

    }*/
    cout<<"Coloring Image"<<endl;
    coloring(img,visited,vectMapTab);

    imwrite( resultName, img);

    /*imshow("Colored Image", img);
    while (waitKey(20) != 27);
    img.release();*/
    cout<<"Done!"<<endl;
    return;

}



void fillImage(Mat &matrix,vector<map<int,int> > &vectMapTab){
    for(int i=0;i<matrix.rows;i++){
        for(int j=0;j<matrix.cols;j++){
            if(i==0){
                if(j==0){
                    if(matrix.at<uchar>(i,j)==matrix.at<uchar>(i,j+1)){
                        map<int,int> maps;
                        vectMapTab.push_back(maps);
                        vectMapTab.at(i*matrix.cols+j)[i*matrix.cols+(j+1)] = INT_MAX/2;
                    }
                    else{
                        map<int,int> maps;
                        vectMapTab.push_back(maps);
                        (vectMapTab.at(i*matrix.cols+j))[i*matrix.cols+(j+1)]=50;
                    }
                    if(matrix.at<uchar>(i,j)==matrix.at<uchar>(i+1,j)){
                        map<int,int> maps;
                        vectMapTab.push_back(maps);
                        vectMapTab.at(i*matrix.cols+j)[(i+1)*matrix.cols+j]=INT_MAX/2;
                    }
                    else{
                        map<int,int> maps;
                        vectMapTab.push_back(maps);
                        vectMapTab.at(i*matrix.cols+j)[(i+1)*matrix.cols+j]=50;
                    }
                }
                else if(j==matrix.cols-1){
                    if(matrix.at<uchar>(i,j)==matrix.at<uchar>(i,j-1)){
                        map<int,int> maps;
                        vectMapTab.push_back(maps);
                        vectMapTab.at(i*matrix.cols+j)[i*matrix.cols+(j-1)]=INT_MAX/2;
                    }
                    else{
                        map<int,int> maps;
                        vectMapTab.push_back(maps);
                        vectMapTab.at(i*matrix.cols+j)[i*matrix.cols+(j-1)]=50;
                    }
                    if(matrix.at<uchar>(i,j)==matrix.at<uchar>(i+1,j)){
                        map<int,int> maps;
                        vectMapTab.push_back(maps);
                        vectMapTab.at(i*matrix.cols+j)[(i+1)*matrix.cols+j]=INT_MAX/2;
                    }
                    else{
                        map<int,int> maps;
                        vectMapTab.push_back(maps);
                        vectMapTab.at(i*matrix.cols+j)[(i+1)*matrix.cols+j]=50;
                    }
                }
                else{
                    if(matrix.at<uchar>(i,j)==matrix.at<uchar>(i,j-1)){
                        map<int,int> maps;
                        vectMapTab.push_back(maps);
                        vectMapTab.at(i*matrix.cols+j)[i*matrix.cols+(j-1)]=INT_MAX/2;
                    }
                    else{
                        map<int,int> maps;
                        vectMapTab.push_back(maps);
                        vectMapTab.at(i*matrix.cols+j)[i*matrix.cols+(j-1)]=50;
                    }
                    if(matrix.at<uchar>(i,j)==matrix.at<uchar>(i,j+1)){
                        map<int,int> maps;
                        vectMapTab.push_back(maps);
                        vectMapTab.at(i*matrix.cols+j)[i*matrix.cols+(j+1)]=INT_MAX/2;
                    }
                    else{
                        map<int,int> maps;
                        vectMapTab.push_back(maps);
                        vectMapTab.at(i*matrix.cols+j)[i*matrix.cols+(j+1)]=50;
                    }
                    if(matrix.at<uchar>(i,j)==matrix.at<uchar>(i+1,j)){
                        map<int,int> maps;
                        vectMapTab.push_back(maps);
                        vectMapTab.at(i*matrix.cols+j)[(i+1)*matrix.cols+j]=INT_MAX/2;
                    }
                    else{
                        map<int,int> maps;
                        vectMapTab.push_back(maps);
                        vectMapTab.at(i*matrix.cols+j)[(i+1)*matrix.cols+j]=50;
                    }
                }
            }
            else if(i==matrix.rows-1){
                if(j==0){
                    if(matrix.at<uchar>(i,j)==matrix.at<uchar>(i-1,j)){
                        map<int,int> maps;
                        vectMapTab.push_back(maps);
                        vectMapTab.at(i*matrix.cols+j)[(i-1)*matrix.cols+j]=INT_MAX/2;
                    }
                    else{
                        map<int,int> maps;
                        vectMapTab.push_back(maps);
                        vectMapTab.at(i*matrix.cols+j)[(i-1)*matrix.cols+j]=50;
                    }
                    if(matrix.at<uchar>(i,j)==matrix.at<uchar>(i,j+1)){
                        map<int,int> maps;
                        vectMapTab.push_back(maps);
                        vectMapTab.at(i*matrix.cols+j)[i*matrix.cols+(j+1)]=INT_MAX/2;
                    }
                    else{
                        map<int,int> maps;
                        vectMapTab.push_back(maps);
                        vectMapTab.at(i*matrix.cols+j)[i*matrix.cols+(j+1)]=50;
                    }
                }
                else if(j==matrix.cols-1){
                    if(matrix.at<uchar>(i,j)==matrix.at<uchar>(i-1,j)){
                        map<int,int> maps;
                        vectMapTab.push_back(maps);
                        vectMapTab.at(i*matrix.cols+j)[(i-1)*matrix.cols+j]=INT_MAX/2;
                    }
                    else{
                        map<int,int> maps;
                        vectMapTab.push_back(maps);
                        vectMapTab.at(i*matrix.cols+j)[(i-1)*matrix.cols+j]=50;
                    }
                    if(matrix.at<uchar>(i,j)==matrix.at<uchar>(i,j-1)){
                        map<int,int> maps;
                        vectMapTab.push_back(maps);
                        vectMapTab.at(i*matrix.cols+j)[i*matrix.cols+(j-1)]=INT_MAX/2;
                    }
                    else{
                        map<int,int> maps;
                        vectMapTab.push_back(maps);
                        vectMapTab.at(i*matrix.cols+j)[i*matrix.cols+(j-1)]=50;
                    }
                }
                else{
                    if(matrix.at<uchar>(i,j)==matrix.at<uchar>(i-1,j)){
                        map<int,int> maps;
                        vectMapTab.push_back(maps);
                        vectMapTab.at(i*matrix.cols+j)[(i-1)*matrix.cols+j]=INT_MAX/2;
                    }
                    else{
                        map<int,int> maps;
                        vectMapTab.push_back(maps);
                        vectMapTab.at(i*matrix.cols+j)[(i-1)*matrix.cols+j]=50;
                    }
                    if(matrix.at<uchar>(i,j)==matrix.at<uchar>(i,j+1)){
                        map<int,int> maps;
                        vectMapTab.push_back(maps);
                        vectMapTab.at(i*matrix.cols+j)[i*matrix.cols+(j+1)]=INT_MAX/2;
                    }
                    else{
                        map<int,int> maps;
                        vectMapTab.push_back(maps);
                        vectMapTab.at(i*matrix.cols+j)[i*matrix.cols+(j+1)]=50;
                    }
                    if(matrix.at<uchar>(i,j)==matrix.at<uchar>(i,j-1)){
                        map<int,int> maps;
                        vectMapTab.push_back(maps);
                        vectMapTab.at(i*matrix.cols+j)[i*matrix.cols+(j-1)]=INT_MAX/2;
                    }
                    else{
                        map<int,int> maps;
                        vectMapTab.push_back(maps);
                        vectMapTab.at(i*matrix.cols+j)[i*matrix.cols+(j-1)]=50;
                    }
                }
            }
            else{
                if (j == 0) {
                    if (matrix.at<uchar>(i,j) == matrix.at<uchar>(i - 1,j)) {
                        map<int,int> maps;
                        vectMapTab.push_back(maps);
                        vectMapTab.at(i*matrix.cols+j)[(i-1)*matrix.cols+j]=INT_MAX/2;
                    } else {
                        map<int,int> maps;
                        vectMapTab.push_back(maps);
                        vectMapTab.at(i*matrix.cols+j)[(i-1)*matrix.cols+j]=50;
                    }
                    if (matrix.at<uchar>(i,j) == matrix.at<uchar>(i,j + 1)) {
                        map<int,int> maps;
                        vectMapTab.push_back(maps);
                        vectMapTab.at(i*matrix.cols+j)[i*matrix.cols+(j+1)]=INT_MAX/2;
                    } else {
                        map<int,int> maps;
                        vectMapTab.push_back(maps);
                        vectMapTab.at(i*matrix.cols+j)[i*matrix.cols+(j+1)]=50;
                    }
                    if (matrix.at<uchar>(i,j) == matrix.at<uchar>(i + 1,j)) {
                        map<int,int> maps;
                        vectMapTab.push_back(maps);
                        vectMapTab.at(i*matrix.cols+j)[(i+1)*matrix.cols+j]=INT_MAX/2;
                    } else {
                        map<int,int> maps;
                        vectMapTab.push_back(maps);
                        vectMapTab.at(i*matrix.cols+j)[(i+1)*matrix.cols+j]=50;
                    }
                }
                else if (j == matrix.cols-1) {
                    if (matrix.at<uchar>(i,j) == matrix.at<uchar>(i - 1,j)) {
                        map<int,int> maps;
                        vectMapTab.push_back(maps);
                        vectMapTab.at(i*matrix.cols+j)[(i-1)*matrix.cols+j]=INT_MAX/2;
                    } else {
                        map<int,int> maps;
                        vectMapTab.push_back(maps);
                        vectMapTab.at(i*matrix.cols+j)[(i-1)*matrix.cols+j]=50;
                    }
                    if (matrix.at<uchar>(i,j) == matrix.at<uchar>(i,j - 1)) {
                        map<int,int> maps;
                        vectMapTab.push_back(maps);
                        vectMapTab.at(i*matrix.cols+j)[i*matrix.cols+(j-1)]=INT_MAX/2;
                    } else {
                        map<int,int> maps;
                        vectMapTab.push_back(maps);
                        vectMapTab.at(i*matrix.cols+j)[i*matrix.cols+(j-1)]=50;
                    }
                    if (matrix.at<uchar>(i,j) == matrix.at<uchar>(i + 1,j)) {
                        map<int,int> maps;
                        vectMapTab.push_back(maps);
                        vectMapTab.at(i*matrix.cols+j)[(i+1)*matrix.cols+j]=INT_MAX/2;
                    } else {
                        map<int,int> maps;
                        vectMapTab.push_back(maps);
                        vectMapTab.at(i*matrix.cols+j)[(i+1)*matrix.cols+j]=50;
                    }
                }

                else{
                    if (matrix.at<uchar>(i,j) == matrix.at<uchar>(i - 1,j)) {
                        map<int,int> maps;
                        vectMapTab.push_back(maps);
                        vectMapTab.at(i*matrix.cols+j)[(i-1)*matrix.cols+j]=INT_MAX/2;
                    } else {
                        map<int,int> maps;
                        vectMapTab.push_back(maps);
                        vectMapTab.at(i*matrix.cols+j)[(i-1)*matrix.cols+j]=50;
                    }
                    if (matrix.at<uchar>(i,j) == matrix.at<uchar>(i,j + 1)) {
                        map<int,int> maps;
                        vectMapTab.push_back(maps);
                        vectMapTab.at(i*matrix.cols+j)[i*matrix.cols+(j+1)]=INT_MAX/2;
                    } else {
                        map<int,int> maps;
                        vectMapTab.push_back(maps);
                        vectMapTab.at(i*matrix.cols+j)[i*matrix.cols+(j+1)]=50;
                    }
                    if (matrix.at<uchar>(i,j) == matrix.at<uchar>(i + 1,j)) {
                        map<int,int> maps;
                        vectMapTab.push_back(maps);
                        vectMapTab.at(i*matrix.cols+j)[(i+1)*matrix.cols+j]=INT_MAX/2;
                    } else {
                        map<int,int> maps;
                        vectMapTab.push_back(maps);
                        vectMapTab.at(i*matrix.cols+j)[(i+1)*matrix.cols+j]=50;
                    }
                    if (matrix.at<uchar>(i,j) == matrix.at<uchar>(i,j - 1)) {
                        map<int,int> maps;
                        vectMapTab.push_back(maps);
                        vectMapTab.at(i*matrix.cols+j)[i*matrix.cols+(j-1)]=INT_MAX/2;
                    } else {
                        map<int,int> maps;
                        vectMapTab.push_back(maps);
                        vectMapTab.at(i*matrix.cols+j)[i*matrix.cols+(j-1)]=50;
                    }
                }
            }


        }
    }
}

int main( int argc, char** argv )
{
    if(argc!=4){
        cout<<"Usage: ../seg input_image initialization_file output_mask"<<endl;
        return -1;
    }
    
    // Load the input image
    // the image should be a 3 channel image by default but we will double check that in teh seam_carving
    Mat in_image,BGRimg;
    in_image = imread(argv[1]/*"simple.png"*/);

    if(!in_image.data)
    {
        cout<<"Could not load input image!!!"<<endl;
        return -1;
    }

    if(in_image.channels()!=3){
        cout<<"Image does not have 3 channels!!! "<<in_image.depth()<<endl;
        return -1;
    }
    
    // the output image
    Mat out_image = in_image.clone();
    
    ifstream f(argv[2]/*"config.txt"*/);
    if(!f){
        cout<<"Could not load initial mask file!!!"<<endl;
        return -1;
    }
    
    int width = in_image.cols;
    int height = in_image.rows;
    
    int n;
    f>>n;
    vector<int> foreground;
    vector<int> background;
    int c1=0,c2=0;
    // get the initil pixels
    for(int i=0;i<n;++i){
        int x, y, t;
        f>>x>>y>>t;

        if(x<0 || x>=width || y<0 || y>=height){
            cout<<"I valid pixel mask!"<<endl;
            return -1;
        }
        
        
        Vec3b pixel;
        pixel[0] = 0;
        pixel[1] = 0;
        pixel[2] = 0;
        
        if(t==1){
            pixel[2] = 255;
        } else {
            pixel[0] = 255;
        }

        if(t==1){
            //foreground[c1] = y*width+x;
            foreground.push_back(y*width+x);
            c1++;
        }else{
            //background[c2] = y*width+x;
            background.push_back(y*width+x);
            c2++;
        }

        out_image.at<Vec3b>(y, x) = pixel;
    }

    cvtColor(in_image,BGRimg,CV_BGR2GRAY);


    vector<map<int,int> > vectMapTab;

    fillImage(BGRimg,vectMapTab);

    vectMapTab.resize((height*width)+2);

    for(int i=0;i<foreground.size();i++){


        vectMapTab.at(vectMapTab.size()-2)[foreground[i]] = INT_MAX/2;

        vectMapTab.at(foreground[i])[vectMapTab.size()-2] = INT_MAX/2;
    }

    for(int i=0;i<background.size();i++){


        vectMapTab.at(vectMapTab.size()-1)[background[i]] = INT_MAX/2;


        vectMapTab.at(background[i])[vectMapTab.size()-1] = INT_MAX/2;
    }

    string resultName = argv[3];
    minCut(in_image,vectMapTab,height*width,(height*width+1),resultName);

    return 0;
}
