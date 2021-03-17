/*
 * Copyright (C) 2013, Broadcom Corporation. All Rights Reserved. 
 *  
 * Permission to use, copy, modify, and/or distribute this software for any 
 * purpose with or without fee is hereby granted, provided that the above 
 * copyright notice and this permission notice appear in all copies. 
 *  
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES 
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY 
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES 
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION 
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN 
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
 */

unsigned int ddr_init_tab[] = {
	14, 0x00000001,
	36, 0x01000000,
	37, 0x10000000,
	38, 0x00100400,
	39, 0x00000400,
	40, 0x00000100,
	42, 0x00000001,
	61, 0x00010100,
	78, 0x01000200,
	79, 0x02000040,
	80, 0x00400100,
	81, 0x00000200,
	83, 0x01ffff0a,
	84, 0x01010101,
	85, 0x01010101,
	86, 0x0f000003,
	87, 0x0000010c,
	88, 0x00010000,
	112, 0x00000200,
	116, 0x0d000000,
	117, 0x00000028,
	119, 0x00010001,
	120, 0x00010001,
	121, 0x00010001,
	122, 0x00010001,
	123, 0x00010001,
	130, 0x00000001,
	139, 0x00000001,
	148, 0x00000001,
	149, 0x00000000,
	150, 0x00000000,
	152, 0x03030303,
	153, 0x03030303,
	156, 0x02006400,
	157, 0x02020202,
	158, 0x00020202,
	160, 0x01000000,
	161, 0x01010064,
	162, 0x01010101,
	163, 0x00000101,
	165, 0x00020000,
	166, 0x00000064,
	168, 0x000b0b00,
	170, 0x02000200,
	171, 0x02000200,
	175, 0x02000200,
	176, 0x02000200,
	180, 0x80000100,
	181, 0x04070303,
	182, 0x0000000a,
	185, 0x0010ffff,
	187, 0x0000000f,
	194, 0x00000204,
	205, 0x00000000,
	0xffffffff};

unsigned int ddr2_init_tab[] = {
		0,	0x0,
		0xffffffff
};

unsigned int ddr2_init_tab_667[] = { 
	0, 0x00000400,
	1, 0x00000000,
	2, 0x00000000,
	3, 0x00000043,
	4, 0x000000a7,
	5, 0x0a040a02,
	6, 0x04020404,
	7, 0x05030e12,
	8, 0x030e1204,
	9, 0x04020305,
	10, 0x03005b26,
	11, 0x04020303,
	12, 0x03005b26,
	13, 0x01010003,
	15, 0x000a0a00,
	16, 0x030000c8,
	17, 0x00860f0f,
	18, 0x06060086,
	19, 0x00000000,
	20, 0x00004201,
	21, 0x00420a24,
	22, 0x00050a24,
	23, 0x00000300,
	24, 0x000a0003,
	25, 0x0003000a,
	26, 0x0003000a,
	27, 0x00c8000a,
	28, 0x00c80046,
	29, 0x00000046,
	30, 0x03000001,
	31, 0x00030303,
	32, 0x00000000,
	33, 0x00000000,
	34, 0x00000000,
	35, 0x00000000,
	41, 0x00000000,
	43, 0x00000000,
	44, 0x00085300,
	45, 0x00000004,
	46, 0x00040853,
	47, 0x00000000,
	48, 0x08530000,
	49, 0x00000004,
	50, 0x00040853,
	51, 0x00000000,
	52, 0x08530000,
	53, 0x00000004,
	54, 0x00040853,
	55, 0x00000000,
	56, 0x08530000,
	57, 0x00000004,
	58, 0x00040853,
	59, 0x00000000,
	60, 0x00000000,
	62, 0x00000000,
	63, 0x00000000,
	64, 0x00000000,
	65, 0x00000000,
	66, 0x00000000,
	67, 0x00000000,
	68, 0x00000000,
	69, 0x00000000,
	70, 0x00000000,
	71, 0x00000000,
	72, 0x00000000,
	73, 0x00000000,
	74, 0x00000000,
	75, 0x00000000,
	76, 0x00000000,
	77, 0x00000000,
	82, 0x01020001,
	85, 0x01010101,
	86, 0x0f000003,
	89, 0x00000000,
	90, 0x00000000,
	91, 0x00000000,
	92, 0x00000000,
	93, 0x00000000,
	94, 0x00000000,
	95, 0x00000000,
	96, 0x00000000,
	97, 0x00000000,
	98, 0x00000000,
	99, 0x00000000,
	100, 0x00000000,
	101, 0x00000000,
	102, 0x00000000,
	103, 0x00000000,
	104, 0x00000000,
	105, 0x00000000,
	106, 0x00000000,
	107, 0x00000000,
	108, 0x02020101,
	109, 0x08080404,
	110, 0x03020000,
	111, 0x01000200,
	113, 0x00000000,
	114, 0x00000000,
	115, 0x00000000,
	118, 0x00000000,
	124, 0x00000000,
	125, 0x00000000,
	126, 0x00000000,
	127, 0x00000000,
	128, 0x001d1d00,
	129, 0x1d1d0001,
	131, 0x00000000,
	132, 0x00000000,
	133, 0x00011d1d,
	134, 0x00011d1d,
	135, 0x00000000,
	136, 0x00000000,
	137, 0x001d1d00,
	138, 0x1d1d0001,
	140, 0x00000000,
	141, 0x00000000,
	142, 0x00011d1d,
	143, 0x00011d1d,
	144, 0x00000000,
	145, 0x00000000,
	146, 0x001d1d00,
	147, 0x1d1d0001,
	151, 0x00000000,
	154, 0x00000000,
	155, 0x00000000,
	159, 0x00000000,
	164, 0x00000000,
	167, 0x00000000,
	169, 0x0a240000,
	172, 0x00000a24,
	173, 0x00006568,
	174, 0x0a240404,
	177, 0x00000a24,
	178, 0x00006568,
	179, 0x02020404,
	183, 0x00000000,
	184, 0x00000000,
	186, 0x00070303,
	188, 0x00000000,
	189, 0x00000000,
	190, 0x00000000,
	191, 0x00000000,
	192, 0x00000000,
	193, 0x00000000,
	195, 0x00000000,
	196, 0x00000000,
	197, 0x00000000,
	198, 0x00000000,
	199, 0x00000000,
	200, 0x00000000,
	201, 0x00000000,
	202, 0x0001046b,
	203, 0x0001046b,
	204, 0x00000000,
	206, 0x02030301,
	207, 0x00000002,
	208, 0x00000000,
	209, 0x00000000,
	210, 0x05050000,
	211, 0x00000505,
	212, 0x00000040,
	213, 0x00000000,
	214, 0x02010505,
	215, 0x00000102,
	216, 0x00001a1a,
	217, 0x00000000,
	0xffffffff
};

