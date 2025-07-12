#include<iostream>
#include<string>
#include<memory>
#include<chrono>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/float64_multi_array.hpp"
#include "sensor_msgs/msg/joint_state.hpp"
#include "geometry_msgs/msg/point.hpp"
#include<cmath>

class Kranode : public rclcpp::Node
{
    public:
    Kranode() : Node("Kranode3")
    {
    publisher_ = this->create_publisher<std_msgs::msg::Float64MultiArray>("joint_angles_goal", 10);
    double x_cor,y_cor;
    auto topic_callback =
        [this, &x_cor ,&y_cor](geometry_msgs::msg::Point::UniquePtr msg) -> void {
            x_cor=msg->x;
            y_cor=msg->y;
            double angle1, angle2;
            double val;
            char option;


              std::cout<< "Current val of x= "<<x_cor<<"  Current val of y= "<<y_cor;
               std::cout<<"\nWhich cordinate to change- x or y:";
               std::cin>>option;
               if(option !='x' && option!='X' && option!='y' && option !='Y')
              {
                  std::cout<<"Invalid input!\n";
                  return;
              }
               std::cout<<"Enter distance to move end effector of chosen axis (0.5 to -0.5) :";
               std::cin>>val;
               if(val> 0.5 || val< -0.5)
               {
                   std::cout<<"Invalid value!\n";
                   return;
               }
               if(option=='x' || option=='X')
               {
                   if( ((x_cor+val)*(x_cor+val) + y_cor*y_cor> 3.5*3.5) || ((x_cor+val)*(x_cor+val) + y_cor*y_cor< 0.5*0.5) )
                   {
                       RCLCPP_WARN(this->get_logger(), "The given position is outside the arm's workspace\n");
                       return;
                   }
                   angle2=std::acos( ( (x_cor +val)*(x_cor+val) +y_cor*y_cor - 6.25 )/(6));
                   angle1=std::atan2(y_cor,(x_cor+val)) - atan2(1.5*std::sin(angle2) , 2+ 1.5*std::cos(angle2));

               }
               else {
                   if( ((y_cor+val)*(y_cor+val) + x_cor*x_cor> 3.5*3.5) || ((y_cor+val)*(y_cor+val) + x_cor*x_cor< 0.5*0.5) )
                   {
                       RCLCPP_WARN(this->get_logger(), "The given position is outside the arm's workspace\n");
                       return;
                   }
                   angle2=std::acos( ( (x_cor)*(x_cor) + (y_cor +val)*(y_cor+val) -6.25)/(6));
                   angle1=std::atan2((y_cor+val),x_cor) - atan2(1.5*std::sin(angle2) , 2+ 1.5*std::cos(angle2));

               RCLCPP_INFO(this->get_logger(), "Publishing:\ntheata1=%lf, theta2=%lf",angle1,angle2);
               std_msgs::msg::Float64MultiArray msg;
               msg.data={angle1,angle2};
               publisher_->publish(msg);
             }
          };
          subscription_ =
            this->create_subscription<geometry_msgs::msg::Point>("end_effector_position", 10, topic_callback);

    }



    private:
      rclcpp::Subscription<geometry_msgs::msg::Point>::SharedPtr subscription_;
      rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr publisher_;


};


int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Kranode>());
  rclcpp::shutdown();
  return 0;
}
