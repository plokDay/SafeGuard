/*
 Navicat Premium Data Transfer

 Source Server         : mysql
 Source Server Type    : MySQL
 Source Server Version : 80017
 Source Host           : localhost:3306
 Source Schema         : safeguard

 Target Server Type    : MySQL
 Target Server Version : 80017
 File Encoding         : 65001

 Date: 18/10/2019 19:18:07
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for virus
-- ----------------------------
DROP TABLE IF EXISTS `virus`;
CREATE TABLE `virus`  (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(500) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `md5` varchar(500) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 16 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of virus
-- ----------------------------
INSERT INTO `virus` VALUES (86, '13扇子.doc', '3d3b8b8e64d04973d560011b4a557471');
INSERT INTO `virus` VALUES (87, '2017-02-17_1825.swf', '5d462f90ea3f3d8544f907129fe017ea');
INSERT INTO `virus` VALUES (88, '2017-02-17_19192222.swf', 'ccbedda5e84099f3946a93e00877e59e');
INSERT INTO `virus` VALUES (89, '3chang.fla', '5591c472a8855f540565b850ad17dc9e');
INSERT INTO `virus` VALUES (90, '6背景.fla1.fla', '25c458492fafa77d10c7ced1ff33caf1');
INSERT INTO `virus` VALUES (91, 'banner.fla', 'c151a4784e3333695c9f1f4276e7bc8b');
INSERT INTO `virus` VALUES (92, 'bird.fla', 'cbc03d24c339a7459ae99d917389aedb');
INSERT INTO `virus` VALUES (93, 'flash4765.swf', 'e4a04033eb4e9ab98d822e49ae776968');
INSERT INTO `virus` VALUES (94, 'hui.swf', 'b84dea89e1c163656b4d022cb6c4fda8');
INSERT INTO `virus` VALUES (95, 'ren.fla', 'e16f8f5bfa033228b0493a897618fa0e');
INSERT INTO `virus` VALUES (96, 'ren2.fla', '94be840c70239357f4c660373e402d57');
INSERT INTO `virus` VALUES (97, '中央人民广播电台少年广播合唱团', '-');
INSERT INTO `virus` VALUES (98, '快乐的节日', '00_00_01-00_00_33');
INSERT INTO `virus` VALUES (99, '00_00_00-00_00_25.mp3', '1e8fbd8af0c8075b2f0a23ff2fa22a29');
INSERT INTO `virus` VALUES (100, '快乐的节日', '00_00_01-00_00_33.mp3');
INSERT INTO `virus` VALUES (101, '938901d5301067db7ee2d4808b7944e9', '中央人民广播电台少年广播合唱团');
INSERT INTO `virus` VALUES (102, '-', '快乐的节日.mp3');
INSERT INTO `virus` VALUES (103, 'f0b35b9e4a49dc00bfdde626b56adfd8', '回家-2.fla');
INSERT INTO `virus` VALUES (104, '87d4dc5b8a2a3369c7e4a12d43a498d6', '回家的路.fla');
INSERT INTO `virus` VALUES (105, '54917426b1bc31149c277975e6c468b6', '回家的路.swf');
INSERT INTO `virus` VALUES (106, '48ed4ed9d0e206e15a2124a79a5953ca', '场景.psd');
INSERT INTO `virus` VALUES (107, '471670885af8443024face0db4dfa3ae', '完.fla');
INSERT INTO `virus` VALUES (108, '4afc667610aed4d2bcc6d8307a558140', '平移镜头.fla');
INSERT INTO `virus` VALUES (109, '6e5260b78a50eec91409bb1631ec8984', '平移镜头.swf');
INSERT INTO `virus` VALUES (110, '65106b9b967ca3deda5fe4d6b22ab7cf', '快乐的节日');
INSERT INTO `virus` VALUES (111, '00_00_24-00_00_33.mp3', 'e840790caf9abc4b3fc6f54d181bbd97');
INSERT INTO `virus` VALUES (112, '扇子.fla', '77e469f384ef3bf03c9cb13ccc0f6081');
INSERT INTO `virus` VALUES (113, '拉镜头.fla', '38e7505969deeeb439dd86bb4462f687');
INSERT INTO `virus` VALUES (114, '拉镜头.swf', '68358b03cb8f3958841cf5da185fa95a');
INSERT INTO `virus` VALUES (115, '推镜头.fla', '907d9e5dcfebf97f447caeebb8229012');
INSERT INTO `virus` VALUES (116, '推镜头.swf', '0ea56d98c0b237ab5dd6ac575039277b');
INSERT INTO `virus` VALUES (117, '示例.fla', 'c2f9b0b2e9c58ed6c28f168a12a21642');
INSERT INTO `virus` VALUES (118, '示例.swf', 'aa2d56873dc0a37e96dcddbefa03d992');
INSERT INTO `virus` VALUES (119, '上下直摇.fla', '753588700743d3580802cd3adf053dce');
INSERT INTO `virus` VALUES (120, '上下直摇.swf', 'b56b1727ac7a9d57125bc7f67c7fcba9');
INSERT INTO `virus` VALUES (121, '镜头右摇.fla', '0c8f19579ce9afe18eb650efc3f410f4');
INSERT INTO `virus` VALUES (122, '镜头右摇.swf', 'e5c413d47805c40f5f52b73acba0583e');
INSERT INTO `virus` VALUES (123, '旋转镜头.fla', '6145b3ec5bf7871a0dd671c8654f5bb8');
INSERT INTO `virus` VALUES (124, '旋转镜头.swf', '17afb21ade168a461626c3f9f002e8f9');
INSERT INTO `virus` VALUES (125, '时钟显示.fla', '8529cea99db1101b18c7cd215aa799ab');
INSERT INTO `virus` VALUES (126, '时间显示.fla', '5922dd74dc9b119934030415ed2bbbb7');
INSERT INTO `virus` VALUES (127, '未命名-1', '(1).fla');
INSERT INTO `virus` VALUES (128, 'ad4a5e6faaa8f23e1cc5304c57f8bd8e', '未命名-1');
INSERT INTO `virus` VALUES (129, '(1).swf', 'c5436278ad5f47d99437b802526833f1');
INSERT INTO `virus` VALUES (130, '未命名-1.fla', '31ea4e13325a11f3f8fd559da8695f9d');
INSERT INTO `virus` VALUES (131, '未命名-111.fla', '87e821ee9eda147ae4234d8491847c72');
INSERT INTO `virus` VALUES (132, '未命名-cs', '3-1.fla');
INSERT INTO `virus` VALUES (133, 'fb4f3ea5650b0ebea9b331b4db01d9ce', '未命名-cs');
INSERT INTO `virus` VALUES (134, '3-1.swf', '20b68a59a7a8bd6218dd1e60c739638e');
INSERT INTO `virus` VALUES (135, '树1.psd', 'ce93e140bb4316da352e2ab6284bcc7e');
INSERT INTO `virus` VALUES (136, '树2.psd', '6dcb9f53e7ec1c042cd5b914ff3a6901');
INSERT INTO `virus` VALUES (137, '网页制作实验考查试卷.doc', '0ee5ec71ee54e18586d62a983cb83e53');
INSERT INTO `virus` VALUES (138, '说明.htm', '4393f75e7770ca358ef82ec56a6dbea4');
INSERT INTO `virus` VALUES (139, '追踪镜头1.fla', '98528a073185afc57e140778cc410f33');
INSERT INTO `virus` VALUES (140, '追踪镜头1.swf', '551a43bf44435dab5b35d900bdfb2077');
INSERT INTO `virus` VALUES (141, '追踪镜头2.fla', 'dcfc19e93154b8b78710a7bce16d3b4c');
INSERT INTO `virus` VALUES (142, '追踪镜头2.swf', '3f75fa55756fa2964b62012abc794d5a');
INSERT INTO `virus` VALUES (143, '键盘相关事件.doc', 'e4b0395f4b44f69022b889da6d80fa02');
INSERT INTO `virus` VALUES (144, '化出化入.fla', 'd50f591ca97ad33a55ced48f7e64565f');
INSERT INTO `virus` VALUES (145, '化出化入.swf', 'ad41151a449dcafacce193d3afd0221a');
INSERT INTO `virus` VALUES (146, '圈入.fla', '68a00fe8e93f860c1e5294e39641b367');
INSERT INTO `virus` VALUES (147, '圈入.swf', 'e21f85536327c76cf97c6e1f7de32b96');
INSERT INTO `virus` VALUES (148, '圈出.fla', '9a1dd9f9043af133efe4e47aa88f97e4');
INSERT INTO `virus` VALUES (149, '圈出.swf', 'bb0a799b3b4865884bde43b8c077be09');
INSERT INTO `virus` VALUES (150, '淡入.fla', '8fd052483b83b54d020fc26c8879e8e2');
INSERT INTO `virus` VALUES (151, '淡入.swf', 'e23b411fc6ac431ffbae1b16b35057db');
INSERT INTO `virus` VALUES (152, '骨骼动画素材-cs6.rar', '7ae9815dc357ef975287b76169211cc1');
INSERT INTO `virus` VALUES (153, '鼠标跟随.fla', '090b5375449963d64ce389fcb0f58046');
INSERT INTO `virus` VALUES (154, '鼠标跟随2.fla', 'f4460a002943e1fa12cd9d96febd76a8');

SET FOREIGN_KEY_CHECKS = 1;
