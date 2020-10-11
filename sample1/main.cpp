#include <iostream>
#include <opencv2/opencv.hpp>
#include "camconfig.hpp"


int main()
{
    unsigned char *fd;
    fd = (unsigned char*)v4l2_tryCheck(640, 480);
    //元々は関数を小分けにするつもりだったが大きくなった
    //こっちにmmapしたアドレスを返して処理を書くつもりがどうしてこうなった
    if(fd == NULL){
        std::cout << "tryCheck Error" << std::endl; 
    }
    
    std::cout << "OK" << std::endl; 

    return 0;


}
