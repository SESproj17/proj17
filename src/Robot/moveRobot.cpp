#include "moveRobot.h"


moveRobot::moveRobot(int firstStart, int secondStart, int givenrobot_id, string givenLizi){
	canMove = false; //Is there a path where the robot can go
	canMoveObst = true;
	ros::NodeHandle  nh;
	this->lizi = givenLizi;
	publisher  = nh.advertise<geometry_msgs::Twist>("/lizi_"+lizi+"/diff_driver/command", 10);	
	steps_pub = nh.advertise<ses::step>("steps", 10);
	path_sub = nh.subscribe("paths", 1, &moveRobot::pathCallback, this);
	string scanString = "/lizi_" + lizi + "/scan";
	laserSub = nh.subscribe(scanString, 1, &moveRobot::scanCallback, this);
	me = new robot(firstStart, secondStart);
	robot_id = givenrobot_id;
	getPose();
	nh.getParam("startX",this->startPoseX);
	nh.getParam("startY",this->startPoseY);
	
}

// Process the incoming laser scan message
void moveRobot::scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan)
{
	bool isObstacleInFront = false;

    // Find the closest range between the defined minimum and maximum angles
    int minIndex = ceil((MIN_SCAN_ANGLE - scan->angle_min) / scan->angle_increment);
    int maxIndex = floor((MAX_SCAN_ANGLE - scan->angle_min) / scan->angle_increment);

    for (int currIndex = minIndex + 1; currIndex <= maxIndex; currIndex++) {
        if (scan->ranges[currIndex] < MIN_DIST_FROM_OBSTACLE) {
        	isObstacleInFront = true;
            break;
        }
    }

    if (isObstacleInFront) {
        ROS_INFO("Stop!");
	    for (int i = 0; i < 20; i++) {
			geometry_msgs::Twist stopCommand;
			stopCommand.linear.x = 0;
			publisher.publish(stopCommand);
			//cout << i << endl;
		}
        canMoveObst = false;
    } else {
    	canMoveObst = true;
    }
}

//Read and process new Path messages	
void moveRobot::pathCallback(const ses::Path::ConstPtr& path_msg){
	
	if(robot_id == path_msg->robot_id){
		cout<<"moveRobot::"<<robot_id<<" path: "<<path_msg->path<<endl;
		me->setState((robotState)path_msg->state);
		me->setPath(path_msg->path);
		me->setProbs(path_msg->probs);
		if((robotState)path_msg->state != done){
			canMove = true;
		}
	}
}


void moveRobot::publishStep(){
	
	ses::step step_msg;

	step_msg.robot_id = robot_id;
	step_msg.state = me->getState();
	step_msg.first_location = me->getLocation()->returnFirst();
	step_msg.second_location = me->getLocation()->returnSecond();
	step_msg.is_the_last = me->isTheLast();
	
	sleep(1.0);
	steps_pub.publish(step_msg);
	ros::spinOnce();
	
	ROS_INFO("Robot %d published on %d,%d step", robot_id, step_msg.first_location, step_msg.second_location);
}


void moveRobot::start(){
	publishStep();

	while (true) {
		ros::spinOnce();

		if(canMove && canMoveObst){
			
			// //debug code
			// if (me->getLocation()->returnFirst() == 0 && me->getLocation()->returnSecond() == 0 && lizi == "2") {
			// 	ROS_INFO("DIE IN HERE!");
			// 	exit(1);
			// }
			//debug code
			vector<myTuple*> path = me->getPath();
			int size = path.size();
			//if(size == 1){cout<<"moveRobot:problemmm of shorted path? "<<me->getLocation()->returnFirst()<<","<<me->getLocation()->returnSecond()<<endl;}
			ROS_INFO("DONT DIE!");
			cout << lizi << " with path size " << size << endl;
			int x = (path[0]->returnFirst() == me->getLocation()->returnFirst());
			int y = (path[0]->returnSecond()== me->getLocation()->returnSecond());
			
			if(size>1 && path[0]->returnFirst()== me->getLocation()->returnFirst() && path[0]->returnSecond()== me->getLocation()->returnSecond()){
				me->move();
				cout<<"before movment@"<<endl;
				moveToNext(path[0], path[1]);
				cout<<"after movement@"<<endl;
				publishStep();
				/*if(!me->imAlive()){
					ROS_INFO("Robot %d died ", robot_id);
					exit(0);
				}*/
				ros::spinOnce();
			}else{
				canMove = false;
			}
		}
	}
}

void moveRobot::step(Direction d){
	if (d == UP || d == DOWN) {
		stepUpDown(d);
		return;
	}
	stepLeftRight(d);
}