unsigned int ddr2_init_tab_800[] = { 
	0, 0x00000400,
	1, 0x00000000,
	2, 0x00000000,
	3, 0x00000050,
	4, 0x000000c8,
	5, 0x0c050c02,
	6, 0x04020405,
	7, 0x06031016,
	8, 0x03101604,
	9, 0x05020306,
	10, 0x03006d60,
	11, 0x05020303,
	12, 0x03006d60,
	13, 0x01010003,
	15, 0x000c0c00,
	16, 0x030000c8,
	17, 0x00a01212,
	18, 0x070700a0,
	19, 0x00000000,
	20, 0x00004f01,
	21, 0x004f0c2d,
	22, 0x00050c2d,
	23, 0x00000300,
	24, 0x000a0003,
	25, 0x0003000a,
	26, 0x0003000a,
	27, 0x00c8000a,
	28, 0x00c80053,
	29, 0x00000053,
	30, 0x03000001,
	31, 0x00030303,
	32, 0x00000000,
	33, 0x00000000,
	34, 0x00000000,
	35, 0x00000000,
	41, 0x00000000,
	43, 0x00000000,
	44, 0x000a6300,
	45, 0x00000004,
	46, 0x00040a63,
	47, 0x00000000,
	48, 0x0a630000,
	49, 0x00000004,
	50, 0x00040a63,
	51, 0x00000000,
	52, 0x0a630000,
	53, 0x00000004,
	54, 0x00040a63,
	55, 0x00000000,
	56, 0x0a630000,
	57, 0x00000004,
	58, 0x00040a63,
	59, 0x00000000,
	60, 0x00000000,
	62, 0x00000000,
	63, 0x00000000,
	64, 0x00000000,
	65, 0x00000000,
	66, 0x00000000,
	67, 0x00000000,
	68, 0x00000000,
	69, 0x00000000,
	70, 0x00000000,
	71, 0x00000000,
	72, 0x00000000,
	73, 0x00000000,
	74, 0x00000000,
	75, 0x00000000,
	76, 0x00000000,
	77, 0x00000000,
	82, 0x01020001,
	85, 0x01010101,
	86, 0x0f000003,
	89, 0x00000000,
	90, 0x00000000,
	91, 0x00000000,
	92, 0x00000000,
	93, 0x00000000,
	94, 0x00000000,
	95, 0x00000000,
	96, 0x00000000,
	97, 0x00000000,
	98, 0x00000000,
	99, 0x00000000,
	100, 0x00000000,
	101, 0x00000000,
	102, 0x00000000,
	103, 0x00000000,
	104, 0x00000000,
	105, 0x00000000,
	106, 0x00000000,
	107, 0x00000000,
	108, 0x02020101,
	109, 0x08080404,
	110, 0x03020000,
	111, 0x01000200,
	113, 0x00000000,
	114, 0x00000000,
	115, 0x00000000,
	118, 0x00000000,
	124, 0x00000000,
	125, 0x00000000,
	126, 0x00000000,
	127, 0x00000000,
	128, 0x001d1d00,
	129, 0x1d1d0001,
	131, 0x00000000,
	132, 0x00000000,
	133, 0x00011d1d,
	134, 0x00011d1d,
	135, 0x00000000,
	136, 0x00000000,
	137, 0x001d1d00,
	138, 0x1d1d0001,
	140, 0x00000000,
	141, 0x00000000,
	142, 0x00011d1d,
	143, 0x00011d1d,
	144, 0x00000000,
	145, 0x00000000,
	146, 0x001d1d00,
	147, 0x1d1d0001,
	151, 0x00000000,
	154, 0x00000000,
	155, 0x00000000,
	159, 0x00000000,
	164, 0x00000000,
	167, 0x00000000,
	169, 0x0c2d0000,
	172, 0x00000c2d,
	173, 0x000079c2,
	174, 0x0c2d0505,
	177, 0x00000c2d,
	178, 0x000079c2,
	179, 0x02020505,
	183, 0x00000000,
	184, 0x00000000,
	186, 0x00070303,
	188, 0x00000000,
	189, 0x00000000,
	190, 0x00000000,
	191, 0x00000000,
	192, 0x00000000,
	193, 0x00000000,
	195, 0x00000000,
	196, 0x00000000,
	197, 0x00000000,
	198, 0x00000000,
	199, 0x00000000,
	200, 0x00000000,
	201, 0x00000000,
	202, 0x00013880,
	203, 0x00013880,
	204, 0x00000000,
	206, 0x02030301,
	207, 0x00000002,
	208, 0x00000000,
	209, 0x00000000,
	210, 0x06060000,
	211, 0x00000606,
	212, 0x00000040,
	213, 0x00000000,
	214, 0x02010505,
	215, 0x00000102,
	216, 0x00001c1c,
	217, 0x00000000,
	0xffffffff
};

