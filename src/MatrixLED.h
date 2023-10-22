#ifndef _MATRIXLED_H_
#define _MATRIXLED_H_

void MatrixLEDInit();	//初始化
void SetMatrixLED(unsigned char **m);	//设置显示图案
void MatrixLEDLoop();	//重复扫描绘制图像

#endif