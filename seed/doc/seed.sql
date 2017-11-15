/*
Navicat MySQL Data Transfer

Source Server         : 192.168.3.145
Source Server Version : 50544
Source Host           : 192.168.3.145:3306
Source Database       : seed

Target Server Type    : MYSQL
Target Server Version : 50544
File Encoding         : 65001

Date: 2015-12-29 10:48:40
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `recover`
-- ----------------------------
DROP TABLE IF EXISTS `recover`;
CREATE TABLE `recover` (
  `id` char(255) NOT NULL DEFAULT '',
  `status` int(11) DEFAULT NULL,
  `result` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of recover
-- ----------------------------