void moveRobot::stepLeftRight(Direction d) {
	getPose();
	int dir = 1;
	float goal;

	//cout << "moveRobot::stepUpDown dx " << me->getLocation()->returnSecond()*DXHorizontal << endl;
	//cout << "moveRobot::stepUpDown startPoseX " << currentLocationX << endl;
	goal = me->getLocation()->returnSecond()*DXHorizontal + this->startPoseX;
	float speed = 0.35;

	geometry_msgs::Twist rotateCommand;

	rotateCommand.linear.x = dir*speed;
	// How fast will we update the robot's movement
	ros::Rate rate(50);
	//debig code
	//cout << "moveRobot::goal " << goal << endl;
	//cout << "moveRobot::stepUpDown currentLocationY " << currentLocationX << endl;
	//cout << "moveRobot::stepUpDown tolarence " << abs(currentLocationX - goal) << endl;
	//debug code
	// Rotate until the robot reaches the target angle
	while (ros::ok() && abs(currentLocationX - goal) > placeTol * 70) {
		//cout << "moveRobot::stepUpDown currentLocationY " << currentLocationX << endl;
		publisher.publish(rotateCommand);
		rate.sleep();
		getPose();
	}

	////debug code
	//cout << "Angle refinement #1_UPDOWN" << endl;
	////debug code

	// Slow the speed near the target
	//rotateCommand.angular.z = turnLeft ? 0.1 * angularSpeed : -0.1 * angularSpeed;
	rotateCommand.linear.x = dir*speed*0.5;

	while (ros::ok() && abs(currentLocationX - goal) > placeTol * 40) {		

		publisher.publish(rotateCommand);
		rate.sleep();
		getPose();

		
	}
	////debug code
	//cout << "Angle refinement #2_UPDOWN" << endl;

	rotateCommand.linear.x = dir*speed*0.35;

	while (ros::ok() && abs(currentLocationX - goal) > placeTol * 10) {		

		publisher.publish(rotateCommand);
		rate.sleep();
		getPose();

		
	}
	////debug code
	//cout << "Angle refinement #2.5_UPDOWN" << endl;

	// Further refine the angle
	//rotateCommand.angular.z = turnLeft ? 0.05 * angularSpeed : -0.05 * angularSpeed;
	rotateCommand.linear.x = 0.2*dir*speed;
	while (ros::ok() && abs(currentLocationX - goal) > placeTol*3) {
		//cout << "moveRobot::stepUpDown currentLocationY " << currentLocationY << endl;
		publisher.publish(rotateCommand);
		rate.sleep();
		getPose();


	}
	////debug code
	//cout << "Angle refinement #3_UPDOWN" << endl;
	////debug code


	for (int i = 0; i < 20; i++) {
		geometry_msgs::Twist stopCommand;
		stopCommand.linear.x = 0;
		publisher.publish(stopCommand);
		//cout << i << endl;
	}
	//cout << "Angle refinement #4_UPDOWN" << endl;	
}



void moveRobot::stepUpDown(Direction d) {
	getPose();
	int dir = 1;
	float goal = 0;
	bool isUp = true;
	//if (d == UP) {dir = -1;isUp = false;}
	//cout << "moveRobot::stepUpDown dx " << me->getLocation()->returnFirst()*DX*(-1)*dir << endl;
	//cout << "moveRobot::" << lizi << "::" << "stepUpDown startPoseY " << startPoseY << endl;
	if (d == DOWN) {
		goal = me->getLocation()->returnFirst()*DX*(-1) + this->startPoseY;
	}else {
		goal = me->getLocation()->returnFirst()*DX + this->startPoseY;
	}
	
	float speed = 0.35;

	geometry_msgs::Twist rotateCommand;

	rotateCommand.linear.x = dir*speed;
	// How fast will we update the robot's movement
	ros::Rate rate(50);
	 cout << "moveRobot::goal " << lizi << ":: " << goal << endl;
	 cout << "moveRobot::stepUpDown currentLocationY " << lizi << ":: " <<  currentLocationY << endl;
	// cout << "moveRobot::stepUpDown tolarence " << abs(currentLocationY - goal) << endl;
	// Rotate until the robot reaches the target angle
	while (ros::ok() && abs(currentLocationY - goal) > placeTol * 70) {
		//cout << "moveRobot::stepUpDown currentLocationY " << currentLocationY << endl;
		publisher.publish(rotateCommand);
		rate.sleep();
		getPose();
	}

	////debug code
	//cout << "Angle refinement #1_UPDOWN" << endl;
	////debug code

	// Slow the speed near the target
	//rotateCommand.angular.z = turnLeft ? 0.1 * angularSpeed : -0.1 * angularSpeed;
	rotateCommand.linear.x = dir*speed*0.5;

	while (ros::ok() && abs(currentLocationY - goal) > placeTol * 40) {		

		publisher.publish(rotateCommand);
		rate.sleep();
		getPose();

		
	}
	////debug code
	//cout << "Angle refinement #2_UPDOWN" << endl;

	rotateCommand.linear.x = dir*speed*0.35;

	while (ros::ok() && abs(currentLocationY - goal) > placeTol * 10) {		

		publisher.publish(rotateCommand);
		rate.sleep();
		getPose();

		
	}
	////debug code
	//cout << "Angle refinement #2.5_UPDOWN" << endl;

	// Further refine the angle
	//rotateCommand.angular.z = turnLeft ? 0.05 * angularSpeed : -0.05 * angularSpeed;
	rotateCommand.linear.x = 0.2*dir*speed;
	while (ros::ok() && abs(currentLocationY - goal) > placeTol*3) {
		//cout << "moveRobot::stepUpDown currentLocationY " << currentLocationY << endl;
		publisher.publish(rotateCommand);
		rate.sleep();
		getPose();


	}
	////debug code
	//cout << "Angle refinement #3_UPDOWN" << endl;
	////debug code


	for (int i = 0; i < 20; i++) {
		geometry_msgs::Twist stopCommand;
		stopCommand.linear.x = 0;
		publisher.publish(stopCommand);
		//cout << i << endl;
	}
	//cout << "Angle refinement #4_UPDOWN" << endl;
}


