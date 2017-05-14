; Auto-generated. Do not edit!


(cl:in-package multi_sync-msg)


;//! \htmlinclude step.msg.html

(cl:defclass <step> (roslisp-msg-protocol:ros-message)
  ((header
    :reader header
    :initarg :header
    :type std_msgs-msg:Header
    :initform (cl:make-instance 'std_msgs-msg:Header))
   (robot_id
    :reader robot_id
    :initarg :robot_id
    :type cl:integer
    :initform 0)
   (state
    :reader state
    :initarg :state
    :type cl:integer
    :initform 0)
   (first_location
    :reader first_location
    :initarg :first_location
    :type cl:integer
    :initform 0)
   (second_location
    :reader second_location
    :initarg :second_location
    :type cl:integer
    :initform 0)
   (is_the_last
    :reader is_the_last
    :initarg :is_the_last
    :type cl:boolean
    :initform cl:nil)
   (area
    :reader area
    :initarg :area
    :type cl:string
    :initform ""))
)

(cl:defclass step (<step>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <step>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'step)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name multi_sync-msg:<step> is deprecated: use multi_sync-msg:step instead.")))

(cl:ensure-generic-function 'header-val :lambda-list '(m))
(cl:defmethod header-val ((m <step>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader multi_sync-msg:header-val is deprecated.  Use multi_sync-msg:header instead.")
  (header m))

(cl:ensure-generic-function 'robot_id-val :lambda-list '(m))
(cl:defmethod robot_id-val ((m <step>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader multi_sync-msg:robot_id-val is deprecated.  Use multi_sync-msg:robot_id instead.")
  (robot_id m))

(cl:ensure-generic-function 'state-val :lambda-list '(m))
(cl:defmethod state-val ((m <step>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader multi_sync-msg:state-val is deprecated.  Use multi_sync-msg:state instead.")
  (state m))

(cl:ensure-generic-function 'first_location-val :lambda-list '(m))
(cl:defmethod first_location-val ((m <step>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader multi_sync-msg:first_location-val is deprecated.  Use multi_sync-msg:first_location instead.")
  (first_location m))

(cl:ensure-generic-function 'second_location-val :lambda-list '(m))
(cl:defmethod second_location-val ((m <step>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader multi_sync-msg:second_location-val is deprecated.  Use multi_sync-msg:second_location instead.")
  (second_location m))

(cl:ensure-generic-function 'is_the_last-val :lambda-list '(m))
(cl:defmethod is_the_last-val ((m <step>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader multi_sync-msg:is_the_last-val is deprecated.  Use multi_sync-msg:is_the_last instead.")
  (is_the_last m))

(cl:ensure-generic-function 'area-val :lambda-list '(m))
(cl:defmethod area-val ((m <step>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader multi_sync-msg:area-val is deprecated.  Use multi_sync-msg:area instead.")
  (area m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <step>) ostream)
  "Serializes a message object of type '<step>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'header) ostream)
  (cl:let* ((signed (cl:slot-value msg 'robot_id)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'state)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'first_location)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'second_location)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'is_the_last) 1 0)) ostream)
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'area))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'area))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <step>) istream)
  "Deserializes a message object of type '<step>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'header) istream)
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'robot_id) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'state) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'first_location) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'second_location) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:setf (cl:slot-value msg 'is_the_last) (cl:not (cl:zerop (cl:read-byte istream))))
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'area) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'area) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<step>)))
  "Returns string type for a message object of type '<step>"
  "multi_sync/step")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'step)))
  "Returns string type for a message object of type 'step"
  "multi_sync/step")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<step>)))
  "Returns md5sum for a message object of type '<step>"
  "68cd35e82dc4acec3d0b16f37c8d7b75")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'step)))
  "Returns md5sum for a message object of type 'step"
  "68cd35e82dc4acec3d0b16f37c8d7b75")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<step>)))
  "Returns full string definition for message of type '<step>"
  (cl:format cl:nil "Header header~%int32 robot_id~%int32 state~%int32 first_location~%int32 second_location~%bool is_the_last~%string area~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%# 0: no frame~%# 1: global frame~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'step)))
  "Returns full string definition for message of type 'step"
  (cl:format cl:nil "Header header~%int32 robot_id~%int32 state~%int32 first_location~%int32 second_location~%bool is_the_last~%string area~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%# 0: no frame~%# 1: global frame~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <step>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'header))
     4
     4
     4
     4
     1
     4 (cl:length (cl:slot-value msg 'area))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <step>))
  "Converts a ROS message object to a list"
  (cl:list 'step
    (cl:cons ':header (header msg))
    (cl:cons ':robot_id (robot_id msg))
    (cl:cons ':state (state msg))
    (cl:cons ':first_location (first_location msg))
    (cl:cons ':second_location (second_location msg))
    (cl:cons ':is_the_last (is_the_last msg))
    (cl:cons ':area (area msg))
))
