//dispaly pose tf origin is polhemus
#include <ros/ros.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <termios.h>
#include <iostream>
#include <geometry_msgs/PoseStamped.h>
#include <tf/transform_broadcaster.h>
#include <polhemus/serial_lib.h>
#define SIOFD_NAME_SIZE 516
#define RECV_DATA_SIZE 256
//~ #define SEND_RECV_SIZE   2
using namespace std;

int main(int argc, char **argv)
{
        int sci_fd, string_num, buff_num, data_num, loop_num=0;
        double num;
        int retval = -1;
        char sci_fd_name[SIOFD_NAME_SIZE];
        char buff[256];//readdata
        char tmp[10];
        //~ char send_buff[SEND_RECV_SIZE];
        double pose_data[8];// terminal_NO,position_x,y,z,quaternion_x,y,z,w
        memset(buff,0x00,RECV_DATA_SIZE);
        memset(sci_fd_name,0x00,SIOFD_NAME_SIZE);//initialize serial port
        strncpy(sci_fd_name,"/dev/ttyUSB0",SIOFD_NAME_SIZE);
        sci_fd = init_sci(sci_fd_name);
        
        if(sci_fd == -1){
            printf("init sci failed.\n");
            return retval;
        }
        
        //ROS
        ros::init(argc, argv, "polhemas_node");
        ros::NodeHandle n;
        cout << "Initializing node... " << endl;
        ros::Publisher pub_pose = n.advertise<geometry_msgs::PoseStamped>("fastrak_topic", 10);
        geometry_msgs::PoseStamped pub_msg;
        tf::TransformBroadcaster br;
        
        while(ros::ok()){
                string_num=0; buff_num=0; data_num=0;//initialize
                retval = str_get_sci(sci_fd,buff,RECV_DATA_SIZE);
                if(retval == -1){
                            printf("recv failed.\n");
                            return retval;
                }

                pub_msg.header.stamp = ros::Time::now();
                pub_msg.header.frame_id = "world";
                
               if(loop_num < 5){//wait some sec for stable
                   loop_num++;
               }
               else{
               do{
                            if(buff[buff_num] == '\0'){
                                tmp[string_num] ='\0';
                                num = atof(tmp);
                                pose_data[data_num]=num/100.0;
                                for(int k=0; k<10 ;k++){tmp[k]=0;}
                                string_num=0;
                                data_num=0;
                                break;
                            }
                            else if(buff[buff_num] == ' '){
                                tmp[string_num] ='\0';
                                num = atof(tmp);
                                pose_data[data_num]=num/100.0;
                                while(buff[buff_num+1] == ' '){buff_num++;}
                                for(int k=0; k<10 ;k++){tmp[k]=0;}
                                string_num=0;
                                data_num++;
                                buff_num++;
                            }
                            else{
                                tmp[string_num] =buff[buff_num];
                                string_num++;
                                buff_num++;
                            }
                    }while(1);
                    printf("%.2f %.2f %.2f %.2f %.2f %.2f %.2f\n",pose_data[1],pose_data[2],pose_data[3],pose_data[4],pose_data[5],pose_data[6],pose_data[7]);
                }
                
                pub_msg.pose.position.x = pose_data[1];
                pub_msg.pose.position.y = pose_data[2];
                pub_msg.pose.position.z = pose_data[3];
                pub_msg.pose.orientation.x = pose_data[4];
                pub_msg.pose.orientation.y = pose_data[5];
                pub_msg.pose.orientation.z = pose_data[6];
                pub_msg.pose.orientation.w = pose_data[7];
                pub_pose.publish(pub_msg);
                br.sendTransform(tf::StampedTransform(tf::Transform(
                tf::Quaternion(pub_msg.pose.orientation.x, pub_msg.pose.orientation.y, pub_msg.pose.orientation.z, pub_msg.pose.orientation.w),
                tf::Vector3(pub_msg.pose.position.x, pub_msg.pose.position.y, pub_msg.pose.position.z)
                ), pub_msg.header.stamp, "world", "fastrak"));
                
                ros::spinOnce();
        }
        close_sci(sci_fd);

        return retval;
}