void moveRobot::getPose() {
	tf::TransformListener listener;
	tf::StampedTransform transform;


	listener.waitForTransform("/map", "/lizi_"+lizi+"/base_link", ros::Time(0), ros::Duration(10.0) );

    try {
    	//listener.waitForTransform("/map", "/" + robotName + "/base_link", ros::Time(0), ros::Duration(60.0));
        listener.lookupTransform("/map",  "/lizi_"+lizi+"/base_link", ros::Time(0), transform);
        currentLocationX = transform.getOrigin().x();
        currentLocationY = transform.getOrigin().y();
        currentAngle = tf::getYaw(transform.getRotation());
    }
    catch (tf::TransformException & ex) {
        ROS_ERROR("%s", ex.what());
    } 
}


 void moveRobot::rotate(Direction d) {

	double directionAngles[] = { 0, M_PI / 2, M_PI, -M_PI/2 };
	double targetAngle = directionAngles[d];
	bool turnLeft;
	getPose();
	if (targetAngle - currentAngle > 0 && targetAngle - currentAngle < M_PI)
		turnLeft = true;
	else if (targetAngle - currentAngle < -M_PI)
		turnLeft = true;
	else
		turnLeft = false;


	geometry_msgs::Twist rotateCommand;
	rotateCommand.angular.z = turnLeft ? angularSpeed : -angularSpeed;

	// How fast will we update the robot's movement
	ros::Rate rate(50);

	// Rotate until the robot reaches the target angle
	while (ros::ok() && abs(currentAngle - targetAngle) > angularTolerance * 50) {

		// The robot can reach the LEFT direction from negative PI or positive PI
		if (d == LEFT && (abs(currentAngle - (-M_PI)) <= angularTolerance * 50))
		    break;

		publisher.publish(rotateCommand);
		rate.sleep();
		getPose();
	}

	////debug code
	//cout << "Angle refinement #1" << endl;
	////debug code

	// Slow the speed near the target
	//rotateCommand.angular.z = turnLeft ? 0.1 * angularSpeed : -0.1 * angularSpeed;
	rotateCommand.angular.z = turnLeft ? 0.5 * angularSpeed : -0.5 * angularSpeed;

	while (ros::ok() && abs(currentAngle - targetAngle) > angularTolerance * 10) {
		// The robot can reach the LEFT direction from negative PI or positive PI
		if (d == LEFT && (abs(currentAngle - (-M_PI)) <= angularTolerance * 10))
			break;
			

		publisher.publish(rotateCommand);
		rate.sleep();
		getPose();

		
	}
	////debug code
	//cout << "Angle refinement #2" << endl;
	////debug code

	// Further refine the angle
	//rotateCommand.angular.z = turnLeft ? 0.05 * angularSpeed : -0.05 * angularSpeed;
	while (ros::ok() && abs(currentAngle - targetAngle) > angularTolerance) {
		// The robot can reach the LEFT direction from negative PI or positive PI
		if (d == LEFT && (abs(currentAngle - (-M_PI)) <= angularTolerance))
			break;
		

		publisher.publish(rotateCommand);
		rate.sleep();
		getPose();


	}
	////debug code
	//cout << "Angle refinement #3" << endl;
	////debug code


	for (int i = 0; i < 20; i++) {
		geometry_msgs::Twist stopCommand;
		stopCommand.angular.z = 0;
		publisher.publish(stopCommand);
	}
	
}


 void moveRobot::moveToNext(myTuple* location, myTuple* nextLocation){
 	Direction d;
 	if (location->returnFirst() == nextLocation->returnFirst() && location->returnSecond() > nextLocation->returnSecond()) {
 		d = LEFT;
	} else if (location->returnFirst() == nextLocation->returnFirst() && location->returnSecond() < nextLocation->returnSecond()) {
		d = RIGHT;
	} else if (location->returnFirst() > nextLocation->returnFirst() && location->returnSecond() == nextLocation->returnSecond()) {
		d = UP;	
	} else if (location->returnFirst() < nextLocation->returnFirst() && location->returnSecond() == nextLocation->returnSecond()) {
		d = DOWN;						
	}
	////debug code
	//cout << "DIR " << d <<endl;
	////debug code
	rotate(d);
	step(d);

 }