unsigned int ddr2_init_tab_1066[] = { 
	0, 0x00000400,
	1, 0x00000000,
	2, 0x00000000,
	3, 0x0000006b,
	4, 0x0000010a,
	5, 0x0e060e02,
	6, 0x06020406,
	7, 0x0704161d,
	8, 0x04161d06,
	9, 0x07020407,
	10, 0x03009173,
	11, 0x07020403,
	12, 0x03009173,
	13, 0x01010003,
	15, 0x000f0f00,
	16, 0x030000c8,
	17, 0x00d51818,
	18, 0x080800d5,
	19, 0x00000000,
	20, 0x00006a01,
	21, 0x006a1033,
	22, 0x00051033,
	23, 0x00000300,
	24, 0x000d0003,
	25, 0x0003000d,
	26, 0x0003000a,
	27, 0x00c8000a,
	28, 0x00c8006f,
	29, 0x0000006f,
	30, 0x03000001,
	31, 0x00030303,
	32, 0x00000000,
	33, 0x00000000,
	34, 0x00000000,
	35, 0x00000000,
	41, 0x00000000,
	43, 0x00000000,
	44, 0x000e7300,
	45, 0x00000004,
	46, 0x00040e73,
	47, 0x00000000,
	48, 0x0e730000,
	49, 0x00000004,
	50, 0x00040e73,
	51, 0x00000000,
	52, 0x0e730000,
	53, 0x00000004,
	54, 0x00040e73,
	55, 0x00000000,
	56, 0x0e730000,
	57, 0x00000004,
	58, 0x00040e73,
	59, 0x00000000,
	60, 0x00000000,
	62, 0x00000000,
	63, 0x00000000,
	64, 0x00000000,
	65, 0x00000000,
	66, 0x00000000,
	67, 0x00000000,
	68, 0x00000000,
	69, 0x00000000,
	70, 0x00000000,
	71, 0x00000000,
	72, 0x00000000,
	73, 0x00000000,
	74, 0x00000000,
	75, 0x00000000,
	76, 0x00000000,
	77, 0x00000000,
	82, 0x01020001,
	85, 0x01010101,
	86, 0x0f000003,
	89, 0x00000000,
	90, 0x00000000,
	91, 0x00000000,
	92, 0x00000000,
	93, 0x00000000,
	94, 0x00000000,
	95, 0x00000000,
	96, 0x00000000,
	97, 0x00000000,
	98, 0x00000000,
	99, 0x00000000,
	100, 0x00000000,
	101, 0x00000000,
	102, 0x00000000,
	103, 0x00000000,
	104, 0x00000000,
	105, 0x00000000,
	106, 0x00000000,
	107, 0x00000000,
	108, 0x02020101,
	109, 0x08080404,
	110, 0x03020000,
	111, 0x01000200,
	113, 0x00000000,
	114, 0x00000000,
	115, 0x00000000,
	118, 0x00000000,
	124, 0x00000000,
	125, 0x00000000,
	126, 0x00000000,
	127, 0x00000000,
	128, 0x001c1c00,
	129, 0x1c1c0001,
	131, 0x00000000,
	132, 0x00000000,
	133, 0x00011c1c,
	134, 0x00011c1c,
	135, 0x00000000,
	136, 0x00000000,
	137, 0x001c1c00,
	138, 0x1c1c0001,
	140, 0x00000000,
	141, 0x00000000,
	142, 0x00011c1c,
	143, 0x00011c1c,
	144, 0x00000000,
	145, 0x00000000,
	146, 0x001c1c00,
	147, 0x1c1c0001,
	151, 0x00000000,
	154, 0x00000000,
	155, 0x00000000,
	159, 0x00000000,
	164, 0x00000000,
	167, 0x00000000,
	169, 0x10330000,
	172, 0x00001033,
	173, 0x0000a1fe,
	174, 0x10330606,
	177, 0x00001033,
	178, 0x0000a1fe,
	179, 0x02020606,
	183, 0x00000000,
	184, 0x00000000,
	186, 0x00070303,
	188, 0x00000000,
	189, 0x00000000,
	190, 0x00000000,
	191, 0x00000000,
	192, 0x00000000,
	193, 0x00000000,
	195, 0x00000000,
	196, 0x00000000,
	197, 0x00000000,
	198, 0x00000000,
	199, 0x00000000,
	200, 0x00000000,
	201, 0x00000000,
	202, 0x00019f8f,
	203, 0x00019f8f,
	204, 0x00000000,
	206, 0x02030301,
	207, 0x00000002,
	208, 0x00000000,
	209, 0x00000000,
	210, 0x08070000,
	211, 0x00000807,
	212, 0x00000040,
	213, 0x00000000,
	214, 0x02010505,
	215, 0x00000102,
	216, 0x00001e1e,
	217, 0x00000000,
	0xffffffff
};

