#include<string>
#include<memory>
#include<chrono>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "sensor_msgs/msg/joint_state.hpp"
#include "geometry_msgs/msg/point.hpp"
#include<cmath>

class Kranode : public rclcpp::Node
{
    public:
    Kranode() : Node("kranode")
    {
    publisher_ = this->create_publisher<geometry_msgs::msg::Point>("end_effector_position", 10);
    geometry_msgs::msg::Point pmsg;
    pmsg.z=0;
    auto topic_callback =
        [this, &pmsg](sensor_msgs::msg::JointState::UniquePtr msg) -> void {
            pmsg.x=2*std::sin(msg->position[1]+msg->position[2])+ 1.5*std::sin(msg->position[1]);
            pmsg.y=2*std::cos(msg->position[1]+msg->position[2])+ 1.5*std::cos(msg->position[1]);
            RCLCPP_INFO(this->get_logger(), "Publishing:\nx cordinate:%lf\ny cordinate:%lf\n", pmsg.x,pmsg.y);
              this->publisher_->publish(pmsg);
          };

          subscription_ =
            this->create_subscription<sensor_msgs::msg::JointState>("joint_states", 10, topic_callback);



    }

    private:
      rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr subscription_;
      rclcpp::Publisher<geometry_msgs::msg::Point>::SharedPtr publisher_;


};


int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Kranode>());
  rclcpp::shutdown();
  return 0;
}
