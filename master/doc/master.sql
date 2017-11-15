/*
Navicat MySQL Data Transfer

Source Server         : 192.168.3.145
Source Server Version : 50544
Source Host           : 192.168.3.145:3306
Source Database       : master

Target Server Type    : MYSQL
Target Server Version : 50544
File Encoding         : 65001

Date: 2016-01-14 14:49:53
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `ackInfo`
-- ----------------------------
DROP TABLE IF EXISTS `ackInfo`;
CREATE TABLE `ackInfo` (
  `task_id` char(255) NOT NULL DEFAULT '',
  `task_result` char(255) DEFAULT NULL,
  `useflag` int(11) DEFAULT NULL,
  `ecode` int(11) DEFAULT NULL,
  PRIMARY KEY (`task_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of ackInfo
-- ----------------------------
INSERT INTO `ackInfo` VALUES ('72878D77-801C-4D04-BF3F-12F68B7281600', 'http://192.168.3.145:3456/result', '0', '0');
INSERT INTO `ackInfo` VALUES ('72878D77-801C-4D04-BF3F-12F68B7281601', 'http://192.168.3.145:3456/result', '0', '1');
INSERT INTO `ackInfo` VALUES ('72878D77-801C-4D04-BF3F-12F68B7281602', 'http://192.168.3.145:3456/result', '0', '0');
INSERT INTO `ackInfo` VALUES ('72878D77-801C-4D04-BF3F-12F68B7281603', 'http://192.168.3.145:3456/result', '0', '0');
INSERT INTO `ackInfo` VALUES ('72878D77-801C-4D04-BF3F-12F68B7281604', 'http://192.168.3.145:3456/result', '0', '0');
INSERT INTO `ackInfo` VALUES ('72878D77-801C-4D04-BF3F-12F68B7281605', 'http://192.168.3.145:3456/result', '0', '0');
INSERT INTO `ackInfo` VALUES ('72878D77-801C-4D04-BF3F-12F68B7281606', 'http://192.168.3.145:3456/result', '0', '0');
INSERT INTO `ackInfo` VALUES ('72878D77-801C-4D04-BF3F-12F68B7281607', 'http://192.168.3.145:3456/result', '0', '0');
INSERT INTO `ackInfo` VALUES ('72878D77-801C-4D04-BF3F-12F68B7281608', 'http://192.168.3.145:3456/result', '0', '0');
INSERT INTO `ackInfo` VALUES ('72878D77-801C-4D04-BF3F-12F68B7281609', 'http://192.168.3.145:3456/result', '0', '1');
INSERT INTO `ackInfo` VALUES ('72878D77-801C-4D04-BF3F-12F68B7281610', 'http://192.168.3.145:3456/result', '0', '0');
INSERT INTO `ackInfo` VALUES ('72878D77-801C-4D04-BF3F-12F68B7281611', 'http://192.168.3.145:3456/result', '0', '0');
INSERT INTO `ackInfo` VALUES ('72878D77-801C-4D04-BF3F-12F68B7281612', 'http://192.168.3.145:3456/result', '0', '0');
INSERT INTO `ackInfo` VALUES ('72878D77-801C-4D04-BF3F-12F68B7281613', 'http://192.168.3.145:3456/result', '0', '0');
INSERT INTO `ackInfo` VALUES ('72878D77-801C-4D04-BF3F-12F68B7281614', 'http://192.168.3.145:3456/result', '0', '0');
INSERT INTO `ackInfo` VALUES ('72878D77-801C-4D04-BF3F-12F68B7281615', 'http://192.168.3.145:3456/result', '0', '0');
INSERT INTO `ackInfo` VALUES ('72878D77-801C-4D04-BF3F-12F68B7281616', 'http://192.168.3.145:3456/result', '0', '0');
INSERT INTO `ackInfo` VALUES ('72878D77-801C-4D04-BF3F-12F68B7281617', 'http://192.168.3.145:3456/result', '0', '0');
INSERT INTO `ackInfo` VALUES ('72878D77-801C-4D04-BF3F-12F68B7281618', 'http://192.168.3.145:3456/result', '0', '0');
INSERT INTO `ackInfo` VALUES ('72878D77-801C-4D04-BF3F-12F68B7281619', 'http://192.168.3.145:3456/result', '0', '0');

-- ----------------------------
-- Table structure for `dealInfo`
-- ----------------------------
DROP TABLE IF EXISTS `dealInfo`;
CREATE TABLE `dealInfo` (
  `task_id` char(255) NOT NULL,
  `taskinfo` char(255) DEFAULT '',
  `task_result` char(255) DEFAULT NULL,
  `seedinfo` char(255) DEFAULT NULL,
  `useflag` int(11) DEFAULT NULL,
  `dealstat` int(1) DEFAULT NULL,
  PRIMARY KEY (`task_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of dealInfo
-- ----------------------------

-- ----------------------------
-- Table structure for `seedService`
-- ----------------------------
DROP TABLE IF EXISTS `seedService`;
CREATE TABLE `seedService` (
  `seedinfo` char(255) NOT NULL DEFAULT '',
  `max_use` int(11) DEFAULT NULL,
  `cur_use` int(11) DEFAULT NULL,
  `useflag` int(11) DEFAULT NULL,
  PRIMARY KEY (`seedinfo`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of seedService
-- ----------------------------
INSERT INTO `seedService` VALUES ('192.168.3.145:1455', '2', '0', '0');

-- ----------------------------
-- Table structure for `seedType`
-- ----------------------------
DROP TABLE IF EXISTS `seedType`;
CREATE TABLE `seedType` (
  `seedinfo` char(255) NOT NULL DEFAULT '0',
  `seedflag` int(11) DEFAULT NULL,
  `num` int(11) DEFAULT NULL,
  PRIMARY KEY (`seedinfo`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of seedType
-- ----------------------------
INSERT INTO `seedType` VALUES ('transcode', '0', '1');

-- ----------------------------
-- Table structure for `test`
-- ----------------------------
DROP TABLE IF EXISTS `test`;
CREATE TABLE `test` (
  `url` char(255) NOT NULL DEFAULT '',
  `id` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`url`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of test
-- ----------------------------

-- ----------------------------
-- Table structure for `transInfo`
-- ----------------------------
DROP TABLE IF EXISTS `transInfo`;
CREATE TABLE `transInfo` (
  `task_id` char(255) NOT NULL DEFAULT '',
  `taskinfo` char(255) DEFAULT '',
  `task_result` char(255) DEFAULT NULL,
  `priority` int(11) DEFAULT NULL,
  `useflag` int(11) DEFAULT NULL,
  PRIMARY KEY (`task_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of transInfo
-- ----------------------------