unsigned int ddr3_init_tab_667[] = { 
	0, 0x00000600,
	1, 0x00000000,
	3, 0x00000043,
	4, 0x000000a7,
	5, 0x0a050a00,
	6, 0x04040405,
	7, 0x05040e14,
	8, 0x040e1404,
	9, 0x0c040405,
	10, 0x03005b68,
	11, 0x0c040404,
	12, 0x03005b68,
	13, 0x01010004,
	15, 0x000a0a00,
	16, 0x03000200,
	17, 0x00000f0f,
	18, 0x05050000,
	19, 0x00000000,
	20, 0x00007801,
	21, 0x00780a20,
	22, 0x00050a20,
	23, 0x00000300,
	24, 0x000a0003,
	25, 0x0000000a,
	26, 0x00000000,
	27, 0x02000000,
	28, 0x0200005a,
	29, 0x0000005a,
	30, 0x05000001,
	31, 0x00050505,
	32, 0x00000000,
	35, 0x00000000,
	41, 0x00000000,
	43, 0x00000000,
	44, 0x00021000,
	45, 0x00000046,
	46, 0x00460210,
	47, 0x00000000,
	48, 0x02100000,
	49, 0x00000046,
	50, 0x00460210,
	51, 0x00000000,
	52, 0x02100000,
	53, 0x00000046,
	54, 0x00460210,
	55, 0x00000000,
	56, 0x02100000,
	57, 0x00000046,
	58, 0x00460210,
	59, 0x00000000,
	60, 0x00000000,
	62, 0x00000000,
	63, 0x00000000,
	64, 0x00000000,
	65, 0x00000000,
	66, 0x00000000, /* No ECC, set to 0x3 for ECC reporting and correcting */
	67, 0x00000000,
	68, 0x00000000,
	69, 0x00000000,
	70, 0x00000000,
	71, 0x00000000,
	72, 0x00000000,
	73, 0x00000000,
	74, 0x00000000,
	75, 0x00000000,
	76, 0x00000000,
	77, 0x00000000,
	82, 0x01010001,
	89, 0x00000000,
	90, 0x00000000,
	91, 0x00000000,
	92, 0x00000000,
	93, 0x00000000,
	94, 0x00000000,
	95, 0x00000000,
	96, 0x00000000,
	97, 0x00000000,
	98, 0x00000000,
	99, 0x00000000,
	100, 0x00000000,
	101, 0x00000000,
	102, 0x00000000,
	103, 0x00000000,
	104, 0x00000000,
	105, 0x00000000,
	106, 0x00000000,
	107, 0x00000000,
	108, 0x02040108,
	109, 0x08010402,
	110, 0x02020002,
	111, 0x01000200,
	113, 0x00000000,
	114, 0x00000000,
	115, 0x00000000,
	118, 0x00000000,
	124, 0x00000000,
	125, 0x00000000,
	126, 0x00000000,
	127, 0x00000000,
	128, 0x00212100,
	129, 0x21210001,
	131, 0x00000000,
	132, 0x00000000,
	133, 0x00012121,
	134, 0x00012121,
	135, 0x00000000,
	136, 0x00000000,
	137, 0x00212100,
	138, 0x21210001,
	140, 0x00000000,
	141, 0x00000000,
	142, 0x00012121,
	143, 0x00012121,
	144, 0x00000000,
	145, 0x00000000,
	146, 0x00212100,
	147, 0x21210001,
	151, 0x00000000,
	167, 0x00000000,
	169, 0x0a200000,
	172, 0x00000a20,
	173, 0x00006540,
	174, 0x0a200504,
	177, 0x00000a20,
	178, 0x00006540,
	179, 0x02020504,
	183, 0x00000000,
	184, 0x00000000,
	186, 0x00070303,
	188, 0x00000000,
	189, 0x00000000,
	190, 0x00000000,
	191, 0x00000000,
	192, 0x00000000,
	193, 0x00000000,
	195, 0x00000000,
	196, 0x00000000,
	197, 0x00000000,
	198, 0x00000000,
	199, 0x00000000,
	200, 0x00000000,
	201, 0x00000000,
	202, 0x00000004,
	203, 0x00000004,
	204, 0x00000000,
	206, 0x02040401,
	207, 0x00000002,
	208, 0x00000000,
	209, 0x00000000,
	210, 0x05050000,
	211, 0x00000505,
	212, 0x00000040,
	213, 0x00000000,
	214, 0x01010606,
	215, 0x00000101,
	216, 0x00001a1a,
	217, 0x00000000,
	0xffffffff};

