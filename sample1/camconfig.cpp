#include <linux/videodev2.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <iostream>
#include <cstdio>
//#include <cstring>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <opencv2/opencv.hpp>

#define FFPP V4L2_PIX_FMT_SGRBG8

void *buffer;

cv::Mat frame(480, 640, CV_8UC1);
cv::Mat color(480, 1, CV_8UC3);

void* v4l2_tryCheck(int w, int h){
    int fd;
    fd = open("/dev/video0", O_RDWR);//読み書き
    if(fd < 0){
        std::cout << "Open Error! /dev/video0 " << std::endl;
        return NULL;
    }

    //スペック情報
    struct v4l2_capability caps;
    memset(&caps, 0, sizeof(caps));//たぶん初期化
    if(ioctl(fd, VIDIOC_QUERYCAP, &caps) != 0){
        std::cout << "Failed HW querycap." << std::endl; 
        return NULL;
    }  
    std::cout << "Bus:" << caps.bus_info << std::endl;
    std::cout << "card:" << caps.card << std::endl;
    std::cout << "driver:" << caps.driver << std::endl;
    std::cout << "version:" << caps.version << std::endl;


    //format request(SET)
    struct v4l2_format fmt;
    memset(&fmt, 0, sizeof(fmt));
    //5つ指定
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = w;
    fmt.fmt.pix.height = h;
    fmt.fmt.pix.pixelformat = FFPP; 
    fmt.fmt.pix.field = V4L2_FIELD_NONE;  
    //fmt.fmt.pix.field = V4L2_FIELD_ANY;
    if(ioctl(fd, VIDIOC_S_FMT, &fmt) != 0){
        std::cout << "Failed to format" << std::endl; 
        return NULL;
    }

    //Buffer request
    struct v4l2_requestbuffers req;
    memset(&(req), 0, sizeof(req));
    //3つ指定
    req.count = 1;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;
    if(ioctl(fd, VIDIOC_REQBUFS, &req) != 0){//ここが成功した場合，fmtで指定したサイズでバッファ確保
        std::cout << "Failed to buffer request" << std::endl; 
        return NULL;
    }

    //Buffer Query
    struct v4l2_buffer buf;
    memset(&buf, 0, sizeof(buf));
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = 0;//!!!    
    //buf.index = BUFINDEX;
        
    if(ioctl(fd, VIDIOC_QUERYBUF, &buf) != 0){
        std::cout << "Failed to query buffer" << std::endl; 
        return NULL;
    }
    std::cout << "buf.length : " << buf.length << std::endl;
    std::cout << "buf.m.offset : " << buf.m.offset << std::endl;
    buffer = mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);
    if(buffer == MAP_FAILED){
        printf("ユーザ空間へのマッピングが失敗しました．");
        return NULL;
    }

    //起動
    /*
    if(ioctl(fd, VIDIOC_STREAMON, &buf.type) != 0){
        std::cout << "Failed to query buffer" << std::endl; 
        return -1;
    }
    */
    
    //return 0;
    
//}



//int v4l2_tryCapture(int fd){

    //struct  v4l2_buffer buf;
    memset(&(buf), 0, sizeof(buf));
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if(ioctl(fd, VIDIOC_STREAMON, &buf.type) != 0){
        std::cout << "Failed to stream on" << std::endl; 
        return NULL;
    }

    std::cout << "ENQUEUE" << std::endl;
    //ビデオストリームに空バッファをエンキュ―
    //struct v4l2_buffer buf;
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = 0;
    while(1)
    {
        {
                //1.ビデオストリームに空バッファをエンキュ―
                if(ioctl(fd, VIDIOC_QBUF, &buf) != 0){
                        std::cout << "Failed to video qbuf" << std::endl;
                        return NULL;
                }
                fd_set fds;


                FD_ZERO(&fds);//fdsを空
                FD_SET(fd, &fds);//fdsにカメラデバイスのfdをセット

                //2.カメラドライバがバッファをフィルするまで待機
                //ここの処理時にEINTRなら再度実行するような記述は本来は書くべき

                struct timeval tv;
                tv.tv_sec = 2;


                int r = select(fd+1, &fds, NULL, NULL, &tv);
                if(r < 0){
                        std::cout << "Failed to select" << std::endl;
                        return NULL;
                }

                //3.ファイルをデキュー
                memset(&(buf), 0, sizeof(buf));
                buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                buf.memory = V4L2_MEMORY_MMAP;

                if(ioctl(fd, VIDIOC_DQBUF, &buf) < 0){
                        std::cout << "Failed to video dque" << std::endl;
                        return NULL;
                }

        }

        //data copy
        {
            memcpy(frame.data, buffer, 640*480);
            cv::cvtColor(frame, color, CV_BayerGB2BGR); 
        }

        // show
        {
            cv::imwrite("hogefuga.jpg", color);
            cv::imshow("img", color);
            cv::waitKey(1);    
        }

    }
    
    if(ioctl(fd, VIDIOC_STREAMOFF, &buf.type) < 0){
        std::cout << "fin" << std::endl;
    }

    return buffer;
}




