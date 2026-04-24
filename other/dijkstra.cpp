#include <iostream>
#include <climits>
#include <string>
#include <dijkstra.h>
using namespace std;

int dis[10], check[10], prevNode[10];   
string result;
string dijkstra(int cmatrix[][10], int start, int end) {

    for (int i = 0; i < 10; i++) {          //初始化
        dis[i] = INT_MAX;
        check[i] = 0;
        prevNode[i] = -1;        
    }
    dis[start] = 0;                          //起点dis设为0
   
    for (int count = 0; count < 10; count++) {
        int mindis = INT_MAX, minpoint = -1;
        for (int i = 0; i < 10; i++) {
            if (!check[i] && dis[i] < mindis) {
                mindis = dis[i];
                minpoint = i;
            }
        }
        if (minpoint == -1){ 
            break;                   // 走到头了
        }
        check[minpoint] = 1;         // dis最小的点check

      
        for (int j = 0; j < 10; j++) {                                                         //更新记录的dis
            if (cmatrix[minpoint][j] < INT_MAX && mindis + cmatrix[minpoint][j] < dis[j]) {
                dis[j] = mindis + cmatrix[minpoint][j];
                prevNode[j] = minpoint;   // 记录前一个节点(每个节点的编号指向下一个编号)
            }
        }
    }
    
    int path[10];   
    int index = 0;
    int temp = end;
    while (temp != -1) {
        path[index++] = temp;              //从终点开始往回推，直到回到最开始（minpoint=-1）的状态
        temp = prevNode[temp];
    }

    for (int i = index - 1; i >= 0; i--) {     //倒着把path输出一遍就是路径
        string r;
        r = path[i];
        result += r;
    }
    return r;
}