unsigned int ddr3_init_tab_800[] = { 
	0, 0x00000600,
	1, 0x00000000,
	3, 0x00000050,
	4, 0x000000c8,
	5, 0x0c050c00,
	6, 0x04040405,
	7, 0x06041018,
	8, 0x04101804,
	9, 0x0c040406,
	10, 0x03006db0,
	11, 0x0c040404,
	12, 0x03006db0,
	13, 0x01010004,
	15, 0x000c0c00,
	16, 0x03000200,
	17, 0x00001212,
	18, 0x06060000,
	19, 0x00000000,
	20, 0x00009001,
	21, 0x00900c28,
	22, 0x00050c28,
	23, 0x00000300,
	24, 0x000a0003,
	25, 0x0000000a,
	26, 0x00000000,
	27, 0x02000000,
	28, 0x0200006c,
	29, 0x0000006c,
	30, 0x05000001,
	31, 0x00050505,
	32, 0x00000000,
	35, 0x00000000,
	41, 0x00000000,
	43, 0x00000000,
	44, 0x00042000,
	45, 0x00000046,
	46, 0x00460420,
	47, 0x00000000,
	48, 0x04200000,
	49, 0x00000046,
	50, 0x00460420,
	51, 0x00000000,
	52, 0x04200000,
	53, 0x00000046,
	54, 0x00460420,
	55, 0x00000000,
	56, 0x04200000,
	57, 0x00000046,
	58, 0x00460420,
	59, 0x00000000,
	60, 0x00000000,
	62, 0x00000000,
	63, 0x00000000,
	64, 0x00000000,
	65, 0x00000000,
	66, 0x00000000, /* No ECC, set to 0x3 for ECC reporting and correcting */
	67, 0x00000000,
	68, 0x00000000,
	69, 0x00000000,
	70, 0x00000000,
	71, 0x00000000,
	72, 0x00000000,
	73, 0x00000000,
	74, 0x00000000,
	75, 0x00000000,
	76, 0x00000000,
	77, 0x00000000,
	82, 0x01010001,
	89, 0x00000000,
	90, 0x00000000,
	91, 0x00000000,
	92, 0x00000000,
	93, 0x00000000,
	94, 0x00000000,
	95, 0x00000000,
	96, 0x00000000,
	97, 0x00000000,
	98, 0x00000000,
	99, 0x00000000,
	100, 0x00000000,
	101, 0x00000000,
	102, 0x00000000,
	103, 0x00000000,
	104, 0x00000000,
	105, 0x00000000,
	106, 0x00000000,
	107, 0x00000000,
	108, 0x02040108,
	109, 0x08010402,
	110, 0x02020002,
	111, 0x01000200,
	113, 0x00000000,
	114, 0x00000000,
	115, 0x00000000,
	118, 0x00000000,
	124, 0x00000000,
	125, 0x00000000,
	126, 0x00000000,
	127, 0x00000000,
	128, 0x00212100,
	129, 0x21210001,
	131, 0x00000000,
	132, 0x00000000,
	133, 0x00012121,
	134, 0x00012121,
	135, 0x00000000,
	136, 0x00000000,
	137, 0x00212100,
	138, 0x21210001,
	140, 0x00000000,
	141, 0x00000000,
	142, 0x00012121,
	143, 0x00012121,
	144, 0x00000000,
	145, 0x00000000,
	146, 0x00212100,
	147, 0x21210001,
	151, 0x00000000,
	167, 0x00000000,
	169, 0x0c280000,
	172, 0x00000c28,
	173, 0x00007990,
	174, 0x0c280505,
	177, 0x00000c28,
	178, 0x00007990,
	179, 0x02020505,
	183, 0x00000000,
	184, 0x00000000,
	186, 0x00070303,
	188, 0x00000000,
	189, 0x00000000,
	190, 0x00000000,
	191, 0x00000000,
	192, 0x00000000,
	193, 0x00000000,
	195, 0x00000000,
	196, 0x00000000,
	197, 0x00000000,
	198, 0x00000000,
	199, 0x00000000,
	200, 0x00000000,
	201, 0x00000000,
	202, 0x00000004,
	203, 0x00000004,
	204, 0x00000000,
	206, 0x02040401,
	207, 0x00000002,
	208, 0x00000000,
	209, 0x00000000,
	210, 0x06060000,
	211, 0x00000606,
	212, 0x00000040,
	213, 0x00000000,
	214, 0x01010606,
	215, 0x00000101,
	216, 0x00001c1c,
	217, 0x00000000,
	0xffffffff};

unsigned int ddr3_init_tab_1066[] = { 
	0, 0x00000600,
	1, 0x00000000,
	3, 0x0000006b,
	4, 0x0000010a,
	5, 0x10061000,
	6, 0x04040406,
	7, 0x0804131a,
	8, 0x04131a04,
	9, 0x0c040408,
	10, 0x030091dd,
	11, 0x0c040404,
	12, 0x030091dd,
	13, 0x01010004,
	15, 0x00101000,
	16, 0x03000200,
	17, 0x00001616,
	18, 0x08080000,
	19, 0x00000000,
	20, 0x0000a001,
	21, 0x00a0102d,
	22, 0x0005102d,
	23, 0x00000400,
	24, 0x000d0004,
	25, 0x0000000d,
	26, 0x00000000,
	27, 0x02000000,
	28, 0x020000a5,
	29, 0x000000a5,
	30, 0x06000001,
	31, 0x00060606,
	32, 0x00000000,
	35, 0x00000000,
	41, 0x00000000,
	43, 0x00000000,
	44, 0x00084000,
	45, 0x00080046,
	46, 0x00460840,
	47, 0x00000008,
	48, 0x08400000,
	49, 0x00080046,
	50, 0x00460840,
	51, 0x00000008,
	52, 0x08400000,
	53, 0x00080046,
	54, 0x00460840,
	55, 0x00000008,
	56, 0x08400000,
	57, 0x00080046,
	58, 0x00460840,
	59, 0x00000008,
	60, 0x00000000,
	62, 0x00000000,
	63, 0x00000000,
	64, 0x00000000,
	65, 0x00000000,
	66, 0x00000000, /* No ECC, set to 0x3 for ECC reporting and correcting */
	67, 0x00000000,
	68, 0x00000000,
	69, 0x00000000,
	70, 0x00000000,
	71, 0x00000000,
	72, 0x00000000,
	73, 0x00000000,
	74, 0x00000000,
	75, 0x00000000,
	76, 0x00000000,
	77, 0x00000000,
	82, 0x01010001,
	89, 0x00000000,
	90, 0x00000000,
	91, 0x00000000,
	92, 0x00000000,
	93, 0x00000000,
	94, 0x00000000,
	95, 0x00000000,
	96, 0x00000000,
	97, 0x00000000,
	98, 0x00000000,
	99, 0x00000000,
	100, 0x00000000,
	101, 0x00000000,
	102, 0x00000000,
	103, 0x00000000,
	104, 0x00000000,
	105, 0x00000000,
	106, 0x00000000,
	107, 0x00000000,
	108, 0x02040108,
	109, 0x08010402,
	110, 0x02020002,
	111, 0x01000200,
	113, 0x00000000,
	114, 0x00000000,
	115, 0x00000000,
	118, 0x00000000,
	124, 0x00000000,
	125, 0x00000000,
	126, 0x00000000,
	127, 0x00000000,
	128, 0x00212100,
	129, 0x21210001,
	131, 0x00000000,
	132, 0x00000000,
	133, 0x00012121,
	134, 0x00012121,
	135, 0x00000000,
	136, 0x00000000,
	137, 0x00212100,
	138, 0x21210001,
	140, 0x00000000,
	141, 0x00000000,
	142, 0x00012121,
	143, 0x00012121,
	144, 0x00000000,
	145, 0x00000000,
	146, 0x00212100,
	147, 0x21210001,
	151, 0x00000000,
	167, 0x00000000,
	169, 0x102d0000,
	172, 0x0000102d,
	173, 0x0000a1c2,
	174, 0x102d0607,
	177, 0x0000102d,
	178, 0x0000a1c2,
	179, 0x02020607,
	183, 0x00000000,
	184, 0x00000000,
	186, 0x00070303,
	188, 0x00000000,
	189, 0x00000000,
	190, 0x00000000,
	191, 0x00000000,
	192, 0x00000000,
	193, 0x00000000,
	195, 0x00000000,
	196, 0x00000000,
	197, 0x00000000,
	198, 0x00000000,
	199, 0x00000000,
	200, 0x00000000,
	201, 0x00000000,
	202, 0x00000006,
	203, 0x00000006,
	204, 0x00000000,
	206, 0x02050501,
	207, 0x00000002,
	208, 0x00000000,
	209, 0x00000000,
	210, 0x08080000,
	211, 0x00000808,
	212, 0x00000040,
	213, 0x00000000,
	214, 0x01010606,
	215, 0x00000101,
	216, 0x00002020,
	217, 0x00000000,
	0xffffffff};

unsigned int ddr3_init_tab_1600[] = {
	0, 0x00000600,
	1, 0x00000000,
	3, 0x000000a0,
	4, 0x00000190,
	5, 0x16081600,
	6, 0x06040408,
	7, 0x0b061c27,
	8, 0x061c2706,
	9, 0x0c04060b,
	10, 0x0400db60,
	11, 0x0c040604,
	12, 0x0400db60,
	13, 0x01010004,
	15, 0x00171700,
	16, 0x03000200,
	17, 0x00002020,
	18, 0x0b0b0000,
	19, 0x00000000,
	20, 0x0000f001,
	21, 0x00f01858,
	22, 0x00051858,
	23, 0x00000500,
	24, 0x00140005,
	25, 0x00000014,
	26, 0x00000000,
	27, 0x02000000,
	28, 0x020000f8,
	29, 0x000000f8,
	30, 0x08000001,
	31, 0x00080808,
	32, 0x00000000,
	35, 0x00000000,
	41, 0x00000000,
	43, 0x00000000,
	44, 0x000c7000,
	45, 0x00180046,
	46, 0x00460c70,
	47, 0x00000018,
	48, 0x0c700000,
	49, 0x00180046,
	50, 0x00460c70,
	51, 0x00000018,
	52, 0x0c700000,
	53, 0x00180046,
	54, 0x00460c70,
	55, 0x00000018,
	56, 0x0c700000,
	57, 0x00180046,
	58, 0x00460c70,
	59, 0x00000018,
	60, 0x00000000,
	62, 0x00000000,
	63, 0x00000000,
	64, 0x00000000,
	65, 0x00000000,
	66, 0x00000000, /* No ECC, set to 0x3 for ECC reporting and correcting */
	67, 0x00000000,
	68, 0x00000000,
	69, 0x00000000,
	70, 0x00000000,
	71, 0x00000000,
	72, 0x00000000,
	73, 0x00000000,
	74, 0x00000000,
	75, 0x00000000,
	76, 0x00000000,
	77, 0x00000000,
	82, 0x01010001,
	89, 0x00000000,
	90, 0x00000000,
	91, 0x00000000,
	92, 0x00000000,
	93, 0x00000000,
	94, 0x00000000,
	95, 0x00000000,
	96, 0x00000000,
	97, 0x00000000,
	98, 0x00000000,
	99, 0x00000000,
	100, 0x00000000,
	101, 0x00000000,
	102, 0x00000000,
	103, 0x00000000,
	104, 0x00000000,
	105, 0x00000000,
	106, 0x00000000,
	107, 0x00000000,
	108, 0x02040108,
	109, 0x08010402,
	110, 0x02020002,
	111, 0x01000200,
	113, 0x00000000,
	114, 0x00000000,
	115, 0x00000000,
	118, 0x00000000,
	124, 0x00000000,
	125, 0x00000000,
	126, 0x00000000,
	127, 0x00000000,
	128, 0x00232300,
	129, 0x23230001,
	131, 0x00000000,
	132, 0x00000000,
	133, 0x00012323,
	134, 0x00012323,
	135, 0x00000000,
	136, 0x00000000,
	137, 0x00232300,
	138, 0x23230001,
	140, 0x00000000,
	141, 0x00000000,
	142, 0x00012323,
	143, 0x00012323,
	144, 0x00000000,
	145, 0x00000000,
	146, 0x00232300,
	147, 0x23230001,
	151, 0x00000000,
	167, 0x00000000,
	169, 0x18580000,
	172, 0x00001858,
	173, 0x0000f370,
	174, 0x1858080a,
	177, 0x00001858,
	178, 0x0000f370,
	179, 0x0202080a,
	183, 0x00000000,
	184, 0x00000000,
	186, 0x00070303,
	188, 0x00000000,
	189, 0x00000000,
	190, 0x00000000,
	191, 0x00000000,
	192, 0x00000000,
	193, 0x00000000,
	195, 0x00000000,
	196, 0x00000000,
	197, 0x00000000,
	198, 0x00000000,
	199, 0x00000000,
	200, 0x00000000,
	201, 0x00000000,
	202, 0x00000008,
	203, 0x00000008,
	204, 0x00000000,
	206, 0x02070701,
	207, 0x00000002,
	208, 0x00000000,
	209, 0x00000000,
	210, 0x0c0b0000,
	211, 0x00000c0b,
	212, 0x00000040,
	213, 0x00000000,
	214, 0x01010606,
	215, 0x00000101,
	216, 0x00002727,
	217, 0x00000000,
	0xffffffff};

ddr3_init_tab_1333[] = { 
	0, 0x00000600,
	1, 0x00000000,
	3, 0x000208d6,
	4, 0x00051616,
	5, 0x12071200,
	6, 0x05040407,
	7, 0x09051821,
	8, 0x05182105,
	9, 0x0c040509,
	10, 0x0400b6d0,
	11, 0x0c040504,
	12, 0x0400b6d0,
	13, 0x01010004,
	15, 0x00131300,
	16, 0x03000200,
	17, 0x00001b1b,
	18, 0x09090000,
	19, 0x00000000,
	20, 0x0000c801,
	21, 0x00c81448,
	22, 0x00051448,
	23, 0x00000400,
	24, 0x00100004,
	25, 0x00000010,
	26, 0x00000000,
	27, 0x02000000,
	28, 0x020000cf,
	29, 0x000000cf,
	30, 0x07000001,
	31, 0x00070707,
	32, 0x00000000,
	35, 0x00000000,
	41, 0x00000000,
	43, 0x00000000,
	44, 0x000a5000,
	45, 0x00100046,
	46, 0x00460a50,
	47, 0x00000010,
	48, 0x0a500000,
	49, 0x00100046,
	50, 0x00460a50,
	51, 0x00000010,
	52, 0x0a500000,
	53, 0x00100046,
	54, 0x00460a50,
	55, 0x00000010,
	56, 0x0a500000,
	57, 0x00100046,
	58, 0x00460a50,
	59, 0x00000010,
	60, 0x00000000,
	62, 0x00000000,
	63, 0x00000000,
	64, 0x00000000,
	65, 0x00000000,
	66, 0x00000000,
	67, 0x00000000,
	68, 0x00000000,
	69, 0x00000000,
	70, 0x00000000,
	71, 0x00000000,
	72, 0x00000000,
	73, 0x00000000,
	74, 0x00000000,
	75, 0x00000000,
	76, 0x00000000,
	77, 0x00000000,
	82, 0x01010001,
	89, 0x00000000,
	90, 0x00000000,
	91, 0x00000000,
	92, 0x00000000,
	93, 0x00000000,
	94, 0x00000000,
	95, 0x00000000,
	96, 0x00000000,
	97, 0x00000000,
	98, 0x00000000,
	99, 0x00000000,
	100, 0x00000000,
	101, 0x00000000,
	102, 0x00000000,
	103, 0x00000000,
	104, 0x00000000,
	105, 0x00000000,
	106, 0x00000000,
	107, 0x00000000,
	108, 0x02040108,
	109, 0x08010402,
	110, 0x02020002,
	111, 0x01000200,
	113, 0x00000000,
	114, 0x00000000,
	115, 0x00000000,
	118, 0x00000000,
	124, 0x00000000,
	125, 0x00000000,
	126, 0x00000000,
	127, 0x00000000,
	128, 0x00222200,
	129, 0x22220001,
	131, 0x00000000,
	132, 0x00000000,
	133, 0x00012222,
	134, 0x00012222,
	135, 0x00000000,
	136, 0x00000000,
	137, 0x00222200,
	138, 0x22220001,
	140, 0x00000000,
	141, 0x00000000,
	142, 0x00012222,
	143, 0x00012222,
	144, 0x00000000,
	145, 0x00000000,
	146, 0x00222200,
	147, 0x22220001,
	151, 0x00000000,
	167, 0x00000000,
	168, 0x000c0c00,
	169, 0x14480000,
	172, 0x00001448,
	173, 0x0000cad0,
	174, 0x14480708,
	177, 0x00001448,
	178, 0x0000cad0,
	179, 0x02020708,
	183, 0x00000000,
	184, 0x00000000,
	186, 0x00070303,
	188, 0x00000000,
	189, 0x00000000,
	190, 0x00000000,
	191, 0x00000000,
	192, 0x00000000,
	193, 0x00000000,
	195, 0x00000000,
	196, 0x00000000,
	197, 0x00000000,
	198, 0x00000000,
	199, 0x00000000,
	200, 0x00000000,
	201, 0x00000000,
	202, 0x00000007,
	203, 0x00000007,
	204, 0x00000000,
	206, 0x02060601,
	207, 0x00000002,
	208, 0x00000000,
	209, 0x00000000,
	210, 0x0a090000,
	211, 0x00000a09,
	212, 0x00000040,
	213, 0x00000000,
	214, 0x01010606,
	215, 0x00000101,
	216, 0x00002323,
	217, 0x00000000,
	0xffffffff
};

unsigned int ddr3_init_tab_933[] = { /* to be added */
	0, 0,
	0xffffffff};

unsigned int ddr2_mode_reg_tab[] = {
		0x0320,
		0x0046,
		0x0000,
		0x0000
};

unsigned int ddr3_mode_reg_tab[] = {
		0x0320,
		0x0046,
		0x0000,
		0x0000
};

unsigned int ddr_clk_tab[] = {
	/*  clk,	ndiv,	MDIV
						0	1	2	3	4	5 */

		333,	120,	30,	18,	0, 100, 15, 24,		
		400,	120,	30,	15,	0, 100, 15, 24,			
		467,    140,	35,	15,	0, 175, 18, 24,		
		533,	128,	32,	12,	0, 80,  16, 24,			
		667,	120,	30,	9,	0, 100, 15, 24,			
		800,	128,	32,	8,	0, 80,  16, 24,			
		0xffffffff
};

unsigned int ddr_phy_wl0_tab[] = {
0x000,	0x0001001a,
0x004,	0x00010000,
0x008,	0x00010022,
0x00c,	0x00010022,
0x010,	0x0001001c,
0x014,	0x0001001c,
0x018,	0x0001001c,
0x01c,	0x0001001c,
0x020,	0x0001001c,
0x024,	0x0001001c,
0x028,	0x0001001c,
0x02c,	0x0001001c,
0x030,	0x00000000,
0x034,	0x0001000a,
0x038,	0x0001000a,
0x03c,	0x0001000a,
0x040,	0x0001000a,
0x044,	0x0001000a,
0x048,	0x0001000a,
0x04c,	0x0001000a,
0x050,	0x0001000a,
0x054,	0x0001000a,
0x058,	0x0001000a,
0x05c,	0x0001000a,
0x060,	0x0001000a,
0x064,	0x0001000a,
0x068,	0x0001000a,
0x06c,	0x0001000a,
0x070,	0x0001000a,
0x074,	0x0001001b,
0x0a4,	0x00010000,
0x0a8,	0x00010021,
0x0ac,	0x00010021,
0x0b0,	0x0001001e,
0x0b4,	0x0001001e,
0x0b8,	0x0001001e,
0x0bc,	0x0001001e,
0x0c0,	0x0001001e,
0x0c4,	0x0001001e,
0x0c8,	0x0001001e,
0x0cc,	0x0001001e,
0x0d0,	0x00000000,
0x0d4,	0x00010009,
0x0d8,	0x00010009,
0x0dc,	0x00010009,
0x0e0,	0x00010009,
0x0e4,	0x00010009,
0x0e8,	0x00010009,
0x0ec,	0x00010009,
0x0f0,	0x00010009,
0x0f4,	0x00010009,
0x0f8,	0x00010009,
0x0fc,	0x00010009,
0x100,	0x00010009,
0x104,	0x00010009,
0x108,	0x00010009,
0x10c,	0x00010009,
0x110,	0x00010009,
0x114,	0x00010019,
0x118,	0x00000000,
0x11c,	0x00000000,
0x120,	0x00000000,
0x124,	0x00000000,
0x128,	0x00000000,
0x12c,	0x00000000,
0x130,	0x00000000,
0x134,	0x00000000,
0x138,	0x00000000,
0x13c,	0x00000000,
0x140,	0x00000000,
0x144,	0x00000000,
0x148,	0x00000000,
0x14c,	0x00000000,
0x150,	0x00000000,
0x154,	0x00000000,
0x158,	0x00000000,
0x15c,	0x00000000,
0x160,	0x00000003,
0x164,	0x00000007,
0x168,	0x00000000,
0x16c,	0x00000000,
0x170,	0x0000b966,
0x174,	0x000063bc,
0x178,	0x0000851a,
0x17c,	0x00000fc0,
0x180,	0x0000b293,
0x184,	0x00007657,
0x188,	0x00003514,
0x18c,	0x0000e2c3,
0x190,	0x00000000,
0x194,	0x00000000,
0x198,	0x00000000,
0x19c,	0x00000000,
0x1a0,	0x00500000,
0x1a4,	0x0000000a,
0x1a8,	0x00000000,
0x1ac,	0x00000003,
0x1b0,	0x00000000,
0xffffffff
};

unsigned int ddr_phy_ctl_ovrd_tab[] = {
0x030,	0x00000000,
0x034,	0x00010028,
0x06c,	0x000007df,
0xffffffff
};